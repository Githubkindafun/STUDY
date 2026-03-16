
#include "i2c.c"
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

// bits per second
#define BAUD 9600                          // baudrate
// wartosc wyliczana zeby hardware wygenerowal BAUD
// UBRR -> USART Baud Rate Register = dzielnik zeby miec Baud jaki chcemy
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

// wikipedia to the rescue
// Circular_buffer
#define BUFFER_SIZE 32
#define R_BUFFER 0
#define T_BUFFER 1

volatile uint8_t buffer[2][BUFFER_SIZE];
volatile uint8_t head[2];
volatile uint8_t tail[2];

uint8_t put(uint8_t who, uint8_t byte) {
    uint8_t bufferHead = head[who];
    uint8_t bufferTail = tail[who];

    if((bufferHead + 1) % BUFFER_SIZE == bufferTail) {
        // pelen wiecej nie wejdzie!
        return 0;
    }
    buffer[who][bufferHead] = byte;
    head[who] = (bufferHead + 1) % BUFFER_SIZE;
    return 1;
}

uint8_t get(uint8_t who, uint8_t *byte) {
    uint8_t bufferHead = head[who];
    uint8_t bufferTail = tail[who];

    if(bufferHead == bufferTail) {
        // pusty
        return 0; 
    }
    *byte = buffer[who][bufferTail];
    tail[who] = (bufferTail + 1) % BUFFER_SIZE;
    return 1;
}

// inicjalizacja UART
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // wyczyść rejestr UCSR0A
  UCSR0A = 0;
  // włącz odbiornik i nadajnik
  // dodatkowo odblokowujemy przerwania recive complete
  UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

// to do
// uart_transmit()
// transmisja jednego znaku
int uart_transmit(char data, FILE *stream) {
    while(!put(T_BUFFER,(uint8_t)data)) {
        // pelen
    } 
    UCSR0B |= _BV(UDRIE0);

    return 0;
}

// uart_recive()
int uart_receive(FILE *stream) {
    uint8_t data;
    while(!get(R_BUFFER, &data)) {
        // pusty
    } 


    return data;
}

// USART Rx Complete
// czyli dane przyjechaly (full byte)
ISR(USART_RX_vect) {
    uint8_t data = UDR0;
    put(R_BUFFER, data);

    // echo 
    put(T_BUFFER, data);
    UCSR0B |= _BV(UDRIE0);
    // echo

}

// USART, Data Register Empty
// gotowosc na nowe dane -> UDR0 puste
ISR(USART_UDRE_vect) {
    uint8_t data;
    if(get(T_BUFFER, &data)) {
        UDR0 = data;
    } else {
        // pusto
        UCSR0B &= ~_BV(UDRIE0);
    }
}


FILE uart_file;

void uart_setup() {
    uart_init();
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();
    sei();
}


// wyzej uart znany z przeszlosci
// nizej setup z z1

// 1 0 1 0 E2 E1 BS RW
// E2 E1 - w przypaku kilku EEPROM'ow podlaczonych
// BS - 0-255 / 256 - 511 adresy w pamieci
// RW - read / write
// const uint8_t eeprom_addr = 0xa0;
const uint8_t eeprom_addr_read = 0b10100001;
const uint8_t eeprom_addr_write = 0b10100000;

// fajne macro do sprawdzania status code'ow
#define i2cCheck(code, msg) \
  if ((TWSR & 0xf8) != (code)) { \
    printf(msg " failed, status: %.2x\r\n", TWSR & 0xf8); \
    i2cReset(); \
    continue; \
  }

void setup() {
    // zainicjalizuj UART
    uart_setup();
    i2cInit();
}

void write(uint8_t addr, uint8_t value) {
    i2cStart();
    // DEV SEL
    i2cSend(eeprom_addr_write);
    // BYTE ADDR
    i2cSend(addr);
    // DATA IN
    i2cSend(value);
    i2cStop();
    // gdzies w dokumentacji bylo
    _delay_ms(10);
}

uint8_t read(uint8_t addr) {
    uint8_t value;
    i2cStart();
    // DEV SEL
    i2cSend(eeprom_addr_write);
    // BYTE ADDR
    i2cSend(addr);

    i2cStart();
    // DEV SEL
    i2cSend(eeprom_addr_read);
    // DATA OUT
    value = i2cReadNoAck();
    
    i2cStop();

    return value;
}

// dobra tu sa util funkcje do parsowania inputu z konsoli

void readLine(char *lineBuffer, uint8_t bufferLength) {
    uint8_t i = 0;
    while(1) {
        int ch = getchar();
        if(ch == '\r' || ch == '\n') {
            lineBuffer[i] = '\0';
            break;
        }
        if(i < bufferLength - 1) {
            lineBuffer[i] = (char)ch;
            i++;
        }
    }
}

uint8_t skipInstruction(char *bashLine, uint8_t i) {
    while(bashLine[i] != '\0' && bashLine[i] != ' ') {
        i++;
    } // pomiamy instruckje
    return i;
}

uint8_t skipSpaces(char *bashLine, uint8_t i) {
    while(bashLine[i] == ' ') {
        i++;
    } // pomijamy spacje
    return i;
}

uint16_t parseUint(char *lineBuffer, uint8_t *i, uint8_t maxDigits) {
    uint16_t value = 0;
    uint8_t digits = 0;
    char ch;
    
    while(1) {
        ch = lineBuffer[*i];

        if(ch < '0' || ch > '9') {
            break;
        }
        value = (uint16_t)(value * 10 + (uint8_t)(ch - '0'));
        (*i)++;
        digits++;
        if(digits >= maxDigits) {
            break;
        }
    }
    return value;
}


// koniec contentu z z1
// teraz get/set date/time

const uint8_t ds3231_addr_read = 0b11010001;
const uint8_t ds3231_addr_write = 0b11010000;

uint8_t bcdToDecimal(uint8_t bcd) {
    uint8_t tens = (uint8_t)(bcd >> 4) * 10;
    uint8_t ones = (uint8_t)(bcd & 0b00001111);
    return tens + ones;
}

uint8_t decimalToBcd(uint8_t decimal) {
    uint8_t tens = 0;
    while (decimal >= 10) {
        decimal -= 10;
        tens++;
    }
    // wciskamy dziesiatki i doklejamy juz nie decimal ale realnie ones
    return (uint8_t)((tens << 4) | decimal);
    
}

uint8_t getValue(uint8_t registerAdress) {
    uint8_t value;

    i2cStart();
    i2cSend(ds3231_addr_write);
    i2cSend(registerAdress);
    i2cStart();
    i2cSend(ds3231_addr_read);
    value = i2cReadNoAck();
    i2cStop();

    return value;
}

uint8_t setDate(uint8_t day, uint8_t month, uint16_t year) {
    if(day < 1 || day > 31) {
        return 0; // pomijam zabawe z lutym ;)
    }
    if(month < 1 || month > 12) {
        return 0;
    }
    if(year < 2000 || year > 2199) {
        return 0;
    }
    // tutaj jest case tego bita century
    // zasadniczo +100 or not +100 i tyle
    uint8_t year8bit;
    uint8_t century = 0;
    if(year > 2099) {
        century = 1;
        year8bit = (uint8_t)(year - 2100);
    } else {
        century = 0;
        year8bit = (uint8_t)(year - 2000);
    }
    // wlasnie zauwazylem ze day != dzien tylko = dzien tyg ale dziala
    // teraz data do przeslania
    uint8_t dayData = decimalToBcd(day);
    uint8_t monthData = decimalToBcd(month);
    if(century) {
        // ten 7 bit century
        monthData |= 0b10000000;
    }
    // 2025 -> 25 ;)
    uint8_t yearData = decimalToBcd(year8bit);

    i2cStart();
    i2cSend(ds3231_addr_write);
    // ok i tu fajny trik, jak przeslemy jakis adres np dnia
    // to kazdy nastepny wyslany bedzie przechodzil po kolejnych
    // rejestrach day -> month -> year -> STOP
    i2cSend(0x04);
    i2cSend(dayData);
    i2cSend(monthData);
    i2cSend(yearData);
    i2cStop();
    return 1;
}

uint8_t setTime(uint8_t second, uint8_t minute, uint8_t hour) {
    if(second > 59) {
        return 0;
    }
    if(minute > 59) {
        return 0;
    }
    if(hour > 23) {
        return 0;
    }
    // teraz data do przeslania
    uint8_t secondData = decimalToBcd(second);
    uint8_t minuteData = decimalToBcd(minute);
    // chcemy 24 godzinnny system
    uint8_t hourData = decimalToBcd(hour) & 0b00111111;

    i2cStart();
    i2cSend(ds3231_addr_write);
    // ok i tu fajny trik, jak przeslemy jakis adres np dnia
    // to kazdy nastepny wyslany bedzie przechodzil po kolejnych
    // rejestrach second -> minute -> hour -> STOP
    i2cSend(0x00);
    i2cSend(secondData);
    i2cSend(minuteData);
    i2cSend(hourData);
    i2cStop();
    return 1;
}



void parseGetDate() {
    // w data sheet jest ladna tabelka
    uint8_t dateValue = getValue(0x04);
    uint8_t mothValue = getValue(0x05);
    uint8_t yearValue = getValue(0x06);

    uint8_t dayToPrint = bcdToDecimal(dateValue & 0b00111111);
    uint8_t monthToPrint = bcdToDecimal(mothValue & 0b00011111);
    uint8_t year8ToPrint = bcdToDecimal(yearValue);
    uint16_t yearToPrint = year8ToPrint + 2000;

    printf("%02u-%02u-%04u\r\n", dayToPrint, monthToPrint, yearToPrint);
}

void parseGetTime() {
    // w data sheet jest ladna tabelka
    uint8_t secondValue = getValue(0x00);
    uint8_t minuteValue = getValue(0x01);
    uint8_t hourValue = getValue(0x02);

    uint8_t secondToPrint = bcdToDecimal(secondValue & 0b01111111);
    uint8_t minuteToPrint = bcdToDecimal(minuteValue & 0b01111111);
    uint8_t hourToPrint = bcdToDecimal(hourValue & 0b00111111);

    printf("%02u-%02u-%02u\r\n", hourToPrint, minuteToPrint, secondToPrint);
}


void parseSetDate(char *lineBuffer) {
    uint8_t i = 0;
    i = skipInstruction(lineBuffer, i);
    i = skipSpaces(lineBuffer, i);
    
    if(lineBuffer[i] != 'd') {
        printf("buddy you should write: set date <DD-MM-YYYY>\r\n");
        return;
    }
    i = skipInstruction(lineBuffer, i);
    i = skipSpaces(lineBuffer, i); 
    
    uint8_t day = (uint8_t)parseUint(lineBuffer, &i, 2);
    if(lineBuffer[i] != '-') {
        printf("something went wrong buddy in parseSetDate");
    }
    i++; // -
    uint8_t month = (uint8_t)parseUint(lineBuffer, &i, 2);
    if(lineBuffer[i] != '-') {
        printf("something went wrong buddy in parseSetDate");
    }
    i++; // -
    uint16_t year = parseUint(lineBuffer, &i, 4);
    
    if(setDate(day, month, year)) {
        printf("date set: %02u-%02u-%04u\r\n", day, month, year);
    } else {
        printf("something went wrong while seting the date");
    }
}

void parseSetTime(char *lineBuffer) {
    uint8_t i = 0;
    i = skipInstruction(lineBuffer, i);
    i = skipSpaces(lineBuffer, i);
    
    if(lineBuffer[i] != 't') {
        printf("buddy you should write: set time <HH-MM-SS>\r\n");
        return;
    }
    i = skipInstruction(lineBuffer, i);
    i = skipSpaces(lineBuffer, i); 
    
    uint8_t hour = (uint8_t)parseUint(lineBuffer, &i, 2);
    if(lineBuffer[i] != '-') {
        printf("something went wrong buddy in parseSetTime");
    }
    i++; // -
    uint8_t minute = (uint8_t)parseUint(lineBuffer, &i, 2);
    if(lineBuffer[i] != '-') {
        printf("something went wrong buddy in parseSetTime");
    }
    i++; // -
    uint8_t second = (uint8_t)parseUint(lineBuffer, &i, 2);
    
    if(setTime(second, minute, hour)) {
        printf("time set: %02u-%02u-%02u\r\n", hour, minute, second);
    } else {
        printf("something went wrong while seting the time");
    }
}

void parseSet(char *lineBuffer) {
    uint8_t i = 0;
    i = skipInstruction(lineBuffer, i);
    i = skipSpaces(lineBuffer, i);
    if(lineBuffer[i] == 'd') {
        parseSetDate(lineBuffer);
    } else if(lineBuffer[i] == 't') {
        parseSetTime(lineBuffer);
    } else {
        printf("buddy i can  only set date or time");
    }
}

int main()
{
    setup();
    char bashLine[64];
    printf("2 functions for date:\r\n    date -> zwrata DD-MM-YYYY\r\n    set date <DD-MM-YYYY>\r\n");
    printf("2 functions for time:\r\n    time -> zwrata HH-MM-SS\r\n    set time <HH-MM-SS>\r\n");
    
    while(1) {
        readLine(bashLine, 64);
        if(bashLine[0] == '\0') {
            continue;
        }
        if(bashLine[0] == 'd') {
            parseGetDate();
        } else if(bashLine[0] == 't') {
            parseGetTime();
        } else if(bashLine[0] == 's') {
            parseSet(bashLine);
        } else {
            printf("no clue what you meant buddy");
        }
    }
}


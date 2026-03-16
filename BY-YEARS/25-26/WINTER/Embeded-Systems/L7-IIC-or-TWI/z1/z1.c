
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

// rozwiazanie robie z lekka klapka na oczy ze poza adres 255 nie patrzymy ;)
int main()
{
    setup();
    char bashLine[64];
    printf("2 functions read <addr>, write <addr> <value>\r\n");

    while(1) {
        readLine(bashLine, 64);
        uint8_t i = 0;
        if(bashLine[0] == '\0') {
            continue;
        }
        if(bashLine[0] == 'w') {
            uint8_t addr = 0;
            uint8_t value = 0;

            i = skipInstruction(bashLine, i);
            i = skipSpaces(bashLine, i);

            while(bashLine[i] >= '0' && bashLine[i] <= '9') {
                addr = addr * 10 + (uint8_t)(bashLine[i] - '0');
                i++;
            } // sczytujemy adres

            i = skipSpaces(bashLine, i);


            while(bashLine[i] >= '0' && bashLine[i] <= '9') {
                value = value * 10 + (uint8_t)(bashLine[i] - '0');
                i++;
            } // sczytujemy value

            write(addr, value);
            printf("write completed c:\r\n");

        } else if(bashLine[0] == 'r') {
            uint8_t addr = 0;

            i = skipInstruction(bashLine, i);
            i = skipSpaces(bashLine, i);

            while(bashLine[i] >= '0' && bashLine[i] <= '9') {
                addr = addr * 10 + (uint8_t)(bashLine[i] - '0');
                i++;
            } // sczytujemy adres


            uint8_t value = read(addr);
            printf("read completed: \r\n%"PRId8, value);
        } else {
            printf("no clue what you meant");
        }
        
        // chcemy:
        // read addr 
        // write addr value
        
    }
}


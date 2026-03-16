#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>


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

volatile uint8_t dataFromMaster = 0;
volatile uint8_t newFlag = 0;


void slave_init() {
    // z datasheeta
    // To initiate the Slave Receiver mode,
    // TWAR and TWCR must be initialized as follows:
    // adres
    TWAR = (uint8_t)(0x7f << 1);
    //
    TWCR = _BV(TWEN) | _BV(TWEA);
    // interrupt odblokowany
    // i flaga "wyczyszczona" -> start twi
    TWCR |= _BV(TWIE) | _BV(TWINT); 
}

ISR(TWI_vect) {
    // prescalerowe bity 0
    uint8_t statusCode = TWSR & 0b11111000;

    // przy kazdym interupcie iic bedzie czekac 
    // az powimy mu co ma dalej robic
    // stad powtarzajace sie linijki z inita
    // mowiace co ma robic
    switch(statusCode) {
        case 0x60:
        case 0x68:
            TWCR = _BV(TWEN) | _BV(TWEA);
            // interrupt odblokowany
            // i flaga "wyczyszczona" -> start twi
            TWCR |= _BV(TWIE) | _BV(TWINT); 
            break;
        case 0x80:
            dataFromMaster = TWDR;
            newFlag = 1;
            TWCR = _BV(TWEN) | _BV(TWEA);
            // interrupt odblokowany
            // i flaga "wyczyszczona" -> start twi
            TWCR |= _BV(TWIE) | _BV(TWINT); 
            break;
        default:
            TWCR = _BV(TWEN) | _BV(TWEA);
            // interrupt odblokowany
            // i flaga "wyczyszczona" -> start twi
            TWCR |= _BV(TWIE) | _BV(TWINT); 
            break;
    }
}

void setup() {
    slave_init();
    uart_init();
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();
    sei();
}

int main() {
    setup();

    while(1) {
        if(newFlag) {
            newFlag = 0;
            printf("wartosc od mastera %"PRId8"\r\n", dataFromMaster);
        }
        sleep_mode();
    }
}
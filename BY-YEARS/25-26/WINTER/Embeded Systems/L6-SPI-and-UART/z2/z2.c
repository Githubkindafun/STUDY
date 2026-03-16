
/*
Przykładowa implementacja wejścia/wyjścia przez UART z pliku helloworld.c wykorzystuje aktywne cze-
kanie, co ma pewne wady.
Podczas nadawania i odbierania procesor większość czasu spędza na sprawdzaniu
bitu statusu, a odbieranie działa poprawnie tylko wtedy, gdy program wykonuje funkcje odczytywania. Po-
praw te wady, implementując rozwiązanie oparte na przerwaniach, wykorzystujące dwa bufory cykliczne:
nadawania i odbierania.

Napisz procedury obsługi przerwań USART RX i USART UDRE1 oraz nowe implementacje funkcji uart transmit
i uart receive unikające aktywnego czekania. Funkcja nadająca może czekać tylko wtedy, jeśli bufor nada-
wania jest pełny, zaś funkcja odbierająca tylko wtedy, gdy bufor odbierania jest pusty. Pamiętaj o prawi-
dłowej synchronizacji funkcji z procedurami obsługi przerwań przy użyciu maskowania. Przetestuj swoje
rozwiązanie.
*/

// USART_RX, USART_UDRE
// uart_transmit()
// uart_recive()
// bez aktywnego czekania !
// nadajaca czeka gdy bufor pelny
// odbierajaca gdy bufor pusty



#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

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

void setup() {
    uart_init();
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();
    sei();
}


int main()
{

  setup();

//   fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
//   stdin = stdout = stderr = &uart_file;

//   printf("Hello world!\r\n");
  while(1) {
    int16_t a = 1;
    scanf("%"SCNd16, &a);
    printf("Odczytano: %"PRId16"\r\n", a);
    sleep_mode();
  }
}
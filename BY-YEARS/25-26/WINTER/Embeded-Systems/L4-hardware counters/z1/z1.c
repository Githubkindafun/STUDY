#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

// inicjalizacja UART
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // wyczyść rejestr UCSR0A
  UCSR0A = 0;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

// transmisja jednego znaku
int uart_transmit(char data, FILE *stream)
{
  // czekaj aż transmiter gotowy
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}

void timer1_init()
{
  // ustaw tryb licznika
  // WGM1  = 0000 -- normal
  // CS1   = 001  -- prescaler 1
  TCCR1B = _BV(CS10);
}
FILE uart_file;


// udalo sie poprawic
// tak ze widac to w lst

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // zainicjalizuj licznik
  timer1_init();
  // program testowy
  volatile uint16_t a;
  volatile uint16_t b;

  volatile uint8_t n8a = 131;
  volatile uint8_t n8b = 7;
  uint8_t res8;
  

  volatile uint16_t n16a = 4123;
  volatile uint16_t n16b = 2123;
  uint16_t res16;

  volatile uint32_t n32a = 512412;
  volatile uint32_t n32b = 121251;
  uint32_t res32;

  volatile uint64_t n64a = 1552344;
  volatile uint64_t n64b = 1513413;
  uint64_t res64;

  volatile float nFa = 42.42;
  volatile float nFb = 24.24;
  float resF;
  
  a = TCNT1; 
  b = TCNT1;
  uint16_t empty = b - a;

  while(1) {

    // 8 bitow operacje
    // bez volatile aby byly w rejestrach
    uint8_t x8 = n8a;
    uint8_t y8 = n8b;
    a = TCNT1; // wartość licznika przed czekaniem
    res8 = x8 + y8;
    b = TCNT1; // wartość licznika po czekaniu
    uint16_t time = (uint16_t)(b - a);
    printf("Wynik operacji [ 8bit + ]: %"PRIu8" \r\n", res8);
    printf("Zmierzony czas [ 8bit + ]: %"PRIu16" cykli\r\n \r\n", time - empty);
    
    a = TCNT1; // wartość licznika przed czekaniem
    res8 = x8 * res8;
    b = TCNT1; // wartość licznika po czekaniu
    time = (uint16_t)(b - a);
    printf("Wynik operacji [ 8bit * ]: %"PRIu8" \r\n", res8);
    printf("Zmierzony czas [ 8bit * ]: %"PRIu16" cykli\r\n \r\n", time - empty);

    a = TCNT1; // wartość licznika przed czekaniem
    res8 = y8 / res8;
    b = TCNT1; // wartość licznika po czekaniu
    time = (uint16_t)(b - a);
    printf("Wynik operacji [ 8bit / ]: %"PRIu8" \r\n", res8);
    printf("Zmierzony czas [ 8bit / ]: %"PRIu16" cykli\r\n \r\n", time - empty);

    // 16 bit bitow operacje
    uint8_t x16 = n16a + res8;
    uint8_t y16 = n16b;
    a = TCNT1; // wartość licznika przed czekaniem
    res16 = x16 + y16;
    b = TCNT1; // wartość licznika po czekaniu
    time = (uint16_t)(b - a);
    printf("Wynik operacji [ 16bit + ]: %"PRIu16" \r\n", res16);
    printf("Zmierzony czas [ 16bit + ]: %"PRIu16" cykli\r\n \r\n", empty);
    
    a = TCNT1; // wartość licznika przed czekaniem
    res16 = x16 * res16;
    b = TCNT1; // wartość licznika po czekaniu
    time = (uint16_t)(b - a);
    printf("Wynik operacji [ 16bit * ]: %"PRIu16" \r\n", res16);
    printf("Zmierzony czas [ 16bit * ]: %"PRIu16" cykli\r\n \r\n", time - empty);

    a = TCNT1; // wartość licznika przed czekaniem
    res16 = y16 / res16;
    b = TCNT1; // wartość licznika po czekaniu
    time = (uint16_t)(b - a);
    printf("Wynik operacji [ 16bit / ]: %"PRIu16" \r\n", res16);
    printf("Zmierzony czas [ 16bit / ]: %"PRIu16" cykli\r\n \r\n", time - empty);

    // 32 bit bitow operacje
    uint8_t x32 = n32a + res16;
    uint8_t y32 = n32b;
    a = TCNT1; // wartość licznika przed czekaniem
    res32 = x32 + y32;
    b = TCNT1; // wartość licznika po czekaniu
    time = (uint16_t)(b - a);
    printf("Wynik operacji [ 32bit + ]: %"PRIu32" \r\n", res32);
    printf("Zmierzony czas [ 32bit + ]: %"PRIu16" cykli\r\n \r\n", time - empty);
    
    a = TCNT1; // wartość licznika przed czekaniem
    res32 = x32 * res32;
    b = TCNT1; // wartość licznika po czekaniu
    time = (uint16_t)(b - a);
    printf("Wynik operacji [ 32bit * ]: %"PRIu32" \r\n", res32);
    printf("Zmierzony czas [ 32bit * ]: %"PRIu16" cykli\r\n \r\n", time - empty);

    a = TCNT1; // wartość licznika przed czekaniem
    res32 = y32 / res32;
    b = TCNT1; // wartość licznika po czekaniu
    time = (uint16_t)(b - a);
    printf("Wynik operacji [ 32bit / ]: %"PRIu32" \r\n", res32);
    printf("Zmierzony czas [ 32bit / ]: %"PRIu16" cykli\r\n \r\n", time - empty);

    // 64 bit bitow operacje
    uint8_t x64 = n64a + res32;
    uint8_t y64 = n64b;
    a = TCNT1; // wartość licznika przed czekaniem
    res64 = x64 + y64;
    b = TCNT1; // wartość licznika po czekaniu
    time = (uint16_t)(b - a);
    printf("Wynik operacji [ 64bit + ]: %"PRIu32" \r\n", res64);
    printf("Zmierzony czas [ 64bit + ]: %"PRIu16" cykli\r\n \r\n", time - empty);
    
    a = TCNT1; // wartość licznika przed czekaniem
    res64 = x64 * res64;
    b = TCNT1; // wartość licznika po czekaniu
    time = (uint16_t)(b - a);
    printf("Wynik operacji [ 64bit * ]: %"PRIu32" \r\n", res64);
    printf("Zmierzony czas [ 64bit * ]: %"PRIu16" cykli\r\n \r\n", time - empty);

    a = TCNT1; // wartość licznika przed czekaniem
    res64 = y64 / res64;
    b = TCNT1; // wartość licznika po czekaniu
    time = (uint16_t)(b - a);
    printf("Wynik operacji [ 64bit / ]: %"PRIu32" \r\n", res64);
    printf("Zmierzony czas [ 64bit / ]: %"PRIu16" cykli\r\n \r\n", time - empty);

    // float bitow operacje
    uint8_t xF = nFa + res64;
    uint8_t yF = nFb;
    a = TCNT1; // wartość licznika przed czekaniem
    resF = xF + yF;
    b = TCNT1; // wartość licznika po czekaniu
    time = (uint16_t)(b - a);
    printf("Wynik operacji [ float + ]: %f \r\n", resF);
    printf("Zmierzony czas [ float + ]: %"PRIu16" cykli\r\n \r\n", time - empty);
    
    a = TCNT1; // wartość licznika przed czekaniem
    resF = xF * resF;
    b = TCNT1; // wartość licznika po czekaniu
    time = (uint16_t)(b - a);
    printf("Wynik operacji [ float * ]: %f \r\n", resF);
    printf("Zmierzony czas [ float * ]: %"PRIu16" cykli\r\n \r\n", time - empty);

    a = TCNT1; // wartość licznika przed czekaniem
    resF = yF / resF;
    b = TCNT1; // wartość licznika po czekaniu
    time = (uint16_t)(b - a);
    resF = yF / resF;
    printf("Wynik operacji [ float / ]: %f \r\n", resF);
    printf("Zmierzony czas [ float / ]: %"PRIu16" cykli\r\n \r\n", time - empty);
    
    _delay_ms(5000);
  }
}


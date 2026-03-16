
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

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

// void timer1_init()
// {
//   // ustaw tryb licznika
//   // WGM1  = 0000 -- normal
//   // CS1   = 001  -- prescaler 1
//   TCCR1B = _BV(CS10);
// }
FILE uart_file;
// ---------------

// ICP1 - D8 - PB0
#define INPUT_PIN PB0
#define INPUT_DDR DDRB
volatile uint16_t timeStampLast = 0;
volatile uint16_t ticksInAPeriod = 0;
volatile uint8_t newMeasurment = 0;

void timer1_init() {
    // ustawiamy port PB0 na input
    INPUT_DDR &= ~_BV(INPUT_PIN);

    // Input Capture interrupt odblokowany
    TIMSK1 |= _BV(ICIE1);

    // overflow?
    // w tresci powiedziane jest ze czestotliwosc powinna byc
    // w teori od 190 - 250 Hz
    // w data sheet wspomniane jest aby uwazac na overflow
    // w przypadku licznika, bo jak w czasie jak chcemy zrobic
    // pomiar 2 razy sie przekreci to mierzenie tego juz nie jest
    // takie frywolne.
    // skoro mamy od 190 - 250 to oznacza ze okres to bedzie
    // 1/250 - 1/190 <=> 4ms - ~5.3ms
    // wezmy prescaler 8
    // fq = 16mln / 8 = 2mln -> 1tick = 1/2mln = 0.5qs
    // dla 190-250 to 
    // 0.004 s * 2mln = 8k tickow
    // 0.0053 s * 2mln ~ 10.6k tickow
    // a zegar od 0 do 65535 potrzebuje 65535 * 0.5qs
    // czyli potrzebuje +/- 32ms 
    // czyli widzimy ze nie ma jak sie 2 razy przekrecic licznik

    // czemu nie dac noise canceling
    TCCR1B |= _BV(ICNC1);

    // prescaler - 64 - wsm mniejszy tez git 8 tez da rade
    // TCCR1B |= _BV(CS10) | _BV(CS11);
    TCCR1B |= _BV(CS11);

    // capture on a rising edge
    TCCR1B |= _BV(ICES1); 

}

ISR(TIMER1_CAPT_vect) {
    uint16_t timeStampAtTheMoment = ICR1;
    
    // dobra taki bajer na wiki znalazlem
    // In C, unsigned integer overflow is defined to 
    // wrap around, while signed integer overflow causes 
    // undefined behavior. 
    // czyli wystarczy
    uint16_t ticksElapsed = timeStampAtTheMoment - timeStampLast;
    timeStampLast = timeStampAtTheMoment;
    if(ticksElapsed > 0) {
        ticksInAPeriod = ticksElapsed;
        newMeasurment = 1;
    } else {
        ticksInAPeriod = 0;
    }
}

void setup() {   
    timer1_init();
    sei();
    sleep_enable();
    set_sleep_mode(SLEEP_MODE_IDLE);
}

int main() {
    setup();
    uart_init();
    // skonfiguruj strumienie wejścia/wyjścia
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;

    while(1) {
        sleep_mode();
        if(newMeasurment) {
            uint16_t ticks;

            cli();
            newMeasurment = 0;
            ticks = ticksInAPeriod;
            sei();
            if(ticks == 0) {
                continue;
            } else {
                // (16MHz / preskaler) / ticks = 
                uint32_t frequencyMeasure = (16000000UL / 8UL) / (uint32_t)ticks;
                printf("Ostatnio zmierzona czestotliwosc : %"PRIu32"Hz \r\n", frequencyMeasure);
            }

        }
    }
}
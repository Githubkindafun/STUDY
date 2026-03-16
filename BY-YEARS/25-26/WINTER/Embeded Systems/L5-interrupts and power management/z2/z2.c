
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

#define BTN PD2
#define BTN_PIN PIND
#define BTN_PORT PORTD

volatile uint16_t lastAdcRead = 0;
volatile uint8_t printingTimerFlag = 0;

// adc init
void adc_init() {
    // napiecie referencyjne na AVcc
    // fotorezystor w porownaniu do AVcc
    ADMUX |= _BV(REFS0);

    // ustawiamy ADCSRA
    // ADC on
    ADCSRA |= _BV(ADEN);
    // ADC interrupt on
    ADCSRA |= _BV(ADIE);
    // ADC auto trigger on
    ADCSRA |= _BV(ADATE);

    // ustawiamy ADCSRB
    // external interrupt request 0
    ADCSRB |= _BV(ADTS1);

    // ADCSRA |= _BV(ADSC);
}
// adc interrupt
ISR(ADC_vect) {
    uint16_t adcRead = ADC;
    lastAdcRead = adcRead;
    // EIFR - external interrupt flag
    // czyscimy flage aby moc potem uzyc ponownie
    // EIFR |= _BV(INTF0);       // <- jak to sobie odkomentujemy
}

// btn init
void btn_init() {
    // PIND |= _BV(BTN_PIN);
    // pull-up
    BTN_PORT |= _BV(BTN);

    // konifurujemy INT0 - external interrupt 0 
    // "The falling edge of INT0 generates an interrupt request."
    EICRA |= _BV(ISC01);
    EIMSK = _BV(INT0);     // <- jak to sobie wykomentujemy
    // bedziemy obslugiwac przerwanie int0 (zewnetrzne przerwanie)
}   

ISR(INT0_vect) {
  //
}     // <- jak to sobie też wykomentujemy
      // to tez bedzie dzialac :) 

// 

// timer 1 - basicly do printowania
void timer_init() {
    // ustaiamy ctc
    TCCR1B |= _BV(WGM12);

    // odblokowujemy timer1 compare A interrupt
    TIMSK1 |= _BV(OCIE1A);

    // prescaler na 256
    TCCR1B |= _BV(CS12);

    // aby osiagnac print co 0.5s 
    OCR1A = 31499;
    // 16mln / 256 -> 62.5k Hz => skoro chcemy 0.5s to 62.5k * 0.5 -> 32500 - 1
}

// timer interrupt
ISR(TIMER1_COMPA_vect) {
    printingTimerFlag = 1;
}

// timer interrupt

void setup() {
  adc_init();
  timer_init();
  btn_init();
  sei();
}


int main() {
    setup();
    uart_init();
    // skonfiguruj strumienie wejścia/wyjścia
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
    uint32_t resistanceValue = 0;
    while(1) {

        if(printingTimerFlag) {
          printingTimerFlag = 0;
          
          uint16_t adcRead;
          
          // sekcja krytyczna
          cli();
          adcRead = lastAdcRead;
          sei();

          if(adcRead > 0) {
            // wzorek to: wpisalem "calculating resistance from adc read" w przegladarke
            resistanceValue = (10000UL * (1024UL - adcRead)) / adcRead;
          }
          printf("Ostatnio zmierzona wartosc w Omach : %"PRIu32" \r\n", resistanceValue);
        }
    }
}





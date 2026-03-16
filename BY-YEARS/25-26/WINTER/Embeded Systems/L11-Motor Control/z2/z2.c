#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

// inicjalizacja UART
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
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

FILE uart_file;

#define SPINNER PB1
#define SPINNER_DDR DDRB

void adc_init() {
    // napięcie referencyjne na Vcc
    ADMUX = _BV(REFS0);
    // prescaler 128 i ADC ON
    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
    // wlaczamy interrupty
    ADCSRA |= _BV(ADIE);
    // Bit 5:0 – ADC5D...ADC0D: ADC5...0 Digital Input Disable (datasheet)
    DIDR0 |= _BV(ADC1D) | _BV(ADC0D);
}


void timer1_init() {
    // clear on compare match - setup low level (z datasheet)
    TCCR1A = _BV(COM1A1);
    // wybieramy PWM phase and frequency correct z top jako ICR1
    TCCR1B |=  _BV(WGM13);
    // prescaler 64 bo chcemy miec 500 Hz
    TCCR1B |= _BV(CS11) | _BV(CS10);
    TCCR1B |= _BV(ICNC1);
    // do wzorku dziala
    ICR1 = 250;
    // a bo czemu nie na 0
    OCR1A = 0;
    // czyscimy na wszelki flagi
    TIFR1 = _BV(TOV1) | _BV(ICF1);
    // ICIE1: Timer/Counter1, Input Capture Interrupt Enable (z datasheet)
    TIMSK1 |= _BV(ICIE1);
    // TOIE1: Timer/Counter1, Overflow Interrupt Enable (z datasheet)
    TIMSK1 |= _BV(TOIE1);
}

volatile uint16_t adcCurrentRead = 0;
volatile uint16_t adcSpeedRead = 0;
volatile uint16_t adcPotentiometerRead = 0;
volatile uint8_t adcNewRead = 0;
volatile uint8_t adcPhase = 0;
volatile uint8_t safety = 0;
// gdzie 0 to nic 1 to ovf, 2 to potencjometr, 3 capt

ISR(TIMER1_CAPT_vect) {
    // odpala sie jak dojdziemy w tym trybie do top
    if(adcPhase != 0) {
        return;
    }
    if(ADCSRA & _BV(ADSC)) {
        return;
    }
    adcPhase = 3;
    // na A0 mamy pomiar wiec na A0 przelaczamy
    ADMUX = (ADMUX & 0xF0) | 0;
    // adc start
    ADCSRA |= _BV(ADSC);
    // z data sheeta :
    // the ICF1 Flag is set when the counter reaches the TOP value.
    // na wszelki czyscimy
    TIFR1 = _BV(ICF1);
}

ISR(TIMER1_OVF_vect) {
    // odpala sie po calym gora dol czyli na BOTTOM
    if(adcPhase != 0) {
        return;
    }
    if(ADCSRA & _BV(ADSC)) {
        return;
    }
    adcPhase = 1;
    // na A0 mamy pomiar wiec na A0 przelaczamy
    ADMUX = (ADMUX & 0xF0) | 0;
    // adc start
    ADCSRA |= _BV(ADSC);
}

ISR(ADC_vect) {
    uint16_t adc = ADC;
    if(safety) {
        safety = 0;
        ADCSRA |= _BV(ADSC);
        return;
    }
    // mamy 3 typy pomiarow z ovf/capt/potencjometr
    if(adcPhase == 1) {
        adcCurrentRead = adc;
        adcPhase = 2;
        // przechodzimy na A1
        ADMUX = (ADMUX & 0xF0) | 1;
        // no i niech zacznie tam sobie liczyc
        ADCSRA |= _BV(ADSC);
        safety = 1;
        // adcNewRead = 1;
    } else if(adcPhase == 2) {
        adcPotentiometerRead = adc;
        adcPhase = 0;
        // adcNewRead = 1;
    } else if(adcPhase == 3) {
        adcSpeedRead = adc;
        adcPhase = 0;
        adcNewRead = 1;
    } else {
        // gdy 0 no to 0
        // a jak jakims cudem sie popsuje to
        // na wszelki 0
        adcPhase = 0;
    }
}

void setup() {
    adc_init();
    // dobra chcemy miec output ustawiony
    SPINNER_DDR |= _BV(SPINNER);
    timer1_init();
    uart_init();
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
    sei();
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();
}

// zasadniczo dzialalo ale mam poderzenie ze cos z motorkiem jest nie tak
 
int main() {
    setup();
    uint16_t loopCounter = 0;
    while(1) {
        if(adcNewRead) {
            uint16_t current = 0;
            uint16_t speed = 0;
            uint16_t potentiometer = 0;
            cli();
            // sekcja krytyczna aby nikt nam nic nie zmienil
            adcNewRead = 0;
            current = adcCurrentRead;
            speed = adcSpeedRead;
            potentiometer = adcPotentiometerRead;
            sei();
            // na podstawie wartosci potencjometru zmieniamy duty
            // co zmieni predkosc silniczka
            
            // uint32_t ocr = (uint32_t)potentiometer * (((uint32_t)250 / (uint32_t)1024) + 1); 
            uint16_t ocr = (uint32_t)potentiometer * ICR1 / (uint32_t)1023;
            if(ocr > ICR1) {
                ocr = ICR1;
            }
            cli();
            OCR1A = ICR1 - ocr;
            sei();
            loopCounter++;

            if(loopCounter >= 24) {
                // wg zadania current i predkosc to bedzie
                // tutaj bedzie na odwrot jednak bo potencjlanie zle 
                // zinterpretowalem to na co patrzylem

                // finally lekkie pomylki byly ale po lekkim poprestawianu zmiennych
                // wyglada jak by mialao dzialac 


                // uint32_t mVCurrent = (uint32_t)5000 - (uint32_t)(speed * (uint32_t)5000) / (uint32_t)1023;
                uint32_t mVCurrent =  ((uint32_t)current * (uint32_t)5000 / (uint32_t)1023);
                // uint32_t mVSpeed = (uint32_t)(current * (uint32_t)5000) / (uint32_t)1023;
                uint32_t mVSpeed = 5000 - ((uint32_t)speed * (uint32_t)5000 / (uint32_t)1023);

                // loopCounter++;
                // aby uart nie latal za bardzo
                // if(loopCounter >= 24) {
                loopCounter = 0;
                printf("TOP: %"PRIu32, mVSpeed);
                printf(" BOTTOM: %"PRIu32, mVCurrent);
                printf(" POTENTIOMETR: %"PRIu16, potentiometer);
                printf("\r\n");
            }
        }
        // sleep_mode();
    }
}
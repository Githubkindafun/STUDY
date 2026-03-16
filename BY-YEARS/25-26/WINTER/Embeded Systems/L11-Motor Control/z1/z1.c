#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>

#define SPINNER PB1
#define SPINNER_DDR DDRB

void adc_init() {
    // napięcie referencyjne na Vcc
    ADMUX |= _BV(REFS0);
    // prescaler 128 i ADC ON
    ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
    // start
    ADCSRA |= _BV(ADSC);
}

uint16_t readADC() {
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
    ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
    uint16_t v = ADC; // weź zmierzoną wartość (0..1023)
    return v;
}

void timer1_init() {
    // clear on compare match - setup low level (z datasheet)
    TCCR1A |= _BV(COM1A1);
    // wybieramy fast PWM z top jako ICR1
    TCCR1A |= _BV(WGM11);
    TCCR1B |= _BV(WGM12) | _BV(WGM13);
    // prescaler 64 bo chcemy miec 500 Hz
    TCCR1B |= _BV(CS11) | _BV(CS10);
    // do wzorku dziala
    ICR1 = 499;
    // teraz dzieki temu ze ICR1 to top to
    // mozemy kozystac z OCR1A aby ustawiac duty
}

void setup() {
    adc_init();
    // dobra chcemy miec output ustawiony
    SPINNER_DDR |= _BV(SPINNER);
    timer1_init();
}

int main() {
    setup();
    while(1) {
        uint16_t adc = readADC();
        // ustawiamy duty w stosunku do ADC
        float conversion = adc * ( 500.0 / 1024.0 ); 
        OCR1A = (uint16_t)conversion;
        // na wszelki wypadek zeby cos sie nie zepsolo.
        _delay_ms(64);
    }
}
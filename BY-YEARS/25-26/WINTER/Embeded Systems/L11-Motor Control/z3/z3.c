#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>

#define SPINNER PB1
#define SPINNER_DDR DDRB

#define A_1_PIN PD4
#define A_1_DDR DDRD
#define A_1_PORT PORTD
#define A_2_PIN PD5
#define A_2_DDR DDRD
#define A_2_PORT PORTD

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
    // setup pinow do kontrolera
    A_1_DDR |= _BV(A_1_PIN);
    A_2_DDR |= _BV(A_2_PIN);
    A_1_PORT &= ~_BV(A_1_PIN);
    A_2_PORT &= ~_BV(A_2_PIN);
}

int main() {
    setup();
    while(1) {
        uint16_t adc = readADC();
        // teraz z tego co rozumiem musimy wybrac
        // czy lewo czy prawo
        // arbitralnie lewo to 0 - 511
        // prawo to 513 - 1024
        // 0 to 512

        if(adc < 512) {
            // 0 to max 511 to min
            int left = adc - 512;
            left *= -1;
            A_2_PORT &= ~_BV(A_2_PIN);
            A_1_PORT |= _BV(A_1_PIN);
            float conversion = (left * ( 500.0 / 1024.0 ));
            OCR1A = (uint16_t)conversion;
        } 
        else if (adc == 512) {
            OCR1A = 0;
            _delay_ms(1000);
        } 
        else {
            // 513 to min 1024 to max
            // ustawiamy duty w stosunku do ADC
            float conversion = (adc * ( 500.0 / 1024.0 )) / 2.0;
            A_1_PORT &= ~_BV(A_1_PIN);
            A_2_PORT |= _BV(A_2_PIN);
            OCR1A = (uint16_t)conversion;
        }
        
        // na wszelki wypadek zeby cos sie nie zepsolo.
        _delay_ms(64);
    }
}
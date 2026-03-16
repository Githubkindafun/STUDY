#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>

#define SPINNER PB1
#define SPINNER_DDR DDRB

// ogolnie to jest to mega dziwne
// ale kazda dokumentacja mowila inaczej
// wiec arbitralnie to wyznaczylem
#define min45 180
// #define min45 313
// #define min45 64
#define zero 375
// #define zero 94
#define max45 438
// #define max45 124

void adc_init() {
    // napięcie referencyjne na Vcc
    ADMUX |= _BV(REFS0);
    // prescaler 128 i ADC ON
    ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
    // start
    // ADCSRA |= _BV(ADSC);
}

uint16_t readADC() {
    ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
    uint16_t v = ADC; // weź zmierzoną wartość (0..1023)
    return v;
}

void timer1_init() {
    // clear on compare match - setup low level (z datasheet)
    TCCR1A |= _BV(COM1A1);
    // wybieramy fast PWM z top jako ICR1
    TCCR1A |= _BV(WGM11);
    TCCR1B |= _BV(WGM12) | _BV(WGM13);
    // prescaler 64 bo chcemy miec 50 Hz
    TCCR1B |= _BV(CS11) | _BV(CS10);
    // TCCR1B |= _BV(CS12);
    // do wzorku dziala 
    ICR1 = 4999;
    // ICR1 = 1249;
    // teraz dzieki temu ze ICR1 to top to
    // mozemy kozystac z OCR1A aby ustawiac duty
    OCR1A = zero;
}

// skoro mamy 4999 tickow i chcemy miec 
// 1ms to 90 stopni wiec 1.25 ms to 45
// 1.5ms to 0
// 2ms to "90" czyli 45 to 1.75ms
// 1ms to 250 tick
// 0.25ms to 63
// zaokraglam ale i tak biore 45 stopni wiec git

// disclaimer zmienilem rzeczy na inny prescaler i niby 180 stopni i teraz dziala
// wczesniej byly dziwne rzeczy + nie bylo 90 stopni tylko ustawione na 45
// a jak ustawiam na 180 calosc to robi 90 idk.


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
        // float conversion = adc * ( 439.0 / 1024.0 ); 
        // problem byl z castowaniem
        uint32_t conversion = min45 + (uint32_t)adc * (max45 - min45) / 1024;
        // int conversion = 94;
        // int conversion = 123;
        // int conversion = 63;
        OCR1A = (uint16_t)conversion;
        // na wszelki wypadek zeby cos sie nie zepsolo.
        _delay_ms(64);
    }
}
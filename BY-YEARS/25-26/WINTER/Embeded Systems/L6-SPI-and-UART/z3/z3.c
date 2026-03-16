#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>
#include "dzwiek.c"

#define CS PB2
#define MOSI PB3
#define SCK PB5

volatile uint16_t soundIdx = 0;

void send_the_sound(uint8_t sound) {
    // dobra 
    // zatem chcemy przeslac 16 bitowa data do ukladu DAC
    // wg data sheet'a DAC to wyglada tak
    // mamy 16 bitow (indeksujac od 0)
    // 0-3 : ignored dont care
    // 4-11: input data (to co przesylamy, nasz input)
    // 12 : Output Shutdown Control bit zasadniczo Vout on/off
    // 13 : GA wg. zadania bedziemy ustawiac na x1 wiec nie dotykam
    // 14 : BUF daje nam bodyguarda pilnujacego Vref
    // 15 : brak nazwy ale zasadniczo write to DAC (0) ? (1)
    // czyli teraz tak
    // 15 : 0
    // 14 : 1 - why not jak moze  pomoc
    // 13 : 1 - tresc zadania
    // 12 : 1 - no ma byc Vout wiec musi byc 1
    // 11-8 : data
    uint8_t high8 = 0b01110000 | (sound >> 4);

    // w DAC 4 ostatnie dont care, wiec bierzemy 4 dolne z sound
    uint8_t low8 = (sound & 0b00001111) << 4;

    // ustawienie CS na 0 spowoduje ze DAC zacznie zwracac uwage
    // na to co tam mu podeslemy
    PORTB &= ~(_BV(CS));
    // pakujemy gorna 8 i automatycznie odpala sie transfer SPI
    SPDR = high8;
    // czekamy az SPIF bedzie 1 to oznacza ze wszystko poszlo
    while(!(SPSR & _BV(SPIF))) {
        // sobie czeka
    }
    // wyczyść flagę przerwania
    SPSR |= _BV(SPIF);
    // analogicznie
    SPDR = low8;
    while(!(SPSR & _BV(SPIF))) {
        // sobie czeka
    }
    // wyczyść flagę przerwania
    SPSR |= _BV(SPIF);
    // juz moze przestac sluchac
    PORTB |= _BV(CS);
}

void spi_init() {
    // ustawiamy na output
    // MOSI, SCK,
    // MOSI - master out slave in
    // skoro Atmega bedzie masterem to musi ogarniac
    // mosi i sck
    DDRB |= _BV(MOSI);
    DDRB |= _BV(SCK);
    
    // CS sluzy do komtroli DAC'a
    // i to my go ogarniamy
    DDRB |= _BV(CS);
    

    // Bit 6 – SPE: SPI Enable
    // the SPI is enabled
    // Bit 4 – MSTR: Master/Slave Select
    SPCR = _BV(SPE) | _BV(MSTR);
    SPCR &= ~_BV(CPOL);
    SPCR &= ~_BV(CPHA);
}
void timer1_init() {
    // ustaiamy ctc
    TCCR1B |= _BV(WGM12);

    // odblokowujemy timer1 compare A interrupt
    TIMSK1 |= _BV(OCIE1A);

    // prescaler na 8
    TCCR1B |= _BV(CS11);

    // aby osiagnac 8kHz -> 2mln / 8k = 250 
    OCR1A = 249;
}

ISR(TIMER1_COMPA_vect) {
    uint8_t sound = pgm_read_byte(&dzwiek_raw[soundIdx]);
    send_the_sound(sound);
    
    soundIdx++;
    if(soundIdx >= dzwiek_raw_len) {
        soundIdx = 0;
    }
}

void setup() {
    timer1_init();
    spi_init();
    sei();
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();
}

int main() {
    setup();
    while(1) {
        sleep_mode();
    }
}
// Zbuduj następujący układ wykorzystujący wyświetlacz 7-segmentowy oraz układ sterownika LED TLC5916.
// Wyświetl na wyświetlaczu co sekundę kolejne cyfry od 0 do 9. Wykorzystaj transceiver SPI do sterowania
// układem sterownika LED. Sposób sterowania tym układem jest następujący:

// Po przesłaniu stanu diod przy użyciu SPI należy przesłać krótki impuls na pin LE. Stan diod LED
// jest ładowany z rejestru przesuwnego po zboczu opadającym.

// Stan wysoki pinu OE wyłącza świecenie diod LED.

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define LE PB1
#define OE PB2
#define MOSI PB3
#define SCK PB5

volatile uint8_t currentDigit = 0;

uint8_t ledTable[10] = {
    0b10111111,
    0b10000110,
    0b11011011,
    0b11001111,
    0b11100110,
    0b11101101,
    0b11111101,
    0b10000111,
    0b11111111,
    0b11101111
};

void show_digit(uint8_t digit) {
    // Writing to the register initiates data transmission
    // zasadniczo przesylamy do sterownika przez SPI
    // przez mosi co kazdy cykl sck bit po bicie sobie plynie
    SPDR = digit;
    while(!(SPSR & _BV(SPIF))) { 
        // czekamy az SPI skonczy
    }
    // wyczyść flagę przerwania
    SPSR |= _BV(SPIF);
    // puszczamy sygnal hihg -> low 
    // zeby sterownik zaktualizowal outputy
    // high -> wczytywane sa nowe
    // low -> zatrzaskuje nowe dane do nowego high
    PORTB |= _BV(LE);
    PORTB &= ~_BV(LE);
}
// timer 1 - basicly do printowania
void timer1_init() {
    // ustaiamy ctc
    TCCR1B |= _BV(WGM12);

    // odblokowujemy timer1 compare A interrupt
    TIMSK1 |= _BV(OCIE1A);

    // prescaler na 256
    TCCR1B |= _BV(CS12);

    OCR1A = 62499;
}

ISR(TIMER1_COMPA_vect) {
    currentDigit++;
    if(currentDigit > 9) {
        currentDigit = 0;
    }
    show_digit(ledTable[currentDigit]);
}

void spi_init() {
    // ustawiamy na output
    // MOSI, SCK, LE, OE
    // MOSI - master out slave in
    // skoro Atmega bedzie masterem to musi ogarniac
    // mosi i sck
    DDRB |= _BV(MOSI);
    DDRB |= _BV(SCK);
    
    // LE i OE sluza do komtroli sterownika 
    DDRB |= _BV(LE);
    DDRB |= _BV(OE);

    // ustawiamy low na OE i LE
    // OE low -> led moga swiecic
    // LE low -> siedzi na low i kiedy dostanie puls high->low
    // nadpisuje rejestr nowymi wartosciami ktore przyszly
    PORTB &= ~_BV(OE);
    PORTB &= ~_BV(LE);

    // Bit 6 – SPE: SPI Enable
    // the SPI is enabled
    // Bit 4 – MSTR: Master/Slave Select
    SPCR = _BV(SPE) | _BV(MSTR);
    
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
    show_digit(ledTable[currentDigit]);
    while(1) {
        sleep_mode();
    }
}


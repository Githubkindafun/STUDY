#include <avr/io.h>
#include <stdint.h>

#define BTN PB0
#define LED PB1

#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN_DDR DDRB
#define BTN_PIN PINB
#define BTN_PORT PORTB

void setup() {
    // btn na input + pullup
    BTN_DDR &= ~_BV(BTN);
    BTN_PORT |= _BV(BTN);
    // LED na output
    LED_DDR |= _BV(LED);
    LED_PORT &= ~_BV(LED);
    // spi slave: mosi oraz sck in, miso out
    DDRB &= ~(_BV(PB3) | _BV(PB5));
    DDRB |= _BV(PB4);
    // odpalamy spi
    SPCR = _BV(SPE);
}

// ze skosa
uint8_t spi_transfer(uint8_t data)
{
    // rozpocznij transmisję
    SPDR = data;
    // czekaj na ukończenie transmisji
    while (!(SPSR & _BV(SPIF)));
    // wyczyść flagę przerwania
    SPSR |= _BV(SPIF);
    // zwróć otrzymane dane
    return SPDR;
}

int main() {
    setup();
    uint8_t recived = 0;
    while(1) {
        uint8_t buttonState = !(BTN_PIN & _BV(BTN));
        uint8_t toSend = 0;
        if(buttonState) {
            toSend = 1;
        }
        uint8_t recived = spi_transfer(toSend);
        if(recived) {
            LED_PORT |= _BV(LED);
        } else {
            LED_PORT &= ~_BV(LED);
        }
    }
}

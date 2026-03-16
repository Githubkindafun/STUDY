#include <avr/io.h>
#include <util/delay.h>

#define LED PB4
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PB3
#define BTN_PIN PINB
#define BTN_PORT PORTB

uint8_t CYCLIC_BUFFER[100] = {0};


int main() {
  BTN_PORT |= _BV(BTN);
  LED_DDR |= _BV(LED);

  uint8_t i = 0;

  while (1) {
    uint8_t buttonState = (BTN_PIN & _BV(BTN));
    CYCLIC_BUFFER[i] = !buttonState;
    if (CYCLIC_BUFFER[(i + 1) % 100]){
        LED_PORT |= _BV(LED);
    } else {
        LED_PORT &= ~_BV(LED);
    }
    i++;
    i = i % 100;
    _delay_ms(10); // bo chcemy 1/100
  }
}

#include <avr/io.h>
#include <util/delay.h>

#define LED0 PD0
#define LED1 PD1
#define LED2 PD2
#define LED3 PD3
#define LED4 PD4
#define LED5 PD5
#define LED6 PD6
#define LED7 PD7

#define LED_DDR DDRD
#define LED_PORT PORTD

#define DELAY 100

int ledTable[8] = {
    LED0,
    LED1,
    LED2,
    LED3,
    LED4,
    LED5,
    LED6,
    LED7
};

void ledOn(int led) {
    LED_PORT |= _BV(led);
}

void ledOff(int led) {
    LED_PORT &= ~_BV(led);
}

int main() {
    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
    // for(int i = 0; i < 8; i++) {
    //     LED_DDR |= _BV(ledTable[i]);
    // }

    LED_DDR = 0b11111111;

    while(1) {
        for(int i = 0; i < 8; i++) {
            if(i > 0) {
                ledOff(ledTable[i - 1]);
            }
            ledOn(ledTable[i]);
            _delay_ms(DELAY);
        }
        ledOff(ledTable[7]);
        for(int i = 7; i >= 0; i--) {
            if(i < 7) {
                ledOff(ledTable[i + 1]);
            }
            ledOn(ledTable[i]);
            _delay_ms(DELAY);
        }
        ledOff(ledTable[0]);
    }
    return 0;
}
#include <avr/io.h>
#include <util/delay.h>


#define LED_DDR DDRD
#define LED_PORT PORTD

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

int main() {
    LED_DDR = 0xFF;

    while(1) {
        for(int i = 0; i < 10; i++) {
            LED_PORT = ~ledTable[i];
            _delay_ms(1000);
        }
    }
    return 0;
}

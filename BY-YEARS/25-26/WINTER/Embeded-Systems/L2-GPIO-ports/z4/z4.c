#include <avr/io.h>
#include <util/delay.h>

#define LED_DDR DDRD
#define LED_PORT PORTD

#define D1 PC0
#define D2 PC1
#define TDDR DDRC
#define TPORT PORTC

void onD1() {
    TPORT &= ~_BV(D1);
}

void offD1() {
    TPORT |= _BV(D1);
}

void onD2() {
    TPORT &= ~_BV(D2);
}

void offD2() {
    TPORT |= _BV(D2);
}

uint8_t ledTable[10] = {
    0b01000000,
    0b01111001,
    0b00100100,
    0b00110000,
    0b00011001,
    0b00010010,
    0b00000010,
    0b01111000,
    0b00000000,
    0b00010000
};

int main() {
    LED_DDR = 0xFF;

    TDDR |= _BV(D1);
    TDDR |= _BV(D2);
    
    int currentNumber = 0;
    while(1) {
        int digit1 = (currentNumber / 10) % 10;
        int digit2 = currentNumber % 10; 

        for(int i = 0; i < 100; i++) {
            LED_PORT = ledTable[digit1];
            onD1();
            _delay_ms(5);
            offD1();
            LED_PORT = ledTable[digit2];
            onD2();
            _delay_ms(5);
            offD2();
        }
        currentNumber++;
        if (currentNumber == 60) {
            currentNumber = 0;
        }
        
    }
    return 0;
}

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#define LED_DDR DDRD
#define LED_PORT PORTD

#define BTN_DDR DDRB
#define BTN_PORT PORTB
#define BTN_PIN PINB
#define BTN_NEXT PB0
#define BTN_PREV PB1
#define BTN_RESET PB2

#define GREY_MAX 255
// https://www.geeksforgeeks.org/dsa/gray-to-binary-and-binary-to-gray-conversion/
uint8_t binaryToGray(uint8_t input) {
    uint8_t shift = (uint8_t)(input >> 1);
    return (uint8_t)(input ^ shift); // xor  
}

void ledNumberToShow(uint8_t input) {
    LED_PORT = input;
}

uint8_t wasButtonPressed(uint8_t button) {
    if((BTN_PIN & _BV(button)) == 0) {
        _delay_us(2000);
        if((BTN_PIN & _BV(button)) == 0) {
            return 1;
        }
    }

    return 0;
}

int main() {
    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
    LED_DDR = 0xFF;
    LED_PORT = 0;

    BTN_DDR &= ~_BV(BTN_NEXT);
    BTN_DDR &= ~_BV(BTN_PREV);
    BTN_DDR &= ~_BV(BTN_RESET);

    BTN_PORT |= _BV(BTN_NEXT);
    BTN_PORT |= _BV(BTN_PREV);
    BTN_PORT |= _BV(BTN_RESET);

    uint8_t numberToShow = 0;
    uint8_t btnNextStatus = 0;
    uint8_t btnPrevStatus = 0;
    uint8_t btnResetStatus = 0;
    ledNumberToShow(binaryToGray(numberToShow));

    while (1) {
        if(wasButtonPressed(BTN_NEXT)) {
            if(!btnNextStatus) {
                btnNextStatus = 1;
                numberToShow++;
                if(numberToShow > GREY_MAX) {
                    numberToShow = 0;
                }
                ledNumberToShow(binaryToGray(numberToShow));
            }
        } else {
            btnNextStatus = 0;
        }
        if(wasButtonPressed(BTN_PREV)) {
            if(!btnPrevStatus) {
                btnPrevStatus = 1;
                numberToShow--;
                ledNumberToShow(binaryToGray(numberToShow));
            }
        } else {
            btnPrevStatus = 0;
        }
        if(wasButtonPressed(BTN_RESET)) {
            if(!btnResetStatus) {
                btnResetStatus = 1;
                numberToShow = 0;
                ledNumberToShow(binaryToGray(numberToShow));
            }
        } else {
            btnResetStatus = 0;
        }
    }
    
}
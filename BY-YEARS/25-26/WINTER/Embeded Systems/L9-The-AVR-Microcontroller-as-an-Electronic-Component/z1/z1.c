
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>

#define LED PB2
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PB0
#define BTN_PIN PINB
#define BTN_PORT PORTB

volatile uint8_t CYCLIC_BUFFER[100] = {0};
volatile uint8_t currentIndex = 0;

void setup() {
    // przycisk oraz led
    BTN_PORT |= _BV(BTN); // pull-up 
    LED_DDR |= _BV(LED); // ustawiamy pin na output
    
    // timer setup
    // clear timer on compare match
    TCCR1B |= _BV(WGM12); // CTC przyda sie do interrupt

    // output compare A match interrupt enable 
    // zasadniczo "odblokowujemy" interrupty gdy Timer1 zmachuje OCR1A
    TIMSK1 |= _BV(OCIE1A);

    // preskaler 8
    TCCR1B |= _BV(CS11);

    // 16000000 ÷ ( 64 ( 1 + 2499 ) = 100Hz
    // na mini jest 8Mhz nie 16Mhz
    // ale defaultowo jest z preskalerem / 8 wiec mamy 1Mhz
    // stad mala poprawka co do wartosci OCR1A i prescalera


    // bo chcemy miec podzial na 1/100 sekundy
    OCR1A = 1249;

    // odblokowujemy interrupty
    sei();
    // ustawiamy sleep mode na IDLE - cpu spi ale timery i interrupty dzialaja 
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();
}

ISR(TIM1_COMPA_vect) {
    uint8_t buttonState = !(BTN_PIN & _BV(BTN));
    uint8_t prevValue = CYCLIC_BUFFER[currentIndex];

    if (prevValue){
        LED_PORT |= _BV(LED);
    } else {
        LED_PORT &= ~_BV(LED);
    }

    CYCLIC_BUFFER[currentIndex] = buttonState;
    currentIndex++;
    if(currentIndex >= 100) {
      currentIndex = 0;
    }
}

int main() {
  setup();
  
  while (1) {
    // cpu sb spi tylko sie budzi realizowac przerwania
    sleep_mode();
  }
}


#include <avr/io.h>
#include <stdio.h>

#define LED PB2
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PA7
#define BTN_PIN PINA
#define BTN_PORT PORTA

#define SCK PA4
#define DO PA5
#define DI PA6

// to z skosa
uint8_t spi_transfer(uint8_t data)
{
    // załaduj dane do przesłania
    USIDR = data;
    // wyczyść flagę przerwania USI
    USISR = _BV(USIOIF);
    // póki transmisja nie została ukończona, wysyłaj impulsy zegara
    while (!(USISR & _BV(USIOIF))) {
        // wygeneruj pojedyncze zbocze zegarowe
        // zostanie wykonane 16 razy
        USICR = _BV(USIWM0) | _BV(USICS1) | _BV(USICLK) | _BV(USITC);
    }
    // zwróć otrzymane dane
    return USIDR;
}

void setup() {

    // pull-up
    BTN_PORT |= _BV(BTN);
    // ustawiamy pin na output
    LED_DDR |= _BV(LED);

    // ustaw piny MOSI i SCK jako wyjścia
    DDRA = _BV(SCK) | _BV(DO);

    // musimy jescze dodac MISO na input
    DDRA &= ~_BV(DI);

    // ustaw USI w trybie trzyprzewodowym (SPI)
    USICR = _BV(USIWM0);
}



int main() {
  setup();
  while(1) {
    uint8_t buttonState = !(BTN_PIN & _BV(BTN));
    uint8_t toSend = 0;
    if(buttonState) {
        toSend = 1;
    }
    uint8_t recived = spi_transfer(toSend);
    if (recived){
        LED_PORT |= _BV(LED);
    } else {
        LED_PORT &= ~_BV(LED);
    }
  }
  
}

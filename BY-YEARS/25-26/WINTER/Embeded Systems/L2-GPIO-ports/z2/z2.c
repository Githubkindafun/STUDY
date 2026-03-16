#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

// inicjalizacja UART
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // wyczyść rejestr UCSR0A
  UCSR0A = 0;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

// transmisja jednego znaku
int uart_transmit(char data, FILE *stream)
{
  // czekaj aż transmiter gotowy
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}

FILE uart_file;


#define LED_DOT PB3
#define LED_DASH PB2
#define LED_SPACE PB1
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PB4
#define BTN_PIN PINB
#define BTN_PORT PORTB

uint8_t morseLength = 0;
uint8_t morseBits = 0;
uint8_t morseTooLong = 0;

char morseTable1[2] = {
    'e',
    't'
};
char morseTable2[4] = {
    'i',
    'a',
    'n',
    'm'
};
char morseTable3[8] = {
    's',
    'u',
    'r',
    'w',
    'd',
    'k',
    'g',
    'o'
};
char morseTable4[16] = {
    'h',
    'v',
    'f',
    0,
    'l',
    0,
    'p',
    'j',
    'b',
    'x',
    'c',
    'y',
    'z',
    'q',
    0,
    0
};

char morseTable(uint8_t morseLength, uint8_t morseBits) {
    switch(morseLength) {
        case 1:
            return morseTable1[morseBits];
        case 2:
            return morseTable2[morseBits];
        case 3:
            return morseTable3[morseBits];
        case 4:
            return morseTable4[morseBits];
        default: return 0;
    }
}

void addDotOrDash(uint8_t isDot) {
    if(morseLength < 4) {
        morseBits = (uint8_t)(morseBits << 1); // dodajemy dot (0)
        if(!isDot) {
            morseBits = (uint8_t)(morseBits | 1); // dodajemy dash (1)
        }
    } else {
        morseTooLong = 1;
    }
    morseLength++;
}

void outputLetter() {
    if(morseLength == 0) {
        return;
    }
    if(morseLength >= 1 && morseLength <= 4) {
        char output = morseTable(morseLength, morseBits);
        putchar(output);
    } else {
        putchar('?');
    }
    morseLength = 0;
    morseBits = 0;
}
int main() {
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;

  /////////////////////////////////////////

  LED_DDR |= _BV(LED_DOT);
  LED_DDR |= _BV(LED_DASH);
  LED_DDR |= _BV(LED_SPACE);
  BTN_PORT |= _BV(BTN);

  uint16_t timePressed = 0;
  uint16_t timeRelesed = 0;
  uint8_t spaceOut = 0;
  while(1) {
    uint8_t buttonState = !(BTN_PIN & _BV(BTN));

    if(buttonState) {
        if(timePressed == 0) {
            timeRelesed = 0;
            spaceOut = 0;
            LED_PORT &= ~_BV(LED_DOT);
            LED_PORT &= ~_BV(LED_DASH);
            LED_PORT &= ~_BV(LED_SPACE);
        }
        timePressed++;
        if(timePressed >= 100) {
            LED_PORT |= _BV(LED_DOT);
        }
        if(timePressed >= 300) {
            LED_PORT |= _BV(LED_DASH);
        }
        LED_PORT &= ~_BV(LED_SPACE);
    } else {
        if(timePressed > 0) {
            if(timePressed >= 300) {
                addDotOrDash(0);
            } else if(timePressed >= 100) {
                addDotOrDash(1);
            }
            timePressed = 0;

            LED_PORT &= ~_BV(LED_DOT);
            LED_PORT &= ~_BV(LED_DASH);
            LED_PORT &= ~_BV(LED_SPACE);
            timeRelesed = 0;
        }
        if(timeRelesed < UINT16_MAX) {
            timeRelesed++;
        }
        if(timeRelesed >= 300) {
            outputLetter();
        }
        if(timeRelesed >= 700) {
            LED_PORT &= ~_BV(LED_DOT);
            LED_PORT &= ~_BV(LED_DASH);
            LED_PORT |= _BV(LED_SPACE);
            if(!spaceOut) {
                putchar(' ');
                spaceOut = 1;
            } 
        } else {
            LED_PORT &= ~_BV(LED_DOT);
            LED_PORT &= ~_BV(LED_DASH);
            LED_PORT &= ~_BV(LED_SPACE);
        }

    }
    _delay_ms(10);
  }
  
}
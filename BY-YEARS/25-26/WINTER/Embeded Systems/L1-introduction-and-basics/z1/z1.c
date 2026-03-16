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
// -----------------
#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB
#define dot 0
#define dash 1

void Blink(int flag) {
  if(flag != 1) {
    LED_PORT |= _BV(LED);
    _delay_ms(200);
    LED_PORT &= ~_BV(LED);
    _delay_ms(200);
  } else {
    LED_PORT |= _BV(LED);
    _delay_ms(600);
    LED_PORT &= ~_BV(LED);
    _delay_ms(600);
  }
  
}

// https://stackoverflow.com/questions/67448719/is-there-a-way-to-implement-morse-code-inside-an-arraysss
char *lettersInMorse[26] = {
        ".-",   // A
        "-...", // B
        "-.-.", // C
        "-..",  // D
        ".",    // E
        "..-.", // F
        "--.",  // G
        "....", // H
        "..",   // I
        ".---", // J
        "-.-",  // K
        ".-..", // L
        "--",   // M
        "-.",   // N
        "---",  // O
        ".--.", // P
        "--.-", // Q
        ".-.",  // R
        "...",  // S
        "-",    // T
        "..-",  // U
        "...-", // V
        ".--",  // W
        "-..-", // X
        "-.--", // Y
        "--.."  // Z
};


void toMorseBlinks(int input) {
    if(input < 0 || input > 25) {
        return;
    }
    char *morsePattern = lettersInMorse[input];
    for(int i = 0; morsePattern[i] != '\0'; i++) {
        if(morsePattern[i] == '.') {
            Blink(dot);
        } else if(morsePattern[i] == '-') {
            Blink(dash);
        }
    }
    _delay_ms(1000);
}


int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;

  
  LED_DDR = 0b00100000;
  printf("Input the letters for the translation one by one c: \r\n");

  char inputChar;
  while(1) {
    scanf(" %c", &inputChar);
    printf("%c", inputChar);
    toMorseBlinks(inputChar - 'a');
  }
}


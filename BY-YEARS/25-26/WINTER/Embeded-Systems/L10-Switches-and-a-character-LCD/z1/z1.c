#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#include "hd44780.c"
#include <string.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

// inicjalizacja UART
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
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

int hd44780_transmit(char data, FILE *stream)
{
  LCD_WriteData(data);
  return 0;
}

FILE hd44780_file;

int main() {
    uart_init();
    LCD_Initialize();
    LCD_Clear();
    fdev_setup_stream(&hd44780_file, hd44780_transmit, NULL, _FDEV_SETUP_WRITE);
    stdout = stderr = &hd44780_file;
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = &uart_file;
    // Cursor on , Cursor Blink
    LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_ON | HD44780_CURSOR_BLINK);
    uint8_t counter = 0;
    char line[41] = {};
    // LCD_Clear();
    LCD_GoTo(0, 1);
    while(1) {
        char ch = getchar();
        switch(ch) {
            case '\r':
                break;
            case '\n':
                // po kliknieciu enter zasadniczo czyscimy ekran 
                // nastepnie przechodzimy do linjki wyzej wypisujemy
                // poprzednio aktualna linijke
                // wracamy do linjki nizej zerujac kontener na linijke
                LCD_Clear();
                LCD_WriteCommand(HD44780_ENTRY_MODE | HD44780_EM_SHIFT_CURSOR | HD44780_EM_INCREMENT);
                LCD_GoTo(0, 0);
                fputs(line, stdout);
                LCD_GoTo(0, 1);
                counter = 0;
                // https://cpp0x.pl/dokumentacja/standard-C/memset/248
                // czyscimy line
                memset(line, 0, sizeof line);
                continue;
            default:
                line[counter] = ch;
                putchar(ch);
                break;
        }
        if(counter == 14) {
            // jak dojdziemy do granicy widzialnego okna to odpalamy shift
            LCD_WriteCommand(HD44780_ENTRY_MODE | HD44780_EM_SHIFT_DISPLAY | HD44780_EM_INCREMENT);
        }
        if(counter == 40) {
            // jak dojdziemy do granicy linjiki no to przechodzimy na cursor
            // i robimy analogiczne dzialanie jak enter
            LCD_Clear();
            LCD_WriteCommand(HD44780_ENTRY_MODE | HD44780_EM_SHIFT_CURSOR | HD44780_EM_INCREMENT);
            LCD_GoTo(0, 0);
            fputs(line, stdout);
            LCD_GoTo(0, 1);
            counter = 0;
            memset(line, 0, sizeof line);
        } else {
          counter++;
        }
    }
}
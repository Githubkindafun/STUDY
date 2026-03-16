
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#include "hd44780.c"
#include <string.h>

int hd44780_transmit(char data, FILE *stream)
{
  LCD_WriteData(data);
  return 0;
}

FILE hd44780_file;

#define zero 0b00000000
#define one 0b00010000
#define two 0b00011000
#define three 0b00011100
#define four 0b00011110
#define five 0b00011111

// stany paska progresu w kazdej komorce
// 0/5, 1/5, 2/5, ..., 5/5 
uint8_t cellStates[5][8] = {
    // {zero, zero, zero, zero, zero, zero, zero, zero},
    {one, one, one, one, one, one, one, one},
    {two, two, two, two, two, two, two, two},
    {three, three, three, three, three, three, three, three},
    {four, four, four, four, four, four, four, four},
    {five, five, five, five, five, five, five, five}
};

void createChar(uint8_t slot, uint8_t customChar[8]) {
    LCD_WriteCommand(HD44780_CGRAM_SET | slot << 3);
    for(int i = 0; i < 8; i++) {
        LCD_WriteData(customChar[i]);
    }
}

void loadCustomChar() {
    for(int i = 0; i < 5; i++) {
        createChar(i, cellStates[i]);
    }
    // wazne jest zeby wrocic do DDRAM aby cokolwiek nastepnego 
    // pojawialo sie na ekranie
    LCD_WriteCommand(HD44780_DDRAM_SET);
}

int main() {
    LCD_Initialize();
    LCD_Clear();
    loadCustomChar();
    fdev_setup_stream(&hd44780_file, hd44780_transmit, NULL, _FDEV_SETUP_WRITE);
    stdout = stderr = &hd44780_file;
    
    uint8_t counter = 0;
    uint8_t cellCounter = 0;
    // LCD_GoTo(0, 0);
    while(1) {
        if(counter == 81) {
            counter = 0;
            cellCounter = 0;
            LCD_Clear();
        }
        LCD_GoTo(0, 1);
        printf("%"PRId8, counter);
        printf("/%"PRId8, 80);
        _delay_ms(500);
        LCD_GoTo(cellCounter, 0);
        uint8_t cellProgress = counter % 5;
        LCD_WriteData(cellProgress);
        counter++;
        if(counter % 5 == 0) {
            cellCounter++;    
        }
        // cellCounter = (cellCounter ) % 16;
        // _delay_ms(1000);
        // LCD_GoTo(0, 1);
        // printf("%"PRId8, counter);
        // _delay_ms(1000); 
    }
    // LCD_WriteData(1);
}
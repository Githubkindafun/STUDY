#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include "notes.h" // plik z ksiazki zawierajacy wavelength dla konkretnych nut

#define BUZZ PB5
#define BUZZ_DDR DDRB
#define BUZZ_PORT PORTB

// funkcja z ksiazki
void pN(uint16_t wavelength, uint32_t duration) {
    uint32_t elapsed = 0;
    uint16_t i;
    for(elapsed = 0; elapsed < duration; elapsed += wavelength) {
        for(i = 0; i < wavelength; i++) {
            _delay_us(1);
        }
        BUZZ_PORT ^= (1 << BUZZ);
    }
}

// #define convert(note) ((uint16_t)( 500000 / (uint32_t)note))

// https://mixbutton.com/music-tools/frequency-and-pitch/music-note-to-frequency-chart
// #define E3 convert(165)
// #define G3 convert(196)
// #define Gs3 convert(208)
// #define A3 convert(220)
// #define B3 convert(247)

// #define C4 convert(262)
// #define Cs4 convert(277)
// #define D4 convert(294)
// #define Ds4 convert(311)
// #define E4 convert(330)
// #define F4 convert(349)
// #define Fs4 convert(370)
// #define G4 convert(392)
// #define Gs4 convert(415)
// #define A4 convert(440)
// #define As4 convert(466)
// #define B4 convert(494)

// #define C5 convert(523)
// #define Cs5 convert(554)
// #define D5 convert(587)
// #define Ds5 convert(622)
// #define E5 convert(659)
// #define F5 convert(698)
// #define Fs5 convert(740)
// #define G5 convert(784)
// #define Gs5 convert(831)
// #define A5 convert(880)
// #define As5 convert(932)
// #define B5 convert(988)


#define tempoScale 2
#define FullNote 0xF000UL * tempoScale
#define HalfNote (FullNote / 2UL)
#define QuarterNote (FullNote / 4UL)
#define Pause 167

// https://pianoletternotes.blogspot.com/2017/10/tetris-theme-by-korobeiniki.html

void pauseBreake(uint8_t amount) {
    for(int i = 0; i < amount; i++) {
        _delay_ms(Pause);
    }
}

typedef struct {
    uint16_t note;
    uint32_t noteTime;
    uint8_t pauseAmount;
}Sound;

const Sound song[] PROGMEM = {
    {E5, FullNote, 3},
    {B4, FullNote, 1},
    {C5, FullNote, 1},
    {D5, FullNote, 1},
    {E5, FullNote, 0},
    {D5, FullNote, 0},
    {C5, FullNote, 1},
    {B4, FullNote, 1},
    {A4, FullNote, 3},
    {A4, FullNote, 1},
    {C5, FullNote, 1},
    {E5, FullNote, 3},
    {D5, FullNote, 1},
    {C5, FullNote, 1},
    {B4, FullNote, 3},
    {B4, FullNote, 0},
    {B4, FullNote, 0},
    {C5, FullNote, 1},
    {D5, FullNote, 3},
    {E5, FullNote, 3},
    {C5, FullNote, 3},
    {A4, FullNote, 3},
    {A4, FullNote, 7},
    {D5, FullNote, 3},
    {D5, FullNote, 1},
    {F5, FullNote, 1},
    {A5, FullNote, 3},
    {G5, FullNote, 1},
    {F5, FullNote, 1},
    {E5, FullNote, 3},
    {E5, FullNote, 1},
    {C5, FullNote, 1},
    {E5, FullNote, 3},
    {D5, FullNote, 1},
    {C5, FullNote, 1},
    {B4, FullNote, 3},
    {B4, FullNote, 0},
    {B4, FullNote, 0},
    {C5, FullNote, 1},
    {D5, FullNote, 3},
    {E5, FullNote, 3},
    {C5, FullNote, 3},
    {A4, FullNote, 3},
    {A4, FullNote, 7},
    {E5, FullNote, 3},
    {B4, FullNote, 1},
    {C5, FullNote, 1},
    {D5, FullNote, 1},
    {E5, FullNote, 0},
    {D5, FullNote, 0},
    {C5, FullNote, 1},
    {B4, FullNote, 1},
    {A4, FullNote, 3},
    {A4, FullNote, 1},
    {C5, FullNote, 1},
    {E5, FullNote, 3},
    {D5, FullNote, 1},
    {C5, FullNote, 1},
    {B4, FullNote, 3},
    {B4, FullNote, 1},
    {C5, FullNote, 1},
    {D5, FullNote, 3},
    {E5, FullNote, 3},
    {A4, FullNote, 3},
    {A4, FullNote, 7},
    {D5, FullNote, 3},
    {D5, FullNote, 1},
    {F5, FullNote, 1},
    {A5, FullNote, 3},
    {G5, FullNote, 1},
    {F5, FullNote, 1},
};

// pgm_read_word(&song[i].note) 16 bitów
// pgm_read_dword(&song[i].noteTime); 32 bity
// pgm_read_byte(&song[i].pauseAmount); 8 bitóœ

void playMelody() {
    uint16_t songLength = sizeof(song) / sizeof(song[0]);
    for(int i = 0; i < songLength; i++) {
        uint16_t songNote = pgm_read_word(&song[i].note);
        uint32_t songNoteTime = pgm_read_dword(&song[i].noteTime);
        uint8_t songPause = pgm_read_byte(&song[i].pauseAmount);
        pN(songNote, songNoteTime);
        if(songPause > 0) {
            pauseBreake(songPause);
        }
    }
}

int main() {
    BUZZ_DDR |= (1 << BUZZ);
    BUZZ_PORT &= ~(1 << BUZZ);

    while(1) {
        playMelody();
        _delay_ms(2000);
    }
}
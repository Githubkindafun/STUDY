#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "pid.c"

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

#define SPINNER PB1
#define SPINNER_DDR DDRB

void adc_init() {
    // napięcie referencyjne na Vcc
    ADMUX = _BV(REFS0);
    // prescaler 128 i ADC ON
    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
    // wlaczamy interrupty
    ADCSRA |= _BV(ADIE);
    // Bit 5:0 – ADC5D...ADC0D: ADC5...0 Digital Input Disable (datasheet)
    DIDR0 |= _BV(ADC1D) | _BV(ADC0D);
}


void timer1_init() {
    // clear on compare match - setup low level (z datasheet)
    TCCR1A = _BV(COM1A1);
    // wybieramy PWM phase and frequency correct z top jako ICR1
    TCCR1B |=  _BV(WGM13);
    // prescaler 64 bo chcemy miec 500 Hz
    TCCR1B |= _BV(CS11) | _BV(CS10);
    TCCR1B |= _BV(ICNC1);
    // do wzorku dziala
    ICR1 = 250;
    // a bo czemu nie na 0
    OCR1A = 0;
    // czyscimy na wszelki flagi
    TIFR1 = _BV(TOV1) | _BV(ICF1);
    // ICIE1: Timer/Counter1, Input Capture Interrupt Enable (z datasheet)
    TIMSK1 |= _BV(ICIE1);
    // TOIE1: Timer/Counter1, Overflow Interrupt Enable (z datasheet)
    // TIMSK1 |= _BV(TOIE1);
}

// volatile uint16_t adcCurrentRead = 0;
volatile uint16_t adcSpeedRead = 0;
volatile uint16_t adcPotentiometerRead = 0;
volatile uint8_t adcNewRead = 0;
volatile uint8_t adcPhase = 0;
volatile uint8_t safety = 0;
// gdzie 0 to nic 1 to ovf, 2 to potencjometr, 3 capt

uint8_t volatile speedReadCounter = 0;
uint16_t volatile speedReadSum = 0;
uint8_t volatile speedReadAvgDone = 0;


ISR(TIMER1_CAPT_vect) {
    // odpala sie jak dojdziemy w tym trybie do top
    if(adcPhase != 0) {
        return;
    }
    if(ADCSRA & _BV(ADSC)) {
        return;
    }
    adcPhase = 1;
    // na A0 mamy pomiar wiec na A0 przelaczamy
    ADMUX = (ADMUX & 0xF0) | 0;
    // safety = 1;
    // adc start
    ADCSRA |= _BV(ADSC);
    // z data sheeta :
    // the ICF1 Flag is set when the counter reaches the TOP value.
    // na wszelki czyscimy
    TIFR1 = _BV(ICF1);
}

ISR(ADC_vect) {
    uint16_t adc = ADC;
    if(safety) {
        safety = 0;
        ADCSRA |= _BV(ADSC);
        return;
    }
    // mamy 2 typy pomiarow z capt/potencjometr
    if(adcPhase == 1) {

        speedReadSum += adc;
        speedReadCounter++;
        
        if(speedReadCounter >= 16) {
            adcSpeedRead = (uint16_t)(speedReadSum / 16);
            speedReadCounter = 0;
            speedReadSum = 0;
            speedReadAvgDone = 1;
        }

        // adcSpeedRead = adc;
        // teraz potencjometr
        adcPhase = 2;
        // przechodzimy na A1
        ADMUX = (ADMUX & 0xF0) | 1;
        // no i niech zacznie tam sobie liczyc
        ADCSRA |= _BV(ADSC);
        // skoro zmienilismy kanal to na wszelki wyrzuacmy jedno
        // jakby jakies smieci mialy byc
        safety = 1;
    } else if(adcPhase == 2) {
        adcPotentiometerRead = adc;
        adcPhase = 0;
        if(speedReadAvgDone) {
            speedReadAvgDone = 0;
            adcNewRead = 1;
        }
    } else {
        adcPhase = 0;
    }
}


#define K_P 0.5
#define K_I 0.05
#define K_D 0.01

pidData_t pidData;

int16_t Get_Reference(void)
{
    return (int16_t)adcPotentiometerRead;
}

void Set_Input(int16_t inputValue)
{
    uint16_t cap = ICR1 - 20;
    uint16_t ocr = (uint32_t)inputValue * cap / (uint32_t)255;
    if(ocr > ICR1) {
        ocr = ICR1;
    }
    OCR1A = ocr;
}

void pid_init() {
    pid_Init(K_P * SCALING_FACTOR, K_I * SCALING_FACTOR, K_D * SCALING_FACTOR, &pidData);
}



void setup() {
    adc_init();
    // dobra chcemy miec output ustawiony
    SPINNER_DDR |= _BV(SPINNER);
    timer1_init();
    uart_init();
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
    sei();
    pid_init();
    // set_sleep_mode(SLEEP_MODE_IDLE);
    // sleep_enable();
}

 
int main() {
    setup();
    uint16_t loopCounter = 0;
    int16_t lastOutputUsed = 0;
    while(1) {
        if(adcNewRead) {
            uint16_t speed = 0;
            uint16_t potentiometer = 0;
            cli();
            // sekcja krytyczna aby nikt nam nic nie zmienil
            adcNewRead = 0;
            // current = adcCurrentRead;
            speed = adcSpeedRead;
            potentiometer = adcPotentiometerRead;
            sei();
            // na podstawie wartosci potencjometru zmieniamy duty
            // co zmieni predkosc silniczka
            
            int16_t setPoint = potentiometer;

            int16_t processVariable = (int16_t)(1023 - speed);

            int32_t sumBefore = pidData.sumError;
            int16_t pidRaw = pid_Controller(setPoint, processVariable, &pidData);

            uint8_t glitched = 0;
            if(pidRaw == MAX_INT || pidRaw == INT16_MIN) {
                glitched = 1;
            }
            int16_t pidUsed = pidRaw;
            if(pidUsed > 255) {
                pidUsed = 255;
            }
            if(pidUsed < 0) {
                pidUsed = 0;
            }
            if(glitched) {
                pidData.sumError = sumBefore;
                pidUsed = lastOutputUsed;
            } else {
                lastOutputUsed = pidUsed;
            }
            Set_Input(pidUsed);

            loopCounter++;

            if(loopCounter >= 12) {

                // uint32_t mVCurrent = (uint32_t)5000 - (uint32_t)(speed * (uint32_t)5000) / (uint32_t)1023;
                // uint32_t mVCurrent = 5000 - ((uint32_t)current * (uint32_t)5000 / (uint32_t)1023);
                // uint32_t mVSpeed = (uint32_t)(current * (uint32_t)5000) / (uint32_t)1023;
                uint32_t mVSpeed = 5000 - ((uint32_t)speed * (uint32_t)5000 / (uint32_t)1023);

                loopCounter = 0;
                printf("SP: %"PRId16, setPoint);
                printf(" PV: %"PRId16, processVariable);
                printf(" TOP: %"PRIu32, mVSpeed);
                // printf(" BOTTOM: %"PRIu32, mVCurrent);
                printf(" USED: %"PRIu16, pidUsed);
                printf("\r\n");
            }
        }
        // sleep_mode();
    }
}
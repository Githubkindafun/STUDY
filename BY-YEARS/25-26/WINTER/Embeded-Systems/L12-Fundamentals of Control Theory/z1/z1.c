#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/interrupt.h>
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

// nasza mala grzalka setup
#define HEATER_RESISTOR_PIN PB5
#define HEATER_RESISTOR_DDR DDRB
#define HEATER_RESISTOR_PORT PORTB

void heater_resistor_init() {
	// PB5 jako wyjscie
	HEATER_RESISTOR_DDR |= _BV(HEATER_RESISTOR_PIN);
	// grzanie off
	HEATER_RESISTOR_PORT &= ~_BV(HEATER_RESISTOR_PIN);
}

void heatOn() {
	HEATER_RESISTOR_PORT |= _BV(HEATER_RESISTOR_PIN);
}

void heatOff() {
	HEATER_RESISTOR_PORT &= ~_BV(HEATER_RESISTOR_PIN);
}

// adc setup
#define Th 1

void adc_init() {
    // napięcie referencyjne 1.1 V
    ADMUX |= _BV(REFS1) | _BV(REFS0);
    // prescaler 128 i ADC ON
    ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
    // start
    // ADCSRA |= _BV(ADSC);
}

uint16_t readADC() {
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
    ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
    uint16_t v = ADC; // weź zmierzoną wartość (0..1023)
    return v;
}

uint16_t readADC_avg() {
    int32_t accumulator = 0;
    for(uint8_t i = 0; i < 16; i++) {
        accumulator += readADC();
    }
    return (uint16_t)(accumulator / 16);
}


volatile uint16_t heaterOnTicks;

uint16_t readADC_avgNoiseReducted() {
    // zasadniczo po to aby dokonac pomiaru
    // podczas wylaczonej grzalki
    uint16_t ticksOn;
    cli();
    ticksOn = heaterOnTicks;
    heaterOnTicks = 0;
    sei();

    heatOff();
    _delay_ms(5);
    uint16_t adc = readADC_avg();
    
    cli();
    heaterOnTicks = ticksOn;
    sei();
    return adc;
}


// wzorek z datasheeta
// po przeksztalceniach dostajemy
// T_a = (V_out - V_0) / T_c
// stale tez sa w datasheet
uint8_t celciousFromAdc(int16_t adc) {
	int32_t Vout = (int32_t)adc * 1100UL / 1024UL;
	if(Vout <= 500) {
		return 0;
	}
	int32_t Ta = (Vout - 500) / 10;
	return (uint8_t)Ta;
}

// wariant x10 bo chcem miec 0.1C
int16_t celciousFromAdcX10(int16_t adc) {
	int32_t Vout = (int32_t)adc * 1100UL / 1024UL;
	if(Vout <= 500) {
		return 0;
	}
    // hokus pokus i teraz 36.0C to 360
    // a 361 to 36.1C = wieksza precyzja
	int32_t Ta = (Vout - 500);
	return (int16_t)Ta;
}

void printX10(int16_t temp) {
    int16_t whole = (int16_t)(temp / 10);
    int16_t frac = (int16_t)(temp % 10);
    printf("%d.%01d C", whole, frac);
}

// wg pinow jak w zadaniu to nie mamy pwm
// wiec zrobimy wypelnienie "frame" jak kiedys
// w przypadku jasnosci leda

// w ms
#define TICK 1
#define FRAME 500
#define PID_PERIOD 500

// interrupt 1 ms
void timer2_init() {
    TCCR2A = _BV(WGM21);
    // prescaler 64
    TCCR2B = _BV(CS22);
    OCR2A = 249;
    TIMSK2 = _BV(OCIE2A);
}


uint16_t frameProgress = 0;
uint16_t pidMS = 0;
volatile uint8_t pidFlag = 0;

ISR(TIMER2_COMPA_vect) {
    frameProgress++;
    pidMS++;
    if(frameProgress >= FRAME) {
        frameProgress = 0;
    }
    if(frameProgress < heaterOnTicks) {
        heatOn();
    } else {
        heatOff();
    }
    if(pidMS >= PID_PERIOD) {
        pidMS = 0;
        pidFlag = 1;
    }
}


void Set_Input(int16_t inputValue)
{
    if(inputValue > FRAME) {
        inputValue = FRAME;
    }
    if(inputValue < 0) {
        inputValue = 0;
    }
  cli();
  heaterOnTicks = (uint16_t)inputValue;
  sei();
}

#define K_P 11.22
#define K_I 0.0765
#define K_D 0.01

pidData_t pidData;
volatile int16_t targetCelcious = 360;

int16_t Get_Reference(void)
{
//   return 8;
    return targetCelcious;
}

void pid_init() {
    pid_Init(K_P * SCALING_FACTOR, K_I * SCALING_FACTOR, K_D * SCALING_FACTOR, &pidData);
}

void setup() {
	uart_init();
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
	adc_init();
	heater_resistor_init();
    timer2_init();
    pid_init();
    sei();
}	


int main() {
    setup();
	// uint8_t on = 1;
	// uint8_t targetTemp = 36;
	// heatOn();
	// do inputu
	char inputLine[6];
	uint8_t index = 0;
    int32_t processVariableX10 = 0;
    uint8_t processVaraibleInInicialized = 0;
    int16_t lastOutputUsed = 0;

    while(1) {

        if(pidFlag) {
            pidFlag = 0;
            uint16_t adc = readADC_avgNoiseReducted();
            int16_t processVariableRead = celciousFromAdcX10(adc);

            // aby walczyc z szumem
            if(!processVaraibleInInicialized) {
                processVariableX10 = processVariableRead;
                processVaraibleInInicialized = 1;
            } else {
                processVariableX10 = (processVariableX10 * 3 + (int32_t)processVariableRead) / 4;
            }

            int16_t processVariableFiltred = (int16_t)processVariableX10;

            int16_t setPoint = Get_Reference();
            int32_t sumBefore = pidData.sumError;

            // odpalamy pid'a
            int16_t pidRaw = pid_Controller(setPoint, processVariableFiltred, &pidData);
            // w wypadku szalonych wartosci
            // wykorzystajmy poprzednia
            uint8_t glitched = 0;
            if(pidRaw == MAX_INT || pidRaw == INT16_MIN) {
                glitched = 1;
            }
            int16_t pidUsed = pidRaw;
            if(pidUsed > 500) {
                pidUsed = 500;
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
            int16_t powerUsed;
            cli();
            powerUsed = heaterOnTicks;
            sei();
            printf("SP: ");
            printX10(setPoint);
            printf(" PV: ");
            printX10(processVariableFiltred);
            printf(" Used: %"PRId16, powerUsed);
            printf("/%"PRIu16, FRAME);
            printf(" %"PRIu16, adc);
            printf("\r\n");
        }

        while(UCSR0A & _BV(RXC0)) {
			char ch = UDR0;
			switch(ch) {
			case '\r':
				break;;
			case '\n':
				inputLine[index] = '\0';
				index = 0;
				if(inputLine[0] != '\0') {
					// https://www.geeksforgeeks.org/c/convert-string-to-int-in-c/
					int val = strtol(inputLine, NULL, 10);
					if(val <= 0) {
						printf("Wrong value amigo\r\n");
					} else {
						targetCelcious = (int16_t)val * 10;
						pid_Reset_Integrator(&pidData);
                        processVariableX10 = 0;
                        processVaraibleInInicialized = 0;
                        lastOutputUsed = 0;
                        printf("New value set %"PRId8, val);
						printf("\r\n");
					}
				}
				break;
			default:
				if(index < sizeof(inputLine) - 1) {
					inputLine[index] = ch;
					index++;
				} else {
					index = 0;
					inputLine[0] = '\0';
				}
				break;
			}
		}
    }
}
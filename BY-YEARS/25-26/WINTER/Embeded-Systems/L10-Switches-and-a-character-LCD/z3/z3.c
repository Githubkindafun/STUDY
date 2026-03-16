
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

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
    ADCSRA |= _BV(ADSC);
}

uint16_t readADC() {
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
    ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
    uint16_t v = ADC; // weź zmierzoną wartość (0..1023)
    return v;
}

// wzorek z datasheeta
// po przeksztalceniach dostajemy
// T_a = (V_out - V_0) / T_c
// stale tez sa w datasheet
uint8_t celciousFromAdc(uint16_t adc) {
	uint32_t Vout = (uint32_t)adc * 1100UL / 1024UL;
	if(Vout <= 500) {
		return 0;
	}
	uint32_t Ta = (Vout - 500) / 10;
	return (uint8_t)Ta;
}


void setup() {
	uart_init();
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
	adc_init();
	heater_resistor_init();
}	


int main() {
    setup();
	uint8_t on = 1;
	uint8_t targetTemp = 36;
	heatOn();
	// do inputu
	char inputLine[6];
	uint8_t index = 0;

    while(1) {
		// bing bong nie zadziala jest blokujace
		// scanf("%u", &targetTemp);
        uint16_t adc = readADC();
		uint8_t tempCelc = celciousFromAdc(adc);
		if(on == 1) {
			if(tempCelc > targetTemp) {
				heatOff();
				on = 0;
			}
		} else {
			if(tempCelc <= targetTemp - Th) {
				heatOn();
				on = 1;
			}
		}

		printf("C: %"PRId8, tempCelc);
		printf("  ADC: %"PRId16, adc);
		if(on == 1) {
			printf("  STATUS: ON\r\n");
		} else {
			printf("  STATUS: OFF\r\n");
		}

		// trzbea jakos inputy sciagac
		// zatem tak długo jak coś jest w recive buffer to pobieramy
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
						targetTemp = val;
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


		_delay_ms(1000);
    }
}
#include "FreeRTOS.h"
#include "task.h"

#include <avr/io.h>


#include <stdio.h>
#include "uart.h"


#define mainLED_TASK_PRIORITY   2
#define mainSERIAL_TASK_PRIORITY 1

// w ms 
#define BUFFER_TIME 1000
#define TIME_FRAME 10
#define LED_LINE_DELAY 200

volatile uint8_t CYCLIC_BUFFER[100];

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB


#define BTN PB3
#define BTN_PIN PINB
#define BTN_PORT PORTB

// te z linijki led
#define LED_LINE_DDR DDRD
#define LED_LINE_PORT PORTD

#define LED0 PD0
#define LED1 PD1
#define LED2 PD2
#define LED3 PD3
#define LED4 PD4
#define LED5 PD5
#define LED6 PD6
#define LED7 PD7



static void vBtnLED(void* pvParameters);
static void vLedLine(void* pvParameters);

void setup() {
    // przycisk setup
    BTN_PORT |= _BV(BTN);
    LED_DDR |= _BV(LED);
    LED_LINE_DDR = 0b11111111;

}

int main(void)
{
    setup();


    xTaskHandle btnLed;

    xTaskCreate(
        vBtnLED,
        "btnLed",
        configMINIMAL_STACK_SIZE,
        NULL,
        1,
        &btnLed
    );

    xTaskHandle ledLine;

    xTaskCreate(
        vLedLine,
        "ledLine",
        configMINIMAL_STACK_SIZE,
        NULL,
        1,
        &ledLine
    );

    // Start scheduler.
    vTaskStartScheduler();

    return 0;
}

void vApplicationIdleHook(void)
{

}

// wazne, oba moga miec prio 1
// nie bedzie zaglozedznia bo oba sie blockuja na jakis czas
// a realnie robia prace tylko przez chwiile

static void vBtnLED(void* pvParameters) {
    // konwersja czasu na ticki
    TickType_t frame = pdMS_TO_TICKS(TIME_FRAME);

    uint8_t i = 0;
    while(1) {
        uint8_t buttonState = (BTN_PIN & _BV(BTN));
        CYCLIC_BUFFER[i] = !buttonState;
        if (CYCLIC_BUFFER[(i + 1) % 100]){
            LED_PORT |= _BV(LED);
        } else {
            LED_PORT &= ~_BV(LED);
        }
        i++;
        i = i % 100;
        // _delay_ms(TIME_FRAME) <- to tak dziala
        vTaskDelay(frame);
    }

}

int ledTable[8] = {
    LED0,
    LED1,
    LED2,
    LED3,
    LED4,
    LED5,
    LED6,
    LED7
};

void ledOn(int led) {
    LED_LINE_PORT |= _BV(led);
}

void ledOff(int led) {
    LED_LINE_PORT &= ~_BV(led);
}

static void vLedLine(void* pvParameters) {
    // delay ms -> tick
    // TickType_t delay = pdMS_TO_TICKS(LED_LINE_DELAY);

    while(1) {
        for(int i = 0; i < 8; i++) {
            if(i > 0) {
                ledOff(ledTable[i - 1]);
            }
            ledOn(ledTable[i]);
            vTaskDelay(LED_LINE_DELAY / portTICK_PERIOD_MS);
        }
        ledOff(ledTable[7]);
        for(int i = 7; i >= 0; i--) {
            if(i < 7) {
                ledOff(ledTable[i + 1]);
            }
            ledOn(ledTable[i]);
            vTaskDelay(LED_LINE_DELAY / portTICK_PERIOD_MS);
        }
        ledOff(ledTable[0]);
    }
}


#include "FreeRTOS.h"
#include "task.h"

#include <avr/io.h>


#include <stdio.h>
#include "uart.h"
#include "queue.h"

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

// opoznienie miedzy wejsciami z queue
#define LED_DELAY 2000
// ogolnie to z tym jakies mambo dzambo bylo
// i musi byc bez pdMS_TO_TICKS bo inaczej sie psuje

// globalny handle kolejki
// zeby kazdy mial do niej dostep
QueueHandle_t xledQueue;

// stare dobrze znane setupowanie uarta
#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

// inicjalizacja UART
void my_uart_init()
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


int my_uart_receive()
{
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0))) {
  }
  return UDR0;
}

void ledON() {
    LED_PORT |= _BV(LED);
}

void ledOFF() {
    LED_PORT &= ~_BV(LED);
}

void setup() {
    my_uart_init();
    // pb5 output
    LED_DDR |= _BV(LED);
    // led wylaczony
    LED_PORT &= ~_BV(LED);
}

// static void vBlinkLed(void* pvParameters);
// static void vSerial(void* pvParameters);
static void vReadTime(void *pvParameters);
static void vLedOnTime(void *pvParameters);

// flow programu
// ledontime ma prio 2 wiec pierwszy sie odpala
// blokuje sie na kolejce
// wtedy odpala sie readtime
// read time aktywnie czeka (my_uart_recive)
// jak cos przeczyta i wysle na kolejke
// to ledontime z wyzszym prio sie odblokuje i przejmie kontrole
// no i sobie poswieci dioda
// zasadniczo to dane sa caly czas czytane tylko jest block na momencik
// kiedy led jest opdalany

int main(void)
{
    setup();
    // Create task.
    // xTaskHandle blink_handle;
    // xTaskHandle serial_handle;

    xTaskHandle time_read_handle;
    xTaskHandle led_handle;
    xledQueue = xQueueCreate(16, sizeof(uint16_t));

    xTaskCreate
        (
         vReadTime,
         "readtime",
         configMINIMAL_STACK_SIZE,
         NULL,
         1,
         &time_read_handle
        );

    // tutaj kluczowe jest nadanie priorytetu 2 
    // dzieki temu jezeli bedzie ready a nie blocked to
    // zawsze zpstanie wybrany do pracy
    // inaczej przez to ze uart zasadniczo pernametnie sie kreci
    // to led by byl zaglodzony i sie nigdy nie wykonal
    xTaskCreate
        (
         vLedOnTime,
         "ledOnTime",
         configMINIMAL_STACK_SIZE,
         NULL,
         2,
         &led_handle
        );

    // Start scheduler.
    vTaskStartScheduler();

    return 0;
}



void vApplicationIdleHook(void)
{

}

// przez to ze tutaj nie ma zadnego delay
// to task jest pernamentie ready
static void vReadTime(void *pvParameters) {
    uint16_t numericValue = 0;
    uint8_t digitRead = 0;
    while(1) {
        uint8_t charachter = my_uart_receive();
        if(charachter >= '0' && charachter <= '9') {
            digitRead = 1;
            numericValue *= 10;
            numericValue += (uint16_t)(charachter - '0'); 
        } else {
            if(digitRead) {
                xQueueSendToBack(xledQueue, &numericValue, portMAX_DELAY);
                numericValue = 0;
                digitRead = 0;
            }
        }
    }
}

// kiedy zostanie cos sczytane z kolejki
// to dioda dziala (wyzsze prio)
static void vLedOnTime(void *pvParameters) {
    uint16_t timeToBeON = 0;

    while(1) {
        // czeka sobie w stanie blocked az cos wejdzie do kolejki
        if(xQueueReceive(xledQueue, &timeToBeON, portMAX_DELAY) == pdPASS) {
            ledON();
            vTaskDelay(timeToBeON / portTICK_PERIOD_MS);
            ledOFF();
            vTaskDelay(LED_DELAY / portTICK_PERIOD_MS);
        }
    }
}

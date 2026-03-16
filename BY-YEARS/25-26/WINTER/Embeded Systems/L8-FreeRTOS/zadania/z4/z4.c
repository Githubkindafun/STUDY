#include "FreeRTOS.h"
#include "task.h"
// semafor
#include "semphr.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdio.h>
#include "uart.h"


int my_uart_transmit(char c) {
  while (!(UCSR0A & _BV(UDRE0)));
  UDR0 = c;
  return 0;
}

// int uart_receive(FILE *stream) {
//   while (!(UCSR0A & _BV(RXC0))) taskYIELD();
//   return UDR0;
// }

void putString(const char *string) {
    uint8_t i = 0;
    while(string[i] != '\0') {
        my_uart_transmit(string[i]);
        i++;
    }
}

void putADCvalue(uint16_t ADCvalue) {
    uint8_t digit;
    uint8_t printStart = 0;

    if(ADCvalue >= 1000) {
        digit = ADCvalue / 1000;
        my_uart_transmit('0' + digit);
        ADCvalue %= 1000;
        printStart = 1;
    }

    if(ADCvalue >= 100 || printStart) {
        digit = ADCvalue / 100;
        my_uart_transmit('0' + digit);
        ADCvalue %= 100;
        printStart = 1;
    }

    if(ADCvalue >= 10 || printStart) {
        digit = ADCvalue / 10;
        my_uart_transmit('0' + digit);
        ADCvalue %= 10;
        printStart = 1;
    }
    my_uart_transmit('0' + ADCvalue);
}

// #define mainLED_TASK_PRIORITY   2

// #define mainSERIAL_TASK_PRIORITY 1

#define PHOTORESISTOR PC0
#define TERMISTOR PC1
#define POTENTIOMETER PC2


static void vPhotoresistor(void* pvParameters);
static void vTermistor(void* pvParameters);
static void vPotentiometer(void* pvParameters);

SemaphoreHandle_t xADCsemaphore;
SemaphoreHandle_t xADCdone;
SemaphoreHandle_t xPrintSemaphore;


void adc_init() {
    // 5V z arduino
    ADMUX |= _BV(REFS0);
    // ADC on
    ADCSRA |= _BV(ADEN);
    // ADC interrupty on
    ADCSRA |= _BV(ADIE);
    // prescaler na 128 bo czemu nie
    ADCSRA |= _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
}

uint16_t readADC(uint8_t mux) {
    uint16_t readValue;
    // portMAX_DELAY
    // will cause the task to wait indefinitely (without a timeout)
    // z mastering FreeRtos
    // tutaj jezeli nikt nie korzysta to przejmujemy i wykonujemy
    // wpp. jezeli jakis inny task jest w readADC to callujacy task przejdzie w stan Blocked
    xSemaphoreTake(xADCsemaphore, portMAX_DELAY);
    // tutaj decydujemy ktory z pinow ma dostep
    ADMUX = ADMUX & 0b11110000;
    ADMUX = ADMUX | (mux & 0b00001111);
    // odpalamy ADC
    ADCSRA |= _BV(ADSC);
    // tutaj skorzystamy z 2 semafora
    // bedzie sobie czekal az przerwanie da semafor / pozwoli przejsc
    xSemaphoreTake(xADCdone, portMAX_DELAY);
    // skoro przerwanie nastapilo => adc sie skonczylo
    // mozemy bezpiecznie sczytac wartosc
    readValue = ADC;
    // Macro to release a semaphore. ladnie opisane na stronce
    xSemaphoreGive(xADCsemaphore);
    return readValue;
}

// oki doki przerwanie 
ISR(ADC_vect) {
    // oddajemy semafor
    // zasadniczo sygnalizujemy ze ADC skonczylo i mozna przeczytac wartosc
    // NULL dlatego ze kazdy task ma to samo prio
    // opisane bylo ze to ma znaczenie tylko jezeli jest jakis task o wyzszym prio
    xSemaphoreGiveFromISR(xADCdone, NULL);
}
// tutaj mnie zlapalo przemyslenie:
// Funkcja powinna działać w sytuacji, gdy
// wiele zadań zażąda pomiaru ADC (być może z różnych wejść); w takiej sytuacji pomiary powinny zostać
// wykonane po kolei
// to znaczy A->B->C ?
// znalazlem
//  If all tasks are the same priority, then the priority doesn’t select which 
// task gets chosen, and among equal priority tasks, the one waiting the 
// longest it chosen first.  
// https://freertos.org/FreeRTOS_Support_Forum_Archive/March_2013/freertos_Mutex_Priority_Inversion_Anamoly_6876559.html





void setup() {
    uart_init();
    adc_init();
    // fdev_setup_stream(&uart_file, my_uart_transmit, uart_receive, _FDEV_SETUP_RW);
    // stdin = stdout = stderr = &uart_file;
}


int main(void)
{
    setup();
    // Create task.
    // xTaskHandle blink_handle;
    // xTaskHandle serial_handle;
    // z tego co rozumiem nie musza tu byc ale i tak dalem
    // jak w przykladzie
    xTaskHandle photoresisitor_handle;
    xTaskHandle termistor_handle;
    xTaskHandle potentiometer_handle;

    xADCsemaphore = xSemaphoreCreateBinary();
    xADCdone = xSemaphoreCreateBinary();
    xPrintSemaphore = xSemaphoreCreateBinary();

    // aby semafory dzialaly musimy im cos dac
    // The semaphore is created in the 'empty' state, meaning the semaphore must first be given
    // using the xSemaphoreGive() API function before it can subsequently be
    // taken (obtained) using the xSemaphoreTake() function.
    xSemaphoreGive(xADCsemaphore);
    xSemaphoreGive(xPrintSemaphore);
    // i nie dajemy ADCdone bo on tylko daje gdy ADC sie skonczy robic

    xTaskCreate
        (
         vPhotoresistor,
         "photoresistorTask",
         configMINIMAL_STACK_SIZE,
         NULL,
         1,
         &photoresisitor_handle
        );

    xTaskCreate
        (
         vTermistor,
         "termistorTask",
         configMINIMAL_STACK_SIZE,
         NULL,
         1,
         &termistor_handle
        );

    xTaskCreate
        (
         vPotentiometer,
         "potentiometerTask",
         configMINIMAL_STACK_SIZE,
         NULL,
         1,
         &potentiometer_handle
        );

    

    // Start scheduler.
    vTaskStartScheduler();

    return 0;
}


void vApplicationIdleHook(void)
{

}

static void vPhotoresistor(void* pvParameters) {
    // TickType_t timeInterval = pdMS_TO_TICKS(100);
    while(1) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
        uint16_t readPhotoresistorValue = readADC(PHOTORESISTOR);
        // taka mini sekcja krytyczna aby printy byly znosne
        // wykorzystac mutex zamiast semafora !
        xSemaphoreTake(xPrintSemaphore, portMAX_DELAY);
        putString("PHOTORESISTOR: ");
        putADCvalue(readPhotoresistorValue);
        putString("\r\n");
        xSemaphoreGive(xPrintSemaphore);
    }
}

static void vTermistor(void* pvParameters) {
    // TickType_t timeInterval = pdMS_TO_TICKS(200);
    while(1) {
        vTaskDelay(200 / portTICK_PERIOD_MS);
        uint16_t readvTermistorValue = readADC(TERMISTOR);
        xSemaphoreTake(xPrintSemaphore, portMAX_DELAY);
        putString("TERMISTOR: ");
        putADCvalue(readvTermistorValue);
        putString("\r\n");
        xSemaphoreGive(xPrintSemaphore);
    }
}

static void vPotentiometer(void* pvParameters) {
    // TickType_t timeInterval = pdMS_TO_TICKS(300);
    while(1) {
        vTaskDelay(300 / portTICK_PERIOD_MS);
        uint16_t readvPotentiometerValue = readADC(POTENTIOMETER);
        xSemaphoreTake(xPrintSemaphore, portMAX_DELAY);
        putString("POTENTIOMETER: ");
        putADCvalue(readvPotentiometerValue);
        putString("\r\n");
        xSemaphoreGive(xPrintSemaphore);
    }
}



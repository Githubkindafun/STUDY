#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include "USI_TWI_Master.c"


#define READ 1
#define WRITE 0

int main() {
    // z skosa
    USI_TWI_Master_Initialise();

    uint8_t toSend = 1;
    while(1) {
        // tam ta funkcja Start_Transceiver_With_Data
        // przyjmuje 2 argumenty ( unsigned char *msg, unsigned char msgSize)
        // gdzie w nasyzm przypadku msgSize = 2 no bo wysylamy 2 bajty
        // addr + value
        // w main zawse definiowane to bylo tak
        // messageBuf[0] = cos crazy ale wyglada na adres
        // messageBuf[1] = data do przeslania

        uint8_t messageBuf[2];
        // addr + zapis/odczyt
        messageBuf[0] = (uint8_t)((0x7f << 1) | WRITE);
        messageBuf[1] = toSend;
        
        uint8_t status = USI_TWI_Start_Transceiver_With_Data(messageBuf, sizeof(messageBuf));
        // to do jezeli starczy czasu, error check
        toSend++;
        _delay_ms(1000);
    }
}
/*
Napisz jak najprostszy program, który będzie wykonywał za pomocą UART funkcję „echo”:
każdy odebrany przez układ znak powinien zostać przesłany z powrotem do nadawcy.
Nie używaj funkcji z biblioteki standardowej języka C (printf, scanf, getchar itp.). 
Wykorzystaj przerwania, 
pętla główna programu powinna wyłącznie usypiać mikrokontroler do stanu bezczynności 
(głębsze tryby uśpienia wyłączają układ UART).
*/

// taki bajernacki PDF znalazlem https://www.mikrocontroller.net/attachment/164063/InterruptUSART.pdf
// tam pokazane bylo co i jak


#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>


#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

void setup() {

    // ustaw baudrate
    UBRR0 = UBRR_VALUE;

    // Bit 4 – RXENn: Receiver Enable n
    // Writing this bit to one enables the USART Receiver.
    UCSR0B |= _BV(RXEN0);
    
    // Bit 3 – TXENn: Transmitter Enable n
    // Writing this bit to one enables the USART Transmitter.
    UCSR0B |= _BV(TXEN0);
    // majac te 2 bity ustawione mamy: 
    // odblokowanie odbierania danych
    // odblokowanie wysylania danych

    // Bit 7 – RXCIEn: RX Complete Interrupt Enable n
    // A USART Receive Complete interrupt will be generated
    // only if the RXCIEn bit is written to one
    // zasadniczo z kazdym odebranym bajtem trigeruje sie interrupt
    UCSR0B |= _BV(RXCIE0);

    // ustawiamy format ramki na  8 bitow
    UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);

    // odblokowujemy interrupty
    sei();
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();
}

// przerwanie
ISR(USART_RX_vect) {
    // w UDR0 znajduje sie odebrany bajt
    // po odczytaniu zostanie wyczyszczona flaga RX
    // czytamy z recivera i potem podajemy do transmitera
    uint8_t recivedData = UDR0;
    // hen data is written to the transmit
    // buffer, and the Transmitter is enabled, the Transmitter will load the data into the 
    // Transmit Shift Register when the
    // Shift Register is empty. Then the data will be serially transmitted on the TxDn pin.
    UDR0 = recivedData;
}


int main() {
    //  usypianie
    setup();
    while(1) {
        sleep_mode();
    }
}
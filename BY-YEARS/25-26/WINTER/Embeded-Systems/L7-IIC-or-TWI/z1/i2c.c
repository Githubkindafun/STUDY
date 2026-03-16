#include "i2c.h"
void i2cInit() {
  // ustaw bitrate
  // 8MHz / (16+2*TWBR*prescaler) ~= 100kHz -> chyba 16MHz
  // ustawiamy SCL freq
  TWBR = 72;
  // uruchom TWI - lieralnie 
  TWCR |= _BV(TWEN);
}
void i2cWaitForComplete() {
  // czekaj na flagę TWINT
  // TWINT bedzie 1 gdy TWI skonczy
  // jak pracuje to jest 0
  while(!(TWCR & _BV(TWINT)));
}
void i2cStart() {
  // wyślij warunek startu
  // TWASTA - generates a START condition on the bus if it is free
  // TWEN - enables TWI
  // TWINT - czyscimy TWINT
  TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTA);
  i2cWaitForComplete();
}
void i2cStop() {
  // wyślij warunek stopu
  // TWST0 -  will generate a STOP condition
  TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);
}
void i2cReset() {
  // wyślij warunek stopu i wyłącz TWI - literalnie
  TWCR = _BV(TWINT) | _BV(TWSTO);
}
uint8_t i2cReadAck() {
  // TWEA - The TWEA bit controls the generation of the acknowledge pulse. If the TWEA bit is written to one, the ACK pulse is
  // generated on the TWI bus
  // odczytaj dane, wyślij ACK
  TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
  i2cWaitForComplete();
  // TWDR – TWI Data Register
  return TWDR;
}
uint8_t i2cReadNoAck() {
  // odczytaj dane, wyślij NOACK - literalnie
  TWCR = _BV(TWINT) | _BV(TWEN);
  i2cWaitForComplete();
  return TWDR;
}
void i2cSend(uint8_t data) {
  // wyślij dane
  TWDR = data;
  TWCR = _BV(TWINT) | _BV(TWEN);
  i2cWaitForComplete();
}

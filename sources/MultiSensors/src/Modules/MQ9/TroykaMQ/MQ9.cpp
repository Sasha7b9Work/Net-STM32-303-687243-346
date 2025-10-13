#include "defines.h"
#include "MQ9.h"

MQ9::MQ9(uint8 pin)
  : BaseMQ(pin) {
}

MQ9::MQ9(uint8 pin, uint8 pinHeater)
  : BaseMQ(pin, pinHeater) {
}

unsigned long MQ9::readLPG() {
  return (unsigned long)readScaled(-0.48f, 3.33f);
}

unsigned long MQ9::readMethane() {
  return (unsigned long)readScaled(-0.38f, 3.21f);
}

unsigned long MQ9::readCarbonMonoxide() {
  return (unsigned long)readScaled(-0.48f, 3.10f);
}

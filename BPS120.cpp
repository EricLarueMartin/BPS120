/*
  Pressure recording for SEEED W600 device.
  Unfortunately the W600 development board is a PITA to work with since the only library for it forces strings to be stored in flash.
  At a price savings of $6 a board over the Feather HUZZAH it isn't worth the trouble to use W600 over an ESP8266 system.
*/


#include <BPS120.h>

int BPS120::measure(){
  Wire.requestFrom(addr,2); // request 2 bytes from slave device
  if (Wire.available()) {
    char c = Wire.read(); // receive a byte as character
    statusBits = c >> 6; // status bits are the leading two bits
    int nPressure = (int)(c & 0x3f) << 8; // mask removes the status bits, shift a byte then read least significant bits
    if (Wire.available()) { // slave may send less than requested
      c = Wire.read(); // read the least significant bits
      nPressure += c; // and add those bits to the most significant bits to attain measured raw pressure
    }
    if (statusBits == 0) { // 0 indicated a valid measurement was taken, so update pressure
      if (abs(rawPressure-nPressure)>diffMax) // if measurement has jumped significantly reset count at new pressure
        count = 1;
      else // for small changes then keep a running average
        ++count;
      // average measurements until enough counts are built up, then apply decay
      rawPressure = rawPressure + (nPressure-rawPressure)/((count>decayTime)?decayTime:count);
    }
  }
  else statusBits = 4; // there are two status bits, this overflow is used for no response
  return statusBits;
}


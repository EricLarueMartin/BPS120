/*
  Pressure recording for SEEED W600 device.
  Unfortunately the W600 development board is a PITA to work with since the only library for it forces strings to be stored in flash.
  At a price savings of $6 a board over the Feather HUZZAH it isn't worth the trouble to use W600 over an ESP8266 system.
*/
#include <Wire.h>

class BPS120 {
public:
  const int rawMax = 16384;
  const double pMax = 0.15f;
  const double pMin = -0.15f;
  const double rawForZero = (pMax-9*pMin)*rawMax/10/(pMax-pMin);
  const int addr = 0x28; // default I2C address
  unsigned long count = 0; // running counter of valid measurements made
  double rawOffset = 0.0f; // offset added to raw measurement to adjust null
  double scaleFactor = 1.0f; // scale factor applied to calculated pressure to adjust range
  double decayTime = 10000.0f; // how much of pressure each loop is the latest measurement
  int diffMax = rawMax/64; // if measured pressure differs from history by more than this amount assume a pressure jump and reset history
  char statusBits = 4; // there are two status bits, this overflow is used for no response
  double rawPressure = -diffMax;
  double measure();
  double psid() {return ((pMax-pMin)*(rawPressure-0.1*rawMax+rawOffset)/(0.8*rawMax)+pMin)*scaleFactor;};
  double inH2O() {return psid()*27.7076f;};
  double Torr() {return psid()*51.7149f;};
  double mTorr() {return psid()*51714.9f;};
  double Pa() {return psid()*6894.76f;};
  double kPa() {return psid()*6.89476f;};
  double atm() {return psid()*0.068046f;};
  void zero(double rawNull){rawOffset = (rawNull==0.0f)?rawForZero-rawPressure:rawNull;}; // 0.0f from calculations is a bad check, but to indicate to use current pressure it's fine
  void setDecay(double setPoint){decayTime = (setPoint > 1.0f)?setPoint:1.0f;}; // It makes no sense to average less than one measurement
};

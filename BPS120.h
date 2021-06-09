#include <Wire.h>

class BPS120 {
public:
  const int rawMax = 16384;
  const float pMax = 0.15f;
  const float pMin = -0.15f;
  const double rawForZero = (pMax-9*pMin)*rawMax/10/(pMax-pMin);
  const int addr = 0x28; // default I2C address
  unsigned long count = 0; // running counter of valid measurements made
  double rawOffset = 0.0f; // offset added to raw measurement to adjust null
  double scaleFactor = 1.0f; // scale factor applied to calculated pressure to adjust range
  double decayTime = 1000.0f; // how much of pressure each loop is the latest measurement
  int diffMax = rawMax/64; // if measured pressure differs from history by more than this amount assume a pressure jump and reset history
  char statusBits = 4; // there are two status bits, this overflow is used for no response
  double rawPressure = -diffMax;
  int measure();
  double rawTopsid(double raw) {return ((pMax-pMin)*(raw-0.1*rawMax)/(0.8*rawMax)+pMin)*scaleFactor;};
  double rawToinH2O(double raw) {return rawTopsid(raw)*27.7076f;};
  double rawToTorr(double raw) {return rawTopsid(raw)*51.7149f;};
  double rawTomTorr(double raw) {return rawTopsid(raw)*51714.9f;};
  double rawToPa(double raw) {return rawTopsid(raw)*6894.76f;};
  double rawTokPa(double raw) {return rawTopsid(raw)*6.89476f;};
  double rawToatm(double raw) {return rawTopsid(raw)*0.068046f;};
  double psid() {return rawTopsid(rawPressure+rawOffset);};
  double inH2O() {return psid()*27.7076f;};
  double Torr() {return psid()*51.7149f;};
  double mTorr() {return psid()*51714.9f;};
  double Pa() {return psid()*6894.76f;};
  double kPa() {return psid()*6.89476f;};
  double atm() {return psid()*0.068046f;};
  void zero(double rawNull){rawOffset = (rawNull==0.0f)?rawForZero-rawPressure:rawNull;}; // 0.0f from calculations is a bad check, but to indicate to use current pressure it's fine
  void setDecay(double setPoint){decayTime = (setPoint > 1.0f)?setPoint:1.0f;}; // It makes no sense to average less than one measurement
  double psidToRaw(double p) {return rawMax*(0.8*p/scaleFactor+0.1*pMax-0.9*pMin)/(pMax-pMin);};
  double inH2OToRaw(double p) {return psidToRaw(p/27.7076f);};
  double TorrToRaw(double p) {return psidToRaw(p/51.7149f);};
  double mTorrToRaw(double p) {return psidToRaw(p/51714.9f);};
  double PaToRaw(double p) {return psidToRaw(p/6894.76f);};
  double kPaToRaw(double p) {return psidToRaw(p/6.89476f);};
  double atmToRaw(double p) {return psidToRaw(p/0.068046f);};
};

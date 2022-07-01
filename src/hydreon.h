/*
  Hydreon Rainsensor Library for senseBox
*/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifndef HYDREON_h
#define HYDREON_h
class HYDREON
{
public:
  HYDREON(Uart &serial);
  void begin();                            // start sensor
  void readAllData();                      // read all Data
  void setContinuousMode(bool continuous); // set Continous Mode
  void setHighResolution(bool high);       // set to HighResolution mode if true
  void reset();                            // reset past sensor measurements
  float getAccumulation();                 // get Accumulation as float
  float getEventAccumulation();            // get Event Accumulation as float
  float getTotalAccumulation();            // get total Accumulation as float
  float getRainfallIntensity();            // get rainfallIntensity as
  String getDataString();

private:
  float accumulation;
  float eventAccumulation;
  float totalAccumulation;
  float rainfallIntensity;
  int startIndex;
  int endIndex;
  String data;
  bool continuousMode;
  Uart &sensor;
};

#endif
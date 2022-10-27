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
  void begin();                      // start sensor
  void readAllData();                // read all Data
  void setHighResolution(bool high); // set to HighResolution mode if true
  void temporaryReset();             // temporary reset past sensor measurements
  float getAccumulation();           // get Accumulation since last request as float
  float getEventAccumulation();      // get Event Accumulation as float
  float getTotalAccumulation();      // get total Accumulation as float
  float getRainfallIntensity();      // get rainfallIntensity as loat
  String getDataString();            // get data String

private:
  float accumulation;
  float eventAccumulation;
  float totalAccumulation;
  float totalAccumulationTemp;
  float rainfallIntensity;
  int startIndex;
  int endIndex;
  String data;
  Uart &sensor;
};

#endif
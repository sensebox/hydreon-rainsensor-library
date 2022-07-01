/*
  Hydreon Rainsensor Library for senseBox
*/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "hydreon.h"
HYDREON::HYDREON(Uart &serial) : sensor(serial) {}

void HYDREON::begin()
{
  sensor.begin(9600);  // opens serial port
  sensor.println('M'); // force Metric
  sensor.println('H'); // high Reselution
  sensor.println('P'); // pooling mode (not continuous)
  continuousMode = false;
  delay(200);
  while (sensor.available())
    sensor.read();
}

void HYDREON::readAllData()
{
  if (!continuousMode)
    sensor.println('R');

  delay(200);
  if (sensor.available())
  {
    while (sensor.available())
      data = sensor.readStringUntil('\n');

    startIndex = data.indexOf("Acc") + 3;
    endIndex = data.indexOf("mm,");
    accumulation = data.substring(startIndex, endIndex).toFloat();
    startIndex = data.indexOf("EventAcc") + 8;
    endIndex = data.indexOf("mm,", startIndex);
    eventAccumulation = data.substring(startIndex, endIndex).toFloat();
    startIndex = data.indexOf("TotalAcc") + 8;
    endIndex = data.indexOf("mm,", startIndex);
    totalAccumulation = data.substring(startIndex, endIndex).toFloat();
    startIndex = data.indexOf("RInt") + 4;
    endIndex = data.indexOf("mmph", startIndex);
    rainfallIntensity = data.substring(startIndex, endIndex).toFloat();
  }
}

void HYDREON::setContinuousMode(bool continuous)
{
  continuousMode = continuous;
  sensor.println('C' ? continuous : 'P');
  delay(200);
  while (sensor.available())
    sensor.read();
}

void HYDREON::setHighResolution(bool high)
{
  sensor.println('H' ? high : 'L');
  delay(200);
  while (sensor.available())
    sensor.read();
}

void HYDREON::reset()
{
  sensor.println('O');
  delay(200);
  while (sensor.available())
    sensor.read();
}

float HYDREON::getAccumulation()
{
  return accumulation;
}

float HYDREON::getEventAccumulation()
{
  return eventAccumulation;
}

float HYDREON::getTotalAccumulation()
{
  return totalAccumulation;
}

float HYDREON::getRainfallIntensity()
{
  return rainfallIntensity;
}

String HYDREON::getDataString()
{
  return data;
}

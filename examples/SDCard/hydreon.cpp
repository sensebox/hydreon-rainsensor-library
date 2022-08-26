/*
  Hydreon Rainsensor Library for senseBox
*/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "hydreon.h"
#define HYDREON_SERIAL_DELAY 300

HYDREON::HYDREON(Uart &serial) : sensor(serial) {}

void HYDREON::begin()
{
  sensor.begin(9600);  // opens serial port
  sensor.println('M'); // force Metric
  sensor.println('H'); // high Resulution
  sensor.println('P'); // pooling mode (not continuous)
  delay(HYDREON_SERIAL_DELAY);
  while (sensor.available())
    sensor.read();
}

void HYDREON::readAllData()
{
  sensor.println('R');
  delay(HYDREON_SERIAL_DELAY);
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
    // totalAccumulationTemp = data.substring(startIndex, endIndex).toFloat();
    // totalAccumulation = max(totalAccumulation, totalAccumulationTemp); // fix to enforce monotonic increas on bug
    startIndex = data.indexOf("RInt") + 4;
    endIndex = data.indexOf("mmph", startIndex);
    rainfallIntensity = data.substring(startIndex, endIndex).toFloat();
  }
}

void HYDREON::setHighResolution(bool high)
{
  sensor.println('H' ? high : 'L');
  delay(HYDREON_SERIAL_DELAY);
  while (sensor.available())
    sensor.read();
}

void HYDREON::reset()
{
  sensor.println('O');
  delay(HYDREON_SERIAL_DELAY);
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

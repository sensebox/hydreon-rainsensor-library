#include <senseBoxIO.h>
#include "hydreon.h"

HYDREON rainsensor(Serial1); //

void setup()
{
    SerialUSB.begin(9600);
    rainsensor.begin();
    // rainsensor.setContinuousMode(true) //totest
    rainsensor.setHighResolution(true);
    SerialUSB.println("restart");
}

void loop()
{
    rainsensor.readAllData();
    SerialUSB.print(rainsensor.getAccumulation());
    SerialUSB.print('\t');
    SerialUSB.print(rainsensor.getEventAccumulation());
    SerialUSB.print('\t');
    SerialUSB.print(rainsensor.getTotalAccumulation());
    SerialUSB.print('\t');
    SerialUSB.println(rainsensor.getRainfallIntensity());
    SerialUSB.println(rainsensor.getDataString());
    delay(2000);
}

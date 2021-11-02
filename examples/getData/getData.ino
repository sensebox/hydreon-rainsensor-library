#include <senseBoxIO.h>
#include "hydreon.h"

HYDREON rainsensor(Serial1); //

void setup() {
 Serial.begin(9600);
 rainsensor.begin();
 delay(1000);
 
}

void loop() {
 Serial.println(rainsensor.getAcc());
 rainsensor.getAllData();
delay(10000);
}

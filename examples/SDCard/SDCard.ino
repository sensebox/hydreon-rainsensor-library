#include <RV8523.h>
#include <senseBoxIO.h>
#include <SPI.h>
#include <SD.h>
#include "hydreon.h"

const char SENSOR_IDEC2[] PROGMEM = "62b9dc101d75e2001c4b4ec2";
const char SENSOR_IDEC3[] PROGMEM = "62b9dc101d75e2001c4b4ec3";
const char SENSOR_IDEC4[] PROGMEM = "62b9dc101d75e2001c4b4ec4";
const char SENSOR_IDEC5[] PROGMEM = "62b9dc101d75e2001c4b4ec5";

char timestamp[20];

RV8523 rtc;
File Data;
const char SENSOR_IDRID[] PROGMEM = "sensorID";
static const uint8_t NUM_SENSORS = 4;
typedef struct measurement
{
    const char *sensorId;
    float value;
} measurement;
char buffer[750];
measurement measurements[NUM_SENSORS];
uint8_t num_measurements = 0;

char *getTimeStamp()
{
    uint8_t sec, min, hour, day, month;
    uint16_t year;
    rtc.get(&sec, &min, &hour, &day, &month, &year);
    sprintf(timestamp, "%02d-%02d-%02dT%02d:%02d:%02dZ", year, month, day, hour, min, sec);
    return timestamp;
}

void addMeasurement(const char *sensorId, float value)
{
    measurements[num_measurements].sensorId = sensorId;
    measurements[num_measurements].value = value;
    num_measurements++;
}

void writeMeasurementsToSdCard(char *timeStamp)
{
    // iterate throug the measurements array
    for (uint8_t i = 0; i < num_measurements; i++)
    {
        sprintf_P(buffer, PSTR("%s,%9.2f,%s"), measurements[i].sensorId, measurements[i].value, timeStamp);
        // transmit buffer to client
        Data.println(buffer);
    }
    // reset num_measurements
    num_measurements = 0;
}

void saveValues()
{

    // send measurements
    writeMeasurementsToSdCard(getTimeStamp());
    num_measurements = 0;
}

HYDREON rainsensor1(Serial1);

void setup()
{
    rtc.begin();
    SD.begin(28);

    Data = SD.open("Data.txt", FILE_WRITE);
    Data.close();

    rtc.start();
    rtc.batterySwitchOver(1);

    rainsensor1.begin();
}

void loop()
{
    rainsensor1.readAllData();
    Data = SD.open("Data.txt", FILE_WRITE);
    addMeasurement(SENSOR_IDEC2, rainsensor1.getAccumulation());
    addMeasurement(SENSOR_IDEC3, rainsensor1.getEventAccumulation());
    addMeasurement(SENSOR_IDEC4, rainsensor1.getTotalAccumulation());
    addMeasurement(SENSOR_IDEC5, rainsensor1.getRainfallIntensity());
    saveValues();
    Data.close();
    delay(5 * 60 * 1000);
}
#include <senseBoxIO.h>
#include <WiFi101.h>
#include <ArduinoBearSSL.h>
#include <ArduinoECCX08.h>
#include "hydreon.h"

char ssid[] = "Tardis";
char pass[] = "k33pc4!m4ndc4!!th3d0ct0r";
int status = WL_IDLE_STATUS;

const long intervalInterval = 10000;
long time_startInterval = 0;
long time_actualInterval = 0;

const char SENSOR_IDEC2[] PROGMEM = "62b9dc101d75e2001c4b4ec2";
const char SENSOR_IDEC3[] PROGMEM = "62b9dc101d75e2001c4b4ec3";
const char SENSOR_IDEC4[] PROGMEM = "62b9dc101d75e2001c4b4ec4";
const char SENSOR_IDEC5[] PROGMEM = "62b9dc101d75e2001c4b4ec5";
const char SENSOR_ID972[] PROGMEM = "62bdaaf1c594f8001bc5e972";
const char SENSOR_ID973[] PROGMEM = "62bdaaf1c594f8001bc5e973";
const char SENSOR_ID974[] PROGMEM = "62bdaaf1c594f8001bc5e974";
const char SENSOR_ID975[] PROGMEM = "62bdaaf1c594f8001bc5e975";
static const uint8_t NUM_SENSORS = 8;
const char SENSEBOX_ID[] PROGMEM = "626c5022c18b98001cb5c036";
const char server[] PROGMEM = "ingress.opensensemap.org";
WiFiClient wifiClient;
BearSSLClient client(wifiClient);
typedef struct measurement
{
    const char *sensorId;
    float value;
} measurement;
char buffer[750];
measurement measurements[NUM_SENSORS];
uint8_t num_measurements = 0;
const int lengthMultiplikator = 35;

unsigned long getTime()
{
    return WiFi.getTime();
}

void addMeasurement(const char *sensorId, float value)
{
    measurements[num_measurements].sensorId = sensorId;
    measurements[num_measurements].value = value;
    num_measurements++;
}

void writeMeasurementsToClient()
{
    // iterate throug the measurements array
    for (uint8_t i = 0; i < num_measurements; i++)
    {
        sprintf_P(buffer, PSTR("%s,%9.2f\n"), measurements[i].sensorId,
                  measurements[i].value);
        // transmit buffer to client
        client.print(buffer);
    }
    // reset num_measurements
    num_measurements = 0;
}

void submitValues()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.disconnect();
        delay(1000); // wait 1s
        WiFi.begin(ssid, pass);
        delay(5000); // wait 5s
    }
    if (client.connected())
    {
        client.stop();
        delay(1000);
    }
    bool connected = false;
    char _server[strlen_P(server)];
    strcpy_P(_server, server);
    for (uint8_t timeout = 2; timeout != 0; timeout--)
    {
        Serial.println(F("connecting..."));
        connected = client.connect(_server, 443);
        if (connected == true)
        {
            // construct the HTTP POST request:
            sprintf_P(buffer,
                      PSTR("POST /boxes/%s/data HTTP/1.1\nAuthorization: f422bf0d52ab54de9bf416118f594a58a1b418605b14cd8bbdddbac6e4249cd3\nHost: %s\nContent-Type: "
                           "text/csv\nConnection: close\nContent-Length: %i\n\n"),
                      SENSEBOX_ID, server, num_measurements * lengthMultiplikator);
            // send the HTTP POST request:
            client.print(buffer);
            // send measurements
            writeMeasurementsToClient();
            // send empty line to end the request
            client.println();
            uint16_t timeout = 0;
            // allow the response to be computed
            while (timeout <= 5000)
            {
                delay(10);
                timeout = timeout + 10;
                if (client.available())
                {
                    break;
                }
            }

            while (client.available())
            {
                char c = client.read();
                // if the server's disconnected, stop the client:
                if (!client.connected())
                {
                    client.stop();
                    break;
                }
            }

            num_measurements = 0;
            break;
        }
        delay(1000);
    }

    if (connected == false)
    {
        delay(5000);
        noInterrupts();
        NVIC_SystemReset();
        while (1)
            ;
    }
}

HYDREON rainsensor1(Serial1);
HYDREON rainsensor2(Serial2);

void setup()
{

    if (WiFi.status() == WL_NO_SHIELD)
    {
        while (true)
            ;
    }
    while (status != WL_CONNECTED)
    {
        status = WiFi.begin(ssid, pass);
        delay(5000);
    }

    ArduinoBearSSL.onGetTime(getTime);

    rainsensor1.begin();
    rainsensor1.reset();
    rainsensor2.begin();
    rainsensor2.reset();
    rainsensor2.setHighResolution(true);
    // rainsensor2.setContinuousMode(true);

    SerialUSB.begin(9600);
    SerialUSB.println("start");
}

void loop()
{
    rainsensor1.readAllData();
    rainsensor2.readAllData();
    time_startInterval = millis();

    if (time_startInterval > time_actualInterval + intervalInterval)
    {
        time_actualInterval = millis();
        addMeasurement(SENSOR_IDEC2, rainsensor1.getAccumulation());
        addMeasurement(SENSOR_IDEC3, rainsensor1.getEventAccumulation());
        addMeasurement(SENSOR_IDEC4, rainsensor1.getTotalAccumulation());
        addMeasurement(SENSOR_IDEC5, rainsensor1.getRainfallIntensity());
        addMeasurement(SENSOR_ID972, rainsensor2.getAccumulation());
        addMeasurement(SENSOR_ID973, rainsensor2.getEventAccumulation());
        addMeasurement(SENSOR_ID974, rainsensor2.getTotalAccumulation());
        addMeasurement(SENSOR_ID975, rainsensor2.getRainfallIntensity());
        submitValues();
        SerialUSB.println(rainsensor1.getDataString());
        SerialUSB.println(rainsensor2.getDataString());
        SerialUSB.println();
    }
}
/*
  Hydreon Rainsensor Library for senseBox
*/


#if ARDUINO >= 100
# include "Arduino.h"
#else
# include "WProgram.h"
#endif
#include "hydreon.h"


HYDREON::HYDREON(Uart& serial):
  sensor(serial) {}

void HYDREON::begin() {

  sensor.begin(9600);// opens serial port, sets data rate to 9600 bps
  sendCmd('M');
  sendCmd('P');

}

void HYDREON::setContinuousMode(bool cmd) {
   if (cmd == true) {
    sendCmd('C');
  } else if (cmd == false) {
    sendCmd('P');
  }
}

void HYDREON::sendCmd(char cmd) {
  sensor.print(cmd);
  sensor.print('\r');
  sensor.print('\n');
  sensor.flush();
  delay(1000);
}


float HYDREON::getAcc() {
  recvWithEndMarker('A');
  if (newData == true) {
    acc = Rg15Parse(rain_sensitivity, "Acc");
    newData = false;
    return acc;
  }
  else return 1;
}

void HYDREON::setHighResolution(bool cmd) {
  if (cmd == true) {
    sendCmd('H');
  } else if (cmd == false) {
    sendCmd('L');
  }
}

void HYDREON::getAllData() {
  recvWithEndMarker('R');
  Serial.println(rain_sensitivity);
}

void HYDREON::pollContinuousData() {
  recvWithEndMarker('null');
  Serial.println(rain_sensitivity);
}

void HYDREON::recvWithEndMarker(char cmd) {
    sendCmd(cmd);
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  // if (Serial.available() > 0) {
  while (sensor.available() > 0 ) {
    // while (Serial1.available() > 0 && newData == false) {
    rc = sensor.read();

    if (rc != endMarker) {
      rain_sensitivity[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      rain_sensitivity[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

float HYDREON::CharToFloat(const char *str)
{
  // simple ascii to double, because atof or strtod are too large
  char strbuf[24];

  strlcpy(strbuf, str, sizeof(strbuf));
  char *pt = strbuf;
  if (*pt == '\0') {
    return 0.0;
  }

  while ((*pt != '\0') && isspace(*pt)) {
    pt++;  // Trim leading spaces
  }

  signed char sign = 1;
  if (*pt == '-') {
    sign = -1;
  }
  if (*pt == '-' || *pt == '+') {
    pt++;  // Skip any sign
  }

  float left = 0;
  if (*pt != '.') {
    left = atoi(pt);                               // Get left part
    while (isdigit(*pt)) {
      pt++;  // Skip number
    }
  }

  float right = 0;
  if (*pt == '.') {
    pt++;
    uint32_t max_decimals = 0;
    while ((max_decimals < 8) && isdigit(pt[max_decimals])) {
      max_decimals++;
    }
    pt[max_decimals] = '\0';                       // Limit decimals to float max of 8
    right = atoi(pt);                              // Decimal part
    while (isdigit(*pt)) {
      pt++;
      right /= 10.0f;
    }
  }

  float result = left + right;
  if (sign < 0) {
    return -result;                                // Add negative sign
  }
  return result;
}



float HYDREON::Rg15Parse(char* buffer, const char* item) {
  char* start = strstr(buffer, item);
  if (start != nullptr) {
    char* end = strstr(start, " mm");
    char tmp = end[0];
    end[0] = '\0';
    float result = CharToFloat (start + strlen(item));
    end[0] = tmp;
    return result;
  } else {
    return 0.0f;
  }
}

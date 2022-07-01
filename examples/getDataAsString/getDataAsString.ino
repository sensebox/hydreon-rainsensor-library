#include <senseBoxIO.h>

// variables to read and parse serial data
char input;
String rawData;
int startIndex, endIndex;

String acc;      // additional accumulation since last message
String eventAcc; // ?
String totalAcc; // total accumululation. stored on sensor (must be resetted manually)
String rInt;     // rain interval over one hour in mmph (mm/h)

void setup()
{
  SerialUSB.begin(9600);
  while (!SerialUSB)
    ;
  Serial1.begin(9600);
  delay(100);

  // disable external tipping bucket
  Serial1.println('Y');
  delay(100);
  // use metric system
  Serial1.println('M');
  delay(100);
  // set to polling mode
  Serial1.println('P');
  delay(100);
}

void loop()
{
  // Read available data
  Serial1.println('R');

  rawData = "";
  while (Serial1.available() > 0)
  {
    rawData = Serial1.readStringUntil('\n');
    SerialUSB.println(rawData); // uncomment to display raw data

    if (!rawData.startsWith("Acc "))
    {
      return; // if line starts with e.g. ";" there is no measurement data
    }
    else
    {
      startIndex = (rawData.indexOf("Acc  ") + 5);
      endIndex = rawData.indexOf(" mm,");
      acc = rawData.substring(startIndex, endIndex);
      startIndex = (rawData.indexOf("EventAcc ") + 10);
      endIndex = rawData.indexOf(" mm,", endIndex + 1);
      eventAcc = rawData.substring(startIndex, endIndex);
      startIndex = (rawData.indexOf("TotalAcc ") + 10);
      endIndex = rawData.indexOf(" mm,", endIndex + 1);
      totalAcc = rawData.substring(startIndex, endIndex);
      startIndex = (rawData.indexOf("RInt ") + 6);
      endIndex = rawData.indexOf(" m", endIndex + 1);
      rInt = rawData.substring(startIndex, endIndex);

      SerialUSB.print("Acc = ");
      SerialUSB.println(acc);
      SerialUSB.print("EventAcc = ");
      SerialUSB.println(eventAcc);
      SerialUSB.print("TotalAcc = ");
      SerialUSB.println(totalAcc);
      SerialUSB.print("RInt = ");
      SerialUSB.println(rInt);
    }
  }
  delay(1000);
}

#include <Adafruit_GFX.h>      // http://librarymanager/All#Adafruit_GFX_Library
#include <Adafruit_SSD1306.h>  // http://librarymanager/All#Adafruit_SSD1306
#include <JC_Button.h>         // http://librarymanager/All#JC_Button
#include <SPI.h>
#include <Wire.h>
#include <hydreon.h>
#include <senseBoxIO.h>

int pageNumber;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Button button_0(0);
HYDREON rainsensor_Serial2(Serial2);

void printOnDisplay(String title1, String measurement1, String unit1,
                    String title2, String measurement2, String unit2) {
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.println(title1);
  display.setCursor(0, 10);
  display.setTextSize(2);
  display.print(measurement1);
  display.print(" ");
  display.setTextSize(1);
  display.println(unit1);
  display.setCursor(0, 30);
  display.setTextSize(1);
  display.println(title2);
  display.setCursor(0, 40);
  display.setTextSize(2);
  display.print(measurement2);
  display.print(" ");
  display.setTextSize(1);
  display.println(unit2);
}

void setup() {
  senseBoxIO.powerI2C(true);
  delay(2000);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display.display();
  delay(100);
  display.clearDisplay();
  pageNumber = 0;

  button_0.begin();
  rainsensor_Serial2.begin();
}

void loop() {
  button_0.read();
  rainsensor_Serial2.readAllData();
  if (button_0.wasPressed()) {
    pageNumber = ((pageNumber + 1) % 2);
  }

  display.clearDisplay();
  switch (pageNumber) {
    case 0:
      printOnDisplay("total Accumulation",
                     String(rainsensor_Serial2.getTotalAccumulation()), "mm",
                     "Rainfall Intensity",
                     String(rainsensor_Serial2.getRainfallIntensity()), "mm/h");

      break;
    case 1:
      printOnDisplay("Accumulation per Event",
                     String(rainsensor_Serial2.getEventAccumulation()), "mm",
                     "Accumulation since last loop",
                     String(rainsensor_Serial2.getAccumulation()), "mm");

      break;
  }
  display.display();
  delay(100);
}
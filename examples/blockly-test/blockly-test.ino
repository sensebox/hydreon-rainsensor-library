#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>     // http://librarymanager/All#Adafruit_GFX_Library
#include <Adafruit_SSD1306.h> // http://librarymanager/All#Adafruit_SSD1306
#include <senseBoxIO.h>
#include "hydreon.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
HYDREON rainsensor_Serial1(Serial1);

void printOnDisplay(String title1, String measurement1, String unit1, String title2, String measurement2, String unit2)
{

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

void setup()
{
    senseBoxIO.powerI2C(true);
    delay(2000);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.display();
    delay(100);
    display.clearDisplay();
    rainsensor_Serial1.begin();
}

void loop()
{
    rainsensor_Serial1.readAllData();
    printOnDisplay("Acc", String(rainsensor_Serial1.getAccumulation()), "mm", "totAcc", String(rainsensor_Serial1.getTotalAccumulation()), "mm");
    display.display();
}
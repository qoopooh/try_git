#include <TinyGPS++.h>
#include "utilities.h"
#include <Wire.h>
#include "SSD1306Wire.h"

TinyGPSPlus gps;
SSD1306Wire display(0x3c, I2C_SDA, I2C_SCL);
int count = 0;
String gpsLocation;
String gpsTime;

void displayInfo();
void drawOLED();
void getLocation();
void getTimeString();

void setup()
{
    initBoard();

    display.init();
    display.flipScreenVertically();
    drawOLED();

    // When the power is turned on, a delay is required.
    delay(1500);

    Serial.print(F("Testing TinyGPS++ library v. "));
    Serial.println(TinyGPSPlus::libraryVersion());
    Serial.println();
}

void loop()
{
    // This sketch displays information every time a new sentence is correctly encoded.
    while (Serial1.available() > 0) {
        int data = Serial1.read();
        if (gps.encode(data)) {
          displayInfo();
          drawOLED();
          ++count;
        }
        Serial.println(data);
    }

    if (millis() > 5000 && gps.charsProcessed() < 10) {
        Serial.print("count: ");
        Serial.println(count);

        Serial.println(F("No GPS detected: check wiring."));
        while (true);
    }
}

void displayInfo()
{
    Serial.print(F("Location: "));
    if (gps.location.isValid()) {
        Serial.print(gps.location.lat(), 6);
        Serial.print(F(","));
        Serial.print(gps.location.lng(), 6);
    } else {
        Serial.print(F("INVALID "));
        Serial.print(count);
    }

    Serial.print(F("  Date/Time: "));
    if (gps.date.isValid()) {
        Serial.print(gps.date.month());
        Serial.print(F("/"));
        Serial.print(gps.date.day());
        Serial.print(F("/"));
        Serial.print(gps.date.year());
    } else {
        Serial.print(F("INVALID"));
    }

    Serial.print(F(" "));
    if (gps.time.isValid()) {
        if (gps.time.hour() < 10) Serial.print(F("0"));
        Serial.print(gps.time.hour());
        Serial.print(F(":"));
        if (gps.time.minute() < 10) Serial.print(F("0"));
        Serial.print(gps.time.minute());
        Serial.print(F(":"));
        if (gps.time.second() < 10) Serial.print(F("0"));
        Serial.print(gps.time.second());
        Serial.print(F("."));
        if (gps.time.centisecond() < 10) Serial.print(F("0"));
        Serial.print(gps.time.centisecond());
    } else {
        Serial.print(F("INVALID"));
    }

    Serial.println();
}

void drawOLED() {
  display.clear();

  display.setTextAlignment(TEXT_ALIGN_LEFT);

  display.setFont(ArialMT_Plain_10);
  getLocation();
  display.drawString(0, 0, gpsLocation);

  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 10, "Count: " + String(count));

  display.setFont(ArialMT_Plain_24);
  getTimeString();
  display.drawString(0, 26, gpsTime);

  display.display();
}

void getLocation() {
  char buff[12];

  gpsLocation = "";
  dtostrf(gps.location.lat(), 4, 6, buff);
  gpsLocation += buff;

  gpsLocation += ", ";
  dtostrf(gps.location.lng(), 4, 6, buff);
  gpsLocation += buff;
}

void getTimeString() {

  gpsTime = "";

  int hour = gps.time.hour();
  if (hour < 10) gpsTime += "0";
  gpsTime += String(hour);

  gpsTime += ":";
  int minute = gps.time.minute();
  if (minute < 10) gpsTime += "0";
  gpsTime += String(minute);

  gpsTime += ":";
  int second = gps.time.second();
  if (second < 10) gpsTime += "0";
  gpsTime += String(second);
}

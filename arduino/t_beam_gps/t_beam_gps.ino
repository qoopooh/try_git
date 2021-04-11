#include <LoRa.h>
#include "boards.h"

int counter = 0;
float old_lat = 0.0f;

void getLocation(char * buf);

void setup()
{
    initBoard();
    // When the power is turned on, a delay is required.
    delay(1500);

    Serial.println("LoRa Sender");
    LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DI0_PIN);
    if (!LoRa.begin(LoRa_frequency)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
}

void loop()
{
    Serial.print("Sending packet:");
    Serial.println(counter);

    // send packet
    //LoRa.beginPacket();
    //LoRa.print("hello ");
    //LoRa.print(counter);
    //LoRa.endPacket();

#ifdef HAS_DISPLAY
    if (u8g2) {
        char buf[256];
        u8g2->clearBuffer();
        u8g2->drawStr(0, 12, "Transmitting: OK!");
        snprintf(buf, sizeof(buf), "Sending: %d", counter);
        u8g2->drawStr(0, 30, buf);
        u8g2->sendBuffer();
    }
#endif
#ifdef HAS_OLED
    char buf[256];
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);

    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 12, "Transmitting: OK!");

    snprintf(buf, sizeof(buf), "Sending: %d", counter);
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 24, buf);

#ifdef HAS_GPS
    display.setFont(ArialMT_Plain_10);
    getLocation(buf);
    display.drawString(0, 48, buf);

    if (gps.location.lat() == old_lat) {
      snprintf(buf, sizeof(buf), "hello: %d", counter);
    } else {
      old_lat = gps.location.lat();
    }
    loraSend(buf);
#endif

    display.display();
#endif

    counter++;
    Serial.println("Sleep for 2 seconds");
    delay(2000);
}

void getLocation(char * buf) {
#ifdef HAS_GPS
  while (Serial1.available() > 0) {
      if (gps.encode(Serial1.read())) {
          Serial.print("Got GPS ");
          Serial.println(gps.location.lat());
      }
  }
  snprintf(buf, 32, "%f, %f", gps.location.lat(), gps.location.lng());
#endif
}

void loraSend(char * buf) {
    LoRa.beginPacket();
    LoRa.print(buf);
    LoRa.endPacket();
}

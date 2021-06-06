#include <LoRa.h>
#include "boards.h"

char buf[256];
char gps_buff[24];
bool gps_show = false;

void getLocation();

void setup()
{
    initBoard();
    // When the power is turned on, a delay is required.
    delay(1500);

    Serial.println("LoRa Receiver");

    LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DI0_PIN);
    if (!LoRa.begin(LoRa_frequency)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
}

void loop()
{
    // try to parse packet
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        // received a packet
        Serial.print("Received packet '");

        String recv = "";
        // read packet
        while (LoRa.available()) {
            recv += (char)LoRa.read();
        }

        Serial.println(recv);

        // print RSSI of packet
        Serial.print("' with RSSI ");
        Serial.println(LoRa.packetRssi());
#ifdef HAS_DISPLAY
        if (u8g2) {
            u8g2->clearBuffer();
            u8g2->drawStr(0, 12, "Received OK!");
            u8g2->drawStr(0, 26, recv.c_str());
            snprintf(buf, sizeof(buf), "RSSI:%i", LoRa.packetRssi());
            u8g2->drawStr(0, 40, buf);
            snprintf(buf, sizeof(buf), "SNR:%.1f", LoRa.packetSnr());
            u8g2->drawStr(0, 56, buf);
            u8g2->sendBuffer();
        }
#endif

#ifdef HAS_OLED
        char buf[256];
        display.clear();
        display.setTextAlignment(TEXT_ALIGN_LEFT);

        display.setFont(ArialMT_Plain_10);
        display.drawString(0, 0, "LoRa receiver:");
        display.drawString(0, 11, recv.c_str());

        snprintf(buf, sizeof(buf), "RSSI:%i", LoRa.packetRssi());
        display.drawString(0, 24, buf);

        snprintf(buf, sizeof(buf), "SNR:%.1f", LoRa.packetSnr());
        display.drawString(0, 35, buf);

        if (gps_show) {
            display.drawString(0, 48, gps_buff);
        }

        display.display();
#endif
    } // packetSize
#ifdef HAS_GPS
    getLocation();
#endif
}

void getLocation() {
#ifdef HAS_GPS
    while (Serial1.available() > 0) {
        if (gps.encode(Serial1.read())) {
            snprintf(gps_buff, sizeof(gps_buff), "%f, %f",
                gps.location.lat(),
                gps.location.lng());
            Serial.println(gps_buff);
            gps_show = true;
        }
    }
#endif
}

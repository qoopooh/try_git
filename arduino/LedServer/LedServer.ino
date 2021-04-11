#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "credentials.h"
#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#define STASSID "your-ssid"
#define STAPSK  "your-password"

#endif

#define PORT 8000

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(PORT);

const int led = LED_BUILTIN;
const int RELAY = 0;    // for ESP01
int led_status = 0;

void handleRoot() {

  if (server.method() == HTTP_POST) {
    for (uint8_t i = 0; i < server.args(); i++) {
      if (server.argName(i) == "led") {
        String req = server.arg(i);
        req.toLowerCase();
        if (req == "on") {
          digitalWrite(led, 0);
          digitalWrite(RELAY, 0);
          led_status = 1;
        } else {
          digitalWrite(led, 1);
          digitalWrite(RELAY, 1);
          led_status = 0;
        }
      }
    }
  }

  server.send(200, "text/plain", led_status == 0 ? "OFF":"ON");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  pinMode(led, OUTPUT);
  pinMode(RELAY, OUTPUT);
  digitalWrite(led, 0);
  digitalWrite(RELAY, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.print("LED_BUILTIN ");
  Serial.println(led);

  // Wait for connection
  Serial.print("Connecting to ");
  Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  digitalWrite(led, 1);
  digitalWrite(RELAY, 1);
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}

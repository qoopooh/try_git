/*
 * ESP32-C3 Super Mini - Board Test
 *
 * Tests:
 *  1. Serial output
 *  2. Built-in LED blink (GPIO 8 on Super Mini)
 *  3. WiFi scan
 *
 * FQBN: esp32:esp32:nologo_esp32c3_super_mini
 * Port: /dev/ttyACM0
 *
 * Upload:
 *   arduino-cli compile --fqbn esp32:esp32:nologo_esp32c3_super_mini .
 *   arduino-cli upload  --fqbn esp32:esp32:nologo_esp32c3_super_mini --port /dev/ttyACM0 .
 *   arduino-cli monitor --port /dev/ttyACM0 --config baudrate=115200
 */

#include <WiFi.h>

// The Super Mini has a blue LED on GPIO 8 (active HIGH)
static const int LED_PIN = 8;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println("\n=== ESP32-C3 Super Mini Board Test ===");

  // --- LED test ---
  pinMode(LED_PIN, OUTPUT);
  Serial.println("[LED] Blinking 3 times...");
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
  Serial.println("[LED] OK");

  // --- Chip info ---
  Serial.printf("[CHIP] Model : %s\n", ESP.getChipModel());
  Serial.printf("[CHIP] Cores : %d\n", ESP.getChipCores());
  Serial.printf("[CHIP] Rev   : %d\n", ESP.getChipRevision());
  Serial.printf("[CHIP] Flash : %u KB\n", ESP.getFlashChipSize() / 1024);
  Serial.printf("[CHIP] Free heap: %u bytes\n", ESP.getFreeHeap());

  // --- WiFi scan ---
  Serial.println("[WiFi] Scanning...");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println("[WiFi] No networks found");
  } else {
    Serial.printf("[WiFi] Found %d network(s):\n", n);
    for (int i = 0; i < n; i++) {
      Serial.printf("  %2d. %-32s  RSSI %4d dBm  ch %2d  %s\n",
        i + 1,
        WiFi.SSID(i).c_str(),
        WiFi.RSSI(i),
        WiFi.channel(i),
        WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "open" : "secured");
    }
  }
  WiFi.scanDelete();
  Serial.println("[WiFi] OK");

  Serial.println("=== Setup complete. LED will blink in loop. ===\n");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(950);
  digitalWrite(LED_PIN, LOW);
  delay(50);
}

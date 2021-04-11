/*
Blink with sleeping for esp32
*/

// reversed LED state
#define HIGH 0
#define LOW  1

//const int LED = LED_BUILTIN;
const int LED = 4;
const int LOOP_COUNT_LIMIT = 10;
const int WAKE_UP_PIN = 38;   // TTGO T-Beam button
const int k_delay = 500;      // unit: millisecond

RTC_DATA_ATTR int bootCount = 0; // save value in RTC memory (8K max.)
int loopCount = 0;

void go_to_sleep();
void print_wakeup_reason();

void setup() {
  // initialize the digital pin as an output.
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(WAKE_UP_PIN, INPUT_PULLUP);

  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  print_wakeup_reason();
}

void loop() {
  digitalWrite(LED, HIGH);    // set the LED on
  delay(k_delay);
  digitalWrite(LED, LOW);     // set the LED off
  delay(k_delay);

  ++loopCount;
  if (loopCount >= LOOP_COUNT_LIMIT) {
    go_to_sleep();
  }
}

void go_to_sleep() {
  digitalWrite(LED, LOW);
  delay(k_delay);

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_38, 0); //1 = High, 0 = Low

  Serial.println("Going to sleep now");
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}


void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

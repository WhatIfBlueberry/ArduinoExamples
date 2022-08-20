#include <Arduino.h>

#define POWER_LED 11

void setup() {
  pinMode(POWER_LED, OUTPUT);
  digitalWrite(POWER_LED, HIGH);
}

void loop() {
  digitalWrite(POWER_LED, LOW);
  delay(1000);
  digitalWrite(POWER_LED, HIGH);
  delay(1000);
}
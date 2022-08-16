#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  pinMode(8, INPUT);
}

/* to configure distance turn philipsscrew on the sensor */
void loop() {
  int detect = digitalRead(8);
  if (detect == LOW) {
    Serial.println("Obstacle in the way");
  } else {
    Serial.println("All clear!");
  }
  delay(300);
}
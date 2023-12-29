#include <Arduino.h>
#include <TM1637Display.h>

#define WET_THRESHOLD 190
#define DRY_THRESHOLD 510

// define the pins used by the LED display
#define CLK 2
#define DIO 3

// display include needs library import.
// PlatformIO -> lib -> search for TM1637 -> install
TM1637Display display(CLK, DIO);

void setup() {
  pinMode(A0, INPUT);
  display.setBrightness(0); // Set the initial brightness to the lowest level
  Serial.begin(9600);
}


void loop() {
  int sensorValue = analogRead(A0);
  int moisture = map(sensorValue, DRY_THRESHOLD, WET_THRESHOLD, 0, 100);
  display.setBrightness(1);
  display.showNumberDec(moisture, false);
  delay(300);
}



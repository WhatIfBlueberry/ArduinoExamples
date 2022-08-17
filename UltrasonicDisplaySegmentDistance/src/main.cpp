#include <Arduino.h>
#include <Ultrasonic.h>
#include <TM1637Display.h>
// ULTRASONIC

#define CLK 2
#define DIO 3

#define TRIG 12
#define ECHO 13
Ultrasonic ultrasonic(TRIG, ECHO);
TM1637Display display(CLK, DIO);

void setup() {
  Serial.begin(9600);
  display.setBrightness(0xff);
}

void loop() {
  int distance = ultrasonic.read();
  Serial.print("Sensor distance: ");
  Serial.print(distance); // Prints the distance on the default unit (centimeters)
  Serial.println("cm");
  display.showNumberDec(distance, false);
  delay(100);
}

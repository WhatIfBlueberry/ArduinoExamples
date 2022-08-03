#include <Arduino.h>
#include <TM1637Display.h>

#define PIN_SLIDE_POT_A A0
#define CLK 2
#define DIO 3

TM1637Display display(CLK, DIO);


/* 
  Simple program that displays the value of a pontentiometer on a 4-digit-diplay
  The values get mapped to each units maximum. The analog potentiometer gets values (0,1023)
  while the Display can Display (0,9999). A maxed out potentiometer (1023) will show (9999) on the Display.
  
  Author: Dylan Rau 08.2022
 */

void setup() {
  Serial.begin(9600);
  pinMode(PIN_SLIDE_POT_A, INPUT);
  display.setBrightness(0xff);
}

void loop() {
  int value_slide_pot_a = analogRead(PIN_SLIDE_POT_A);
  Serial.print("Slide Pot value: ");
  Serial.println(value_slide_pot_a);
  int mappedValueForDisplay = map(value_slide_pot_a, 0, 1023, 0, 9999);
  Serial.print("mapped value: ");
  Serial.println(mappedValueForDisplay);
  display.showNumberDec(mappedValueForDisplay, false);
  delay(20);
}
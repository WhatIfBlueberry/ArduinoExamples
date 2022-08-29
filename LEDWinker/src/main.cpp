#include <Arduino.h>
#include <Servo.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* Added Sweeping Comtraya!
Need to comment out one of the .cpp files in order to work */

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int textSize = 4;

Servo myservo; // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int potpin = 0; // analog pin used to connect the potentiometer
int val;        // variable to read the value from the analog pin

int inverseSpeed = 0;

int getInverseSpeed()
{
  val = analogRead(potpin); // reads the value of the potentiometer (value between 0 and 1023)
  inverseSpeed = map(val, 0, 1023, 1000, 10);
  return inverseSpeed;
}

void displayPowerLevel()
{
  int powerLevel = map(val, 0, 1023, 0, 100);
  Serial.println(powerLevel);
  display.clearDisplay();
  display.setTextSize(textSize);
  display.setCursor(0, 0);
  display.print(powerLevel);
  display.display();
}

void setup()
{
  Serial.begin(9600);
  Serial.println("This firmware was uploaded from " + String(UPLOAD_PORT));
  Serial.println("At " + String(UPLOAD_SPEED) + " baud.");
  Serial.println("Monitor speed was " + String(MONITOR_SPEED) + " baud.");
  myservo.attach(9); // attaches the servo on pin 9 to the servo object
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.setTextSize(textSize);
  display.setTextColor(WHITE);
  display.setTextWrap(false);
}

void loop()
{
  for (pos = 0; pos <= 180; pos += 10)
  { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);       // tell servo to go to position in variable 'pos'
    delay(getInverseSpeed()); // waits 15ms for the servo to reach the position
    displayPowerLevel();
  }
  Serial.println("DoneLEFT");
  for (pos = 180; pos >= 0; pos -= 10)
  {                           // goes from 180 degrees to 0 degrees
    myservo.write(pos);       // tell servo to go to position in variable 'pos'
    delay(getInverseSpeed()); // waits 15ms for the servo to reach the position
    displayPowerLevel();
  }
  Serial.println("DoneRIGHT");
}
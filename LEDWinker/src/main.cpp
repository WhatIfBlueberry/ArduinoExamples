#include <Arduino.h>
#include <Servo.h>

Servo myservo; // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int potpin = 0; // analog pin used to connect the potentiometer
int val;        // variable to read the value from the analog pin

int inverseSpeed = 0;

const char message[] = "Delay in Seconds: ";

int getInverseSpeed()
{
  val = analogRead(potpin); // reads the value of the potentiometer (value between 0 and 1023)
  inverseSpeed = map(val, 0, 1023, 1000, 10);
  return inverseSpeed;
}

void setup()
{
  Serial.begin(9600);
  Serial.println("This firmware was uploaded from " + String(UPLOAD_PORT));
  Serial.println("At " + String(UPLOAD_SPEED) + " baud.");
  Serial.println("Monitor speed was " + String(MONITOR_SPEED) + " baud.");
  myservo.attach(9); // attaches the servo on pin 9 to the servo object
}

void loop()
{
  for (pos = 0; pos <= 180; pos += 10)
  { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);       // tell servo to go to position in variable 'pos'
    delay(getInverseSpeed()); // waits 15ms for the servo to reach the position
    Serial.println(map(val, 0, 1023, 0, 100));
  }
  Serial.println("DoneLEFT");
  for (pos = 180; pos >= 0; pos -= 10)
  {                           // goes from 180 degrees to 0 degrees
    myservo.write(pos);       // tell servo to go to position in variable 'pos'
    delay(getInverseSpeed()); // waits 15ms for the servo to reach the position
    Serial.println(map(val, 0, 1023, 0, 100));
  }
  Serial.println("DoneRIGHT");
}
#include <Arduino.h>
#include <WS2812FX.h>

#include "MeanFilterLib.h"
#include "Timer.h"

// ULTRASONIC
#define PIN_ECHO 2
#define PIN_TRIG 3

// POTENTIO
#define PIN_SLIDE_POT_A A0
#define MAX_SLIDE_POT_ANALGOG_READ_VALUE 740

// VOLLGASBUTTON
#define PIN_STARTUP 6

// LED
#define NUM_LEDS 60                 // add number of LEDs of your RGB LED strip
#define PIN_LED 5                   // digital output PIN that is connected to DIN of the RGB LED strip
#define LED_COLOR CRGB::DarkOrchid  // see https://github.com/FastLED/FastLED/wiki/Pixel-reference for a full list, e.g. CRGB::AliceBlue, CRGB::Amethyst, CRGB::AntiqueWhite...

// CONTROL
#define PIN_CONTROL 9

// FLAG
#define PIN_FLAG 11

// LED STRIP
#define SHOULD_SIZE 30

MeanFilter<long> meanFilter(10);

// WS2812FX ws2812fx(NUM_LEDS, PIN_LED, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN_LED, NEO_GRB + NEO_KHZ800);

void setup() {
    Serial.begin(9600);
    pinMode(PIN_FLAG, OUTPUT);
    pinMode(PIN_STARTUP, INPUT);
    pinMode(PIN_SLIDE_POT_A, INPUT);
    pinMode(PIN_CONTROL, OUTPUT);
    pinMode(PIN_TRIG, OUTPUT);  // Sets the PIN_TRIG as an OUTPUT
    pinMode(PIN_ECHO, INPUT);   // Sets the PIN_ECHO as an INPUT

    randomSeed(analogRead(A1));

    strip.begin();
    strip.setBrightness(42);
    strip.setPixelColor(10, strip.Color(255, 0, 0));
    strip.show();
}

#define DISTANCE_COUNT 10

// #### SHARED VARIABLES
uint8_t distance;  // in cm
int value_slide_pot_a;
enum GameMode {
    SANDBOX = 0,
    FLAPPYBIRD = 1,
};
GameMode gameMode = SANDBOX;

// #### SHARED FUNCTIONS
int distanceAdjusted() {
    if (distance > 50) {
        if (distance > 100) {
            return 15;
        } else {
            return 50;
        }
    } else {
        return distance;
    }
}

// #### LOOP
void loop() {
    int value = digitalRead(PIN_STARTUP);
    if (value == LOW) {
        return;  // todo nen pin zu RESET legen und hier auf LOW (glaub ich) setzen
    }

    // FLAG at constant speed and startup sequence
    static Timer startTimer(50);  // delay before startup
    if (startTimer.tick()) {
        analogWrite(PIN_FLAG, 50);
    }

    // VARIABLES CALCULATION
    value_slide_pot_a = analogRead(PIN_SLIDE_POT_A);
    gameMode = value_slide_pot_a > MAX_SLIDE_POT_ANALGOG_READ_VALUE / 2 ? SANDBOX : FLAPPYBIRD;

    // ULTRA SONIC SENSOR
    static Timer ultraSonicTimer(100);
    if (ultraSonicTimer.tick()) {
        enum ultraSonicState {
            STARTING,
            TRIGGER,
            ECHO,
        };
        static ultraSonicState state = STARTING;

        switch (state) {
            case TRIGGER:  // start
                digitalWrite(PIN_TRIG, HIGH);
                ultraSonicTimer.start(10);
                state = ECHO;
                break;

            case ECHO:  // measure
                static long dauer, distNow, mean;
                digitalWrite(PIN_TRIG, LOW);
                dauer = pulseIn(PIN_ECHO, HIGH);
                distNow = (dauer / 2) * 0.03432;
                mean = meanFilter.AddValue(distNow);
                distance = constrain(mean, 0, 255);

            case STARTING:  // start new cycle
                digitalWrite(PIN_TRIG, LOW);
                ultraSonicTimer.start(5);
                state = TRIGGER;
                break;
        }
    }

    // POWER CONTROL
    static Timer powerControl(50);
    if (powerControl.tick()) {
        // SET POWER-LEVEL
        static long powerLevel;
        powerLevel = map(distanceAdjusted(), 0, 50, 42, 255);
        powerLevel = constrain(powerLevel, 42, 255);
        analogWrite(PIN_CONTROL, powerLevel);
    }

    static Timer flagControl(50);
    if (flagControl.tick()) {
        // SET FLAG-SPEED
        analogWrite(PIN_FLAG, 5);
    }
    // asso ja bitte gamemode 1 mit blau
    //  LED CONTROL
    static int shouldStart = 0, shouldSize = SHOULD_SIZE;
    // ja die LEDS brauch ich, die 5V sind immer an bis später ^^mach ich :)
    enum movementEnum {
        STOP = 0,
        UP = 1,
        UPFAST = 2,
        DOWN = -1,
        DOWNFAST = -2,
    };
    static movementEnum movement = STOP;

    static Timer randTimer(1000);

    static Timer movementTimer(100);

    if (randTimer.tick()) {
        if (gameMode == FLAPPYBIRD) {
            // generate random int
            static int randomInt;
            randomInt = random(-2, 3);
            movement = (movementEnum)randomInt;

            // set movement speed
            if (movement == DOWNFAST || movement == UPFAST) {
                movementTimer.setInterval(100);
            } else {
                movementTimer.setInterval(200);
            }

            // decrease shouldSize
            randomInt = random(0, 4);
            if (randomInt == 0) {
                shouldSize--;
                shouldSize = max(shouldSize, 10);
            }

            // set next interval
            if (movement == DOWNFAST || movement == UPFAST) {
                randomInt = random(0, 1500);
            } else {
                randomInt = random(0, 200);
            }
            randTimer.start(500 + randomInt);
        } else {
            movement = STOP;
            shouldSize = SHOULD_SIZE;
            randTimer.start(5000);
        }
    }

    if (movementTimer.tick()) {
        shouldStart = shouldStart + (movement > 0 ? 1 : (movement < 0 ? -1 : 0));
        shouldStart = constrain(shouldStart, 12, NUM_LEDS - shouldSize);
    }

    static Timer ledControl(20);
    if (ledControl.tick()) {
        // SET LED-COUNT
        int powerLevel = map(distanceAdjusted(), 0, 50, 0, NUM_LEDS);
        powerLevel = constrain(powerLevel, 0, NUM_LEDS);

        for (int i = 0; i < NUM_LEDS; ++i) {
            strip.setPixelColor(i, 0);
            switch (gameMode) {
                case SANDBOX:
                    // power level
                    if (i < powerLevel) {
                        strip.setPixelColor(i, strip.Color(0, 40, 64));
                    }
                    break;
                case FLAPPYBIRD:
                    // should area
                    if (i >= shouldStart && i < shouldStart + shouldSize) {
                        strip.setPixelColor(i, strip.Color(0, 0, 64));
                    }
                    break;
            }
        }

        strip.show();
    }

    // DISPLAY AREA
    static Timer dislpay(500);
    if (dislpay.tick()) {
        Serial.print("GM: ");
        Serial.print(gameMode);
        Serial.print(" | Distance: ");
        Serial.print(distance);
        Serial.print(" cm");
        Serial.print(" | movement: ");
        Serial.print(movement);
        Serial.print(" | size: ");
        Serial.print(shouldSize);
        Serial.println();
    }
}

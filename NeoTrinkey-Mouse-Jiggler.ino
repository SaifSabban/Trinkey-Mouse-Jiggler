#include <Adafruit_NeoPixel.h> // Imports the Neopixel library
#include "Adafruit_FreeTouch.h" // Imports the Capacitive Touch library
#include <Mouse.h> // Imports the Mouse library

Adafruit_NeoPixel Square = Adafruit_NeoPixel(NUM_NEOPIXEL, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800); // Creates Neopixel Instance

Adafruit_FreeTouch Touch1 = Adafruit_FreeTouch(1, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE); // Creates Right Touch Instance
Adafruit_FreeTouch Touch2 = Adafruit_FreeTouch(2, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE); // Creates Left Touch Instance

void setup() { // The Set-up loop. It runs once at the start of the code
  Touch1.begin(); // Activates Right Touch
  Touch2.begin(); // Activates Left Touch

  delay(100); // Gives a delay at the start of the code to help with ramdom seed

  Square.begin(); // Activates Neopixel
  Square.setBrightness(100); // Sets Brightness To less than half
  Square.show(); // Clears pixels

  randomSeed(analogRead(0)); // Reads the value from an unused analog pin to seed the sudo random function
  Mouse.begin(); // Function that starts the mouse emulation
}

void loop() { // The main loop of you code that will run indefinitely
  bool ActiveateMouse = TouchButton(); // State to activate mouse jiggler

  if (ActiveateMouse) {
    MouseJiggler(30000); // The mouse jiggler function
  }
}

bool TouchButton() {
  uint16_t touch1 = Touch1.measure(); // Measures Right Touch
  uint16_t touch2 = Touch2.measure(); // Measures Left Touch

  static bool State = 1; // Set the current state to high

  static bool isTouching; // Checks when a touch occurs
  static bool LastTouchingState; // last value of touch
  static bool TouchToggle; // acitivates touch after debounce

  static unsigned long lastDebounceTime = 0; // Debounce timer to stop false touch

  if ((touch1 > 500) || (touch2 > 500)) isTouching = 1; // If touching set to istouching 1
  else isTouching = 0; // If not touching set to istouching 0


  if (isTouching != LastTouchingState) { // is senses a touch
    lastDebounceTime = millis(); // Set lastdebounce timer to current time
  }

  if ((millis() - lastDebounceTime) > 50) { // If Debounce timer passes 50 milliseconds
    if (isTouching != TouchToggle) { // If the touch avlue and toggle value are not the same
      TouchToggle = isTouching; // Sets the toggle value to that of actual touch value

      if (TouchToggle == HIGH) { // If debounced and touching
        State = !State; // Change stat to what it isn't
      }
    }
  }

  if (TouchToggle) { // If touching sensors
    if (State) { // If State is 1
      Square.fill(Square.Color(0, 0, 255)); // Set Neopixeel colour to Blue
      Square.show(); // Tell Neopixel to show set colour
    }
    else { // If State is 0
      Square.fill(Square.Color(255, 0, 0)); // Set Neopixeel colour to Red
      Square.show(); // Tell Neopixel to show set colour
    }
  }
  else if (!TouchToggle) { // If not touching sensors
    Square.fill(Square.Color(0, 0, 0)); // Set Neopixeel colour to off
    Square.show(); // Tell Neopixel to show set colour
  }

  LastTouchingState = isTouching; // Sets the Previous Touch Value to the current value
  return State; // Returns State Value
}

void MouseJiggler(unsigned int Function_Interval) { // The mouse jiggler function, uses and input for the mean time interval
  static unsigned long interval_Timer; // The is an unsigned long that will remember the last time the if statement was run
  static int Jiggle_interval; // The is an in that will keep the randomised Jiggle_interval in the function

  if ((millis() - interval_Timer) >= Jiggle_interval) { // The main if statement that check if the interval between last running the function, and the current time, has excceded the jiggle interval
    int distance = random(10, 800); // Gives a random distance for the mouse to move
    int x = random(3) - 1; // Gives a random x-axis directionality
    int y = random(3) - 1; // Gives a random y-axis directionality
    for (int i = 0; i < distance; i++) { // a for loop that will make the mouse move a random interval
      Mouse.move(x, y, 0); // Function that moves the mouse
      delay(3); // A 3 milli second delay to give time for the computer to react
    }
    Jiggle_interval = random(Function_Interval - (Function_Interval / 2), Function_Interval + (Function_Interval / 2)); // Sets a new random Jiggle_interval that is +- 50% of the Function_Interval
    interval_Timer = millis(); // Sets the interval_Timer to th current time at the end of the if statement
  }
}

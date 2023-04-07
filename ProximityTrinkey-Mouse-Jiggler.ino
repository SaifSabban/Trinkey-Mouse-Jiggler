#include "Adafruit_APDS9960.h" // Imports the Adafruit_APDS9960 library
#include "Adafruit_FreeTouch.h" // Imports the Capacitive Touch library
#include <Adafruit_NeoPixel.h> // Imports the Neopixel library
#include <Mouse.h> // Imports the Mouse library
#include <Keyboard.h> // Imports the Keyboard library

Adafruit_APDS9960 APDS_Sensor; // Creates APDS9960 called apds

Adafruit_NeoPixel Square = Adafruit_NeoPixel(2, 3, NEO_GRB + NEO_KHZ800); // Creates Neopixel Instance

Adafruit_FreeTouch Touch1 = Adafruit_FreeTouch(1, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE); // Creates Right Touch Instance
Adafruit_FreeTouch Touch2 = Adafruit_FreeTouch(2, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE); // Creates Left Touch Instance

void setup() { // The Set-up loop. It runs once at the start of the code
  delay(100); // Gives a delay at the start of the code to help with ramdom seed

  if ((Touch1.begin()) && (Touch2.begin()))  Mouse.begin(); // Function that starts the mouse emulation if touch sensors are working
  if (!APDS_Sensor.begin()) Keyboard.begin(); // Function that starts the Keyboard emulation if APDS9960 sensors is working

  Square.begin(); // Activates Neopixel
  Square.setBrightness(75); // Sets Brightness To less than half
  Square.show(); // Clears pixels

  randomSeed(analogRead(0)); // Reads the value from an unused analog pin to seed the sudo random function

  //gesture mode will be entered once proximity mode senses something close
  APDS_Sensor.enableProximity(true); // Set APDS9960 to Prpximity mode
  APDS_Sensor.enableGesture(true); // Set APDS9960 to Gesture mode
}

void loop() { // The main loop of you code that will run indefinitely
  bool ActiveateMouse = TouchButton(); // State to activate mouse jiggler
  if (ActiveateMouse) MouseJiggler(30000); // The mouse jiggler function
  GestureCheck(ActiveateMouse); // The Gesture function
}

void GestureCheck(bool Light) { // The Gesture Check function, uses and input from APDS9960 to perform keyborad functions
  //  char ctrlKey = KEY_LEFT_GUI; // Control Key option for OSX.
  char ctrlKey = KEY_LEFT_CTRL; // Control Key option for Windows and Linux.
  static uint8_t LastGesture; // Last Gesture variable
  static unsigned long DelayTimer = 0; // Delay timer to play Light

  uint8_t Gesture = APDS_Sensor.readGesture(); // Reads Gesture being made

  if (Gesture == APDS9960_DOWN) {
    Keyboard.press(ctrlKey);
    Keyboard.press('v');
    Keyboard.releaseAll();
  }
  else if (Gesture == APDS9960_UP) {
    Keyboard.press(ctrlKey);
    Keyboard.press('c');
    Keyboard.releaseAll();
  }

  if (Light) {
    if (LastGesture != Gesture) {
      DelayTimer = millis(); // Set lastdebounce timer to current time
      LastGesture = Gesture; // Current Gesture is set as last gesture
    }

    if ((millis() - DelayTimer) < 200) { // If delay timer less than 150 milliseconds
      if (Gesture == APDS9960_DOWN) Square.fill(Square.Color(0, 125, 255)); // Set Neopixel colour to Blue
      else if (Gesture == APDS9960_UP) Square.fill(Square.Color(125, 255, 0)); // Set Neopixel colour to Green
      else if (Gesture == APDS9960_LEFT) Square.fill(Square.Color(255, 0, 0)); // Set Neopixel colour to Red
      else if (Gesture == APDS9960_RIGHT) Square.fill(Square.Color(255, 255, 255)); // Set Neopixel colour to white
      Square.show(); // Tell Neopixel to show set colour
    }
    else if (((millis() - DelayTimer) >= 200) &&  ((millis() - DelayTimer) < 300)) {
      Square.fill(Square.Color(0, 0, 0)); // Set Neopixel colour to off
      Square.show(); // Tell Neopixel to show set colour
    }
  }
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

bool TouchButton() { // The touch button function, treats the touch sensors as momentary buttons
  uint16_t TouchSensor1 = Touch1.measure(); // Reads Value from sensor
  uint16_t TouchSensor2 = Touch2.measure(); // Reads Value from sensor

  static bool State; // Set the current state to high

  static bool isTouching; // Checks when a touch occurs
  static bool LastTouchingState; // last value of touch
  static bool TouchToggle; // acitivates touch after debounce

  static unsigned long lastDebounceTime = 0; // Debounce timer to stop false touch

  if ((TouchSensor1 > 400) || (TouchSensor2 > 400)) isTouching = 1; // If touching set to istouching 1
  else isTouching = 0; // If not touching set to istouching 0

  if (isTouching != LastTouchingState) lastDebounceTime = millis(); // Set lastdebounce timer to current time if it senses a touch

  if ((millis() - lastDebounceTime) > 50) { // If Debounce timer passes 50 milliseconds
    if (isTouching != TouchToggle) { // If the touch avlue and toggle value are not the same
      TouchToggle = isTouching; // Sets the toggle value to that of actual touch value

      if (TouchToggle) { // If debounced and touching
        State = !State; // Change stat to what it isn't

        if (State) { // If State is 1
          Square.fill(Square.Color(0, 0, 255)); // Set Neopixel colour to Blue
          Square.show(); // Tell Neopixel to show set colour
        } else { // If State is 0
          Square.fill(Square.Color(255, 0, 0)); // Set Neopixel colour to Red
          Square.show(); // Tell Neopixel to show set colour
        }
      } else { // If not touching sensors
        Square.fill(Square.Color(0, 0, 0)); // Set Neopixel colour to off
        Square.show(); // Tell Neopixel to show set colour
      }
    }
  }

  LastTouchingState = isTouching; // Sets the Previous Touch Value to the current value
  return State; // Returns State Value
}

/* This sample code by Saif Sabban demonstrates a basic use of the Mouse library.
   This code will randomly move your mouse cursor at semi-random intervals.
   It only uses a single function, s should be easy to implement in almost any existing code.
*/
#include <Mouse.h> // Imports the Mouse library

void setup() { // The Set-up loop. It runs once at the start of the code
  delay(100); // Gives a delay at the start of the code to help with ramdom seed
  randomSeed(analogRead(0)); // Reads the value from an unused analog pin to seed the sudo random funtion
  Mouse.begin(); // Function that starts the mouse emulation
}

void loop() { // The main loop of you code that will run indefinitely
  MouseJiggler(30000); // The mouse jiggler function
}

void MouseJiggler(unsigned int Function_Interval) { // The mouse jiggler function, uses and input for the mean time interval
  static unsigned long interval_Timer; // The is an unsigend long that will remember the last time the if statment was run
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
    interval_Timer = millis(); // Sets the interval_Timer to th current time at the end of the if statment
  }
}

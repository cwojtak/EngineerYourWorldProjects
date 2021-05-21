//Connor Wojtak
//5-6-2021
//rangefinder_calibration_tester
//A program to test rangefinder calibration subroutines.

#include <EYW.h>
#include "Arduino.h"

EYW::RangeFinder theRangeFinder;

int currentRange = 0;

void setup() {
  Serial.begin(9600);
  theRangeFinder.begin(); //Initialize the range finder.
  boolean success = false; //Creates a variable to determine if calibration successfully occured.
  for(int i = 0; i < 10; i++){ //Attempts calibration up to maxCalibrationAttempts times.
    if(theRangeFinder.calibrate(10) == true){ //Calibrates the range finder.
      success = true; //If calibration was successful, the program continues on in the calibration sequence.
      break;
    }
    Serial.print(F("\nRange finder calibration failed. Retrying...")); //Calibration failed.
    delay(100); //Waits 100 milliseconds before trying again.
  }
  if(success == false){ //Checks to see if all calibration attempts failed.
    Serial.print(F("\nDANGER: The range finder's calibration has failed. Please reset the Arduino.")); //All calibration attempts failed.
  } else {
    Serial.print(F("\nSUCCESS: The range finder has been calibrated successfully.")); //Success. Plays 3 calibration notes.
    return;
  }
}

void loop() {
  delay(1000);
  currentRange = theRangeFinder.getDistance();
  Serial.println(currentRange);
}

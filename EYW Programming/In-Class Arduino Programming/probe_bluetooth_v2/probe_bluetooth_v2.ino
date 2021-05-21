//Connor Wojtak
//3-31-2021
//probe_bluetooth_v2
//A probe program to remotely control and receive data from a probe.
//This code is repurposed from:
/*
    Jesse DeWald 
    November 18, 2014
    The University of Texas at Austin
    UTeachEngineering - Engineer Your World
    This code is available in the Public Domain
*/


//Imports
#include "Arduino.h"
#include <Servo.h>
#include "Bluetooth_vars.h"
#include "Bluetooth_func.h"

/*================================================== BLUETOOTH DEFINTIONS ==================================================*/
//A long value used for timers.
long timerTime = 0;
/*================================================== END BLUETOOTH DEFINTIONS ==================================================*/

/*================================================== SHARED DEFINTIONS ==================================================*/
//Probe State Definition
String PROBE_STATE = "CALIBRATION"; //Identifies what state the probe is in.
                                    //Possible values include CALIBRATION, ASCENT, DESCENT, LANDING, and PANIC.
/*================================================== END SHARED DEFINTIONS ==================================================*/

//These commands run on startup.
void setup() 
{ 
  startBT();            //Starts communication with the bluetooth module.
  setServo();           //Sets the servo to the default settings and postiion.
  PROBE_STATE = "ASCENT"; //Sets the probe state to ascent when calibration is complete.
} 
 
//These commands will loop forever.
void loop()
{
  //updateAltimeter();
  //updateRangeFinder();
  //COMMAND PARSER
  if (Serial.available())   //Checks for sent commands.
  {
    processCommand();       //Processes the command.
  }
  //STATE ACTIONS
  if((PROBE_STATE.compareTo("ASCENT") == 0 || PROBE_STATE.compareTo("DESCENT") == 0) && verboseState == true){ //Checks to see if telemetry should be outputted.
      if(millis() - timerTime > 5000){ //Checks to see if a second has past.
          timerTime = millis(); //Resets the timer.
          Serial.print("\nT+"); //The next lines print out telemetry.
          Serial.print(millis() / 1000);
          Serial.print(" seconds\n\tHeight = ");
          Serial.print("TODO: ADD HEIGHT");
          Serial.print(" m\n\t Instantaneous Velocity = ");
          Serial.print("TODO: ADD VELOCITY");
          Serial.print(" m/s\n");
      }
  }
  if(PROBE_STATE.compareTo("DESCENT") == 0 || PROBE_STATE.compareTo("PANIC") == 0){ //Checks to see if the probe is descending or panicking so that the camera takes pictures.
      //updateCamera();
  }
  //STATE SETTERS
  if(PROBE_STATE.compareTo("ASCENT") == 0 && launchedFlag == true){ //Checks to see if the probe has changed from ascending to descending.
      PROBE_STATE = "DESCENT";
  }
  /*
  if(rangeSensorTriggered == true && PROBE_STATE.compareTo("DESCENT") == 0){
      PROBE_STATE = "LANDING";
  }
  if(PROBE_STATE.compareTo("PANIC") != 0 && (position < minHeight || position > maxHeight || iVelocity > maxVelocity || iVelocity < minVelocity)){ //Checks to see if there is a reason for the probe to panic and take as many pictures as possible.
      PROBE_STATE = "PANIC";
      Serial.print("\nPANIC: Position or instantaneous velocity out of bounds!");
  }
  */
}

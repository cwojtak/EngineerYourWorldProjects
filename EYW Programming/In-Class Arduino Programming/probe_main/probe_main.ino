//Connor Wojtak
//4-14-2021
//probe_main
//The main probe program for all subsystems.
//This code is repurposed from:
/*
    Bluetooth_code_v2_0
    Jesse DeWald
    November 18, 2014
    The University of Texas at Austin
    UTeachEngineering - Engineer Your World
    This code is available in the Public Domain
*/

//Imports
#include <EYW.h>
#include "Arduino.h"
#include <Servo.h>
#include "Bluetooth_vars.h"
#include "Bluetooth_func.h"

/*================================================== SHARED DEFINITIONS ==================================================*/

//Pin Definitions
#define buttonPin 2
#define servoPin 3
#define ledPin 4
#define soundPin 5
#define triggerPin 6
#define echoPin 7
#define optocouplerPin 8

//Note Definitions
#define NG3 196
#define NG4 392
#define NC7 2093

//Probe State Definition
String PROBE_STATE = "CALIBRATION"; //Identifies what state the probe is in.
                                    //Possible values include CALIBRATION, ASCENT, DESCENT, LANDING, and PANIC.
//General Calibration Definition
#define maxCalibrationAttempts 10 //Sets the number of times to attempt calibration if failures occur.

/*================================================== END SHARED DEFINITIONS ==================================================*/

/*================================================== MISC DEFINITIONS ==================================================*/

//Telemetry Definitions
long telemetryTimer = 0; //A long containing telemetry timer data.
int telemetryFrequency = 5000; //Gives the wait time in milliseconds between telemetry packets.
int telemetryCutOffHeight = 5; //Gives a height in which the telemetry should cease to ensure time isn't wasted on Serial printing.
//NOTE: Motor Wiring - BLACK = GROUND, RED = 5V, and WHITE = PIN.

/*================================================== END MISC DEFINITIONS ==================================================*/

/*================================================== ALTIMETER DEFINITIONS ==================================================*/

//Altimeter Object
EYW::Altimeter myAltimeter; //Instantiates a new altimeter.

//Altimeter Data Definitions
float currentHeight = 0; //A float containing the current height.
float currentVelocity = 0; //A float containing the current velocity.
long altimeterTimer = 0; //A long containing altimeter timer data.
short altimeterCutOff = 5; //The height in which the altimeter should cease to take readings.
int altimeterReadingFrequency = 500; //The time in milliseconds in which the altimeter should wait to take readings.
float autoDropHeight = 20; //The height in meters in which the probe should automatically drop the probe.

/*================================================== END ALTIMETER DEFINITIONS ==================================================*/

/*================================================== RANGE FINDER DEFINITIONS ==================================================*/

//Range Finder Object
EYW::RangeFinder theRangeFinder; //Instantiates a new range finder.

//Timer and Alarm Definitions
boolean proximityAlarmState = false; //Contains whether the alarm is sounding.
//int proximityAlarmSoundTime = 15; //Sets the time in seconds for how long the proximity alarm will sound after detection.
int proximityDistance = 200; //Sets in centimeters the distance in which the range sensor will set off an alarm.
int currentRange = 0; //Gives the current range of the probe.

#define landingNote 440 //Defines the note frequency that plays during landing.

/*================================================== END RANGE FINDER DEFINITIONS ==================================================*/

/*================================================== CAMERA DEFINITIONS ==================================================*/

//Camera Time Definitions
int cameraFireLength = 500; //The length to "hold the button" to take a picture using the camera in milliseconds.
int cameraPictureFrequency = 1500; //The length to wait to take a picture
long cameraTimer = 0; //A long containing camera timer data.
long cameraAutoOffTimer = 0; //A long containing camera timer data for the auto off feature.
long cameraAutoOffPreventionFrequency = 25000; //The frequency to take pictures during ascent to prevent the auto off feature from activating.
int cameraOnLength = 3000; //The length to "hold the button" to turn on the camera in milliseconds.

/* OLD SERVO CAMERA CODE
EYW::Camera cameraServo; //Instantiates a new camera object.
*/

/*================================================== END CAMERA DEFINITIONS ==================================================*/

/*================================================== SHARED FUNCTIONS ==================================================*/

//Calibrates and initializes the probe's systems.
void setup()
{
  miscInitializer(); //Calibrates sound, LED pins, and bluetooth.
 
  Serial.print(F("\nAltimeter calibration beginning in one second; please place the probe on the ground...")); //Altimeter calibration.
  calibrationNote(NG3); //Signals the beginning of the altimeter calibration.
  delay(1000); //Waits 1000 milliseconds to begin.
  altimeterInitializer(); //Initializes the altimeter.

  Serial.print(F("\nRange finder calibration beginning in three seconds; please place the probe 10 cm away from the ground.")); //Range finder calibration.
  calibrationNote(NG3); //Signals the beginning of the range finder calibration.
  delay(3000); //Waits 3000 milliseconds to begin.
  rangeFinderInitializer(); //Initializes the range finder.

  Serial.print(F("\nCamera calibration beginning in one second.")); //Camera calibration.
  calibrationNote(NG3); //Signals the beginning of the camera calibration.
  delay(1000); //Waits 1000 milliseconds to begin.
  cameraInitalizer(); //Initializes the camera.

  Serial.println(F("The servo motor will open in ten seconds.")); //Prints out a signal to ready the probe for attachment.
  delay(10000); //Waits ten seconds.
  toggleServo();        //Opens the servo to attach the probe to the drone.
  Serial.println(F("You have ten seconds to attach the probe to the drone.")); //Prints out a signal to attach the probe now.
  delay(10000); //Waits ten seconds.
  toggleServo(); //Closes the servo to secure the probe to the drone.
  
  Serial.print(F("\nCalibration complete. Entering ascent mode in one second...")); //Calibration is complete.
  delay(1000); //Waits 1000 milliseconds to begin.
  PROBE_STATE = "ASCENT"; //Sets the probe state to ascent when calibration is complete.
}
 
//Updates the probe's subsystems.
void loop()
{
  /*STATE SETTERS*/
  if(changeStateTo.compareTo("NO") != 0){ //Checks to see if there has been a manual state change.
    PROBE_STATE = changeStateTo; //Sets the state to whatever state command has been set.
    if(PROBE_STATE.compareTo("ASCENT") == 0){ //Checks to see if the state was changed to "Ascent."
      proximityAlarmState = false; //If the state was changed to "Ascent," disables the alarm.
    }
    changeStateTo = "NO"; //Resets changeStateTo its default value.
  }
  if(PROBE_STATE.compareTo("ASCENT") == 0 && launchedFlag == true){ //Checks to see if the probe has changed from ascending to descending.
      /* CAMERA SERVO BACKUP
       * cameraServo.beginTimer(100); //Begins taking pictures 0.1 seconds after the probe is released.
      */
      PROBE_STATE = "DESCENT"; //Sets the state to DESCENT.
  }
  if(proximityAlarmState == true && PROBE_STATE.compareTo("DESCENT") == 0){ //Checks to see if the probe has started to land.
      PROBE_STATE = "LANDING"; //Sets the state to LANDING.
  }
  /*END STATE SETTERS*/
  /*SUBSYSTEM UPDATES*/
  miscUpdate(); //Updates telemetry and bluetooth.
  altimeterUpdate(); //Updates the altimeter.
  rangeFinderUpdate(); //Updates the range finder.
  //cameraUpdate(); //Updates the camera. Was removed as the camera is no longer being used.
  /*END SUBSYSTEM UPDATES*/
}

/*================================================== END SHARED FUNCTIONS ==================================================*/

/*================================================== MISC FUNCTIONS ==================================================*/

//Calibrates sound, bluetooth, and LED services.
void miscInitializer(){
  pinMode(soundPin, OUTPUT); //Sets the speaker pin to output.
  pinMode(ledPin, OUTPUT); //Sets the LED pin to output.
  calibrationNote(NG3); //Signals beginning of bluetooth calibration.
  startBT();            //Starts communication with the bluetooth module.
  setServo();           //Sets the servo to the default settings and position.
  Serial.print(F("SUCCESS: Miscellaneous systems have been calibrated successfully.")); //Signals that calibration was successful.
  calibrationNote(NG4); //Signals end of bluetooth calibration.
}

//Updates bluetooth and telemetry services.
void miscUpdate(){
  if (Serial.available())   //Checks for sent commands.
  {
    processCommand();       //Processes the command.
  }
  if(autoDropState == true && currentHeight > autoDropHeight && PROBE_STATE.compareTo("ASCENT") == 0){
    //Check if the system is already launched.
    if (launchedFlag) Serial.println("System needs to be RESET before it can LAUNCH");
    //If the system is ready to be launched
    else 
    {
      Serial.println(F("Auto-Launch initiated.")); //Signals the beginni
      launchedFlag = false;                  //Sets launchedFlag to false.
      countDown();                           //Starts the coutdown.
      toggleServo();                         //Toggles the servo.
    }
  }
  if(currentHeight < telemetryCutOffHeight && !PROBE_STATE.compareTo("ASCENT") == 0) return; //Prevents telemetry from running if we are close to landing.
  if((PROBE_STATE.compareTo("ASCENT") == 0 || PROBE_STATE.compareTo("DESCENT") == 0) && verboseState == true){ //Checks to see if telemetry should be outputted.
      if(millis() - telemetryTimer > telemetryFrequency){ //Checks to see if another telemetry packet should be sent.
          telemetryTimer = millis(); //Resets the timer.
          Serial.print("\nT+"); //Prints out time since the Arduino was started.
          Serial.print(millis() / 1000);
          Serial.print(" seconds: ");
          Serial.print(PROBE_STATE); //Prints out the current probe state.
          Serial.print("\n\tHeight = "); //Prints out the current height.
          Serial.print(currentHeight);
          Serial.print(" m\n\tRange = "); //Prints out the current range.
          Serial.print(currentRange);
          Serial.print(" cm\n\tInstantaneous Velocity = "); //Prints out the current velocity.
          Serial.print(currentVelocity);
          Serial.print(" m/s\n");
      }
  }
}

//Plays a note and flashes an LED for calibration.
void calibrationNote(int frequency){
  digitalWrite(ledPin, HIGH); //Turns on the LED.
  tone(soundPin, frequency); //Plays the calibration note.
  delay(500); //Waits 500 milliseconds.
  digitalWrite(ledPin, LOW); //Turns off the LED.
  noTone(soundPin); //Stops the calibration note.
  delay(20); //Waits 20 milliseconds.
}

/*================================================== END MISC FUNCTIONS ==================================================*/

/*================================================== ALTIMETER FUNCTIONS ==================================================*/

//Calibrates the altimeter.
void altimeterInitializer(){
  myAltimeter.begin(2, 13, 5); //Initializes the altimeter.
  boolean success = false; //Creates a variable to determine if calibration successfully occured.
  for(int i = 0; i < maxCalibrationAttempts; i++){ //Attempts calibration up to maxCalibrationAttempts times.
    if(myAltimeter.calibrate(100) == true){ //Calibrates the altimeter.
      success = true; //If calibration was successful, the program continues on in the calibration sequence.
      break;
    }
    Serial.print(F("\nAltimeter calibration failed. Retrying...")); //Calibration failed.
    delay(100); //Waits 100 milliseconds before trying again.
  }
  if(success == false){ //Checks to see if all calibration attempts failed.
    Serial.print(F("\nDANGER: The altimeter's calibration has failed. Please reset the Arduino.")); //All calibration attempts failed.
    calibrationNote(NC7); //Plays failure note.
  } else {
    Serial.print(F("\nSUCCESS: The altimeter has been calibrated successfully.")); //Success. Plays 2 calibration notes.
    calibrationNote(NG4);
    delay(500);
    calibrationNote(NG4);
    return;
  }
  altimeterTimer = millis(); //Sets the altimeter timer.
}

//Updates the altimeter.
void altimeterUpdate(){
  if(millis() - altimeterTimer < altimeterReadingFrequency || !(PROBE_STATE.compareTo("DESCENT") == 0 || PROBE_STATE.compareTo("ASCENT") == 0)) return; //Checks to see if the altimeter should be updated.
  currentHeight = myAltimeter.getHeightAvg(10); //Sets the current height.
  currentVelocity = currentHeight / (millis() - altimeterTimer); //Sets the current velocity.
  altimeterTimer = millis(); //Resets the altimeter timer.
}

/*================================================== END ALTIMETER FUNCTIONS ==================================================*/

/*================================================== RANGE FINDER FUNCTIONS ==================================================*/

//Calibrates the range finder.
void rangeFinderInitializer(){
  theRangeFinder.begin(); //Initialize the range finder.
  boolean success = false; //Creates a variable to determine if calibration successfully occured.
  for(int i = 0; i < maxCalibrationAttempts; i++){ //Attempts calibration up to maxCalibrationAttempts times.
    if(theRangeFinder.calibrate(10) == true){ //Calibrates the range finder.
      success = true; //If calibration was successful, the program continues on in the calibration sequence.
      break;
    }
    Serial.print(F("\nRange finder calibration failed. Retrying...")); //Calibration failed.
    delay(100); //Waits 100 milliseconds before trying again.
  }
  if(success == false){ //Checks to see if all calibration attempts failed.
    Serial.print(F("\nDANGER: The range finder's calibration has failed. Please reset the Arduino.")); //All calibration attempts failed.
    calibrationNote(NC7); //Plays failure note.
  } else {
    Serial.print(F("\nSUCCESS: The range finder has been calibrated successfully.")); //Success. Plays 3 calibration notes.
    calibrationNote(NG4);
    delay(500);
    calibrationNote(NG4);
    delay(500);
    calibrationNote(NG4);
    return;
  }
}

//Updates the range finder.
void rangeFinderUpdate() {
  currentRange = theRangeFinder.getDistance(); //Sets the current detected range.
  if(!(PROBE_STATE.compareTo("DESCENT") == 0 || PROBE_STATE.compareTo("LANDING") == 0)) return; //Checks to see if the range finder should be updated further.
  if(currentRange < proximityDistance && proximityAlarmState == false){ //Checks the current height to see if we are within range of the ground and if we are already in the alarm state.
    proximityAlarmState = true; //Sets the alarm state.
  }
  if(currentRange < proximityDistance && proximityAlarmState == true){ //Checks if the probe has been identified in proximity to the ground and plays a warning.
    tone(soundPin, landingNote); //Begins the default tone.
    theRangeFinder.led(true); //Turns on the LED.
    //delay(200); //Waits 0.20 seconds.
    //noTone(soundPin); //Stops the default tone.
    //delay(20); //Waits 0.02 seconds.
    //theRangeFinder.led(false); //Turns off the LED.
  }
}

/*================================================== END RANGE FINDER FUNCTIONS ==================================================*/

/*================================================== CAMERA FUNCTIONS ==================================================*/

//Calibrates the camera.
void cameraInitalizer(){
  pinMode(optocouplerPin, OUTPUT); //Enables output mode for optocouplerPin.
  digitalWrite(optocouplerPin, HIGH); //Enables the output signal.
  delay(cameraOnLength); //Waits for how long the camera's button must be pressed to turn on the camera.
  digitalWrite(optocouplerPin, LOW); //Disables the output signal.
  Serial.print(F("\nSUCCESS: The camera has been calibrated successfully.\nDO NOT FORGET TO HIT THE MODE BUTTON.")); //Success. Plays 4 calibration notes.
  calibrationNote(NG4);
  delay(500);
  calibrationNote(NG4);
  delay(500);
  calibrationNote(NG4);
  delay(500);
  calibrationNote(NG4);
  cameraTimer = millis(); //Sets the camera timer.
  cameraAutoOffTimer = millis();
  
  /* OLD SERVO CAMERA CODE
  cameraServo.begin(2,4,5,8);
  boolean success = false; //Creates a variable to determine if calibration successfully occured.
  for(int i = 0; i < maxCalibrationAttempts; i++){ //Attempts calibration up to maxCalibrationAttempts times.
    if(cameraServo.calibrate(45, 135) == true){ //Calibrates the camera.
      cameraServo.motor.write(45); //Turns on the camera.
      delay(3000); //Waits three seconds.
      cameraServo.motor.write(135); //Finishes turning on the camera.
      cameraServo.beginTimer(1); //Starts a new timer.
      success = true; //If calibration was successful, the program continues on in the calibration sequence.
      break;
    }
    Serial.print("\nCamera calibration failed. Retrying..."); //Calibration failed.
    delay(100); //Waits 100 milliseconds before retrying calibration.
  }
  if(success == false){ //Checks to see if all calibration attempts failed.
    Serial.print("\nDANGER: The camera's calibration has failed. Please reset the Arduino."); //All calibration attempts failed.
    calibrationNote(NC7); //Plays the failure note.
  } else {
    Serial.print("\nSUCCESS: The camera has been calibrated successfully."); //Success. Plays 4 calibration notes.
    calibrationNote(NG4);
    delay(500);
    calibrationNote(NG4);
    delay(500);
    calibrationNote(NG4);
    delay(500);
    calibrationNote(NG4);
    cameraAutoOffTimer = millis();
    return;
  }
  */
}

//Updates the camera.
void cameraUpdate(){
  if(millis() - cameraAutoOffTimer > cameraAutoOffPreventionFrequency && PROBE_STATE.compareTo("ASCENT") == 0){ //Checks to see if the auto off prevention feature should be enabled.
    digitalWrite(optocouplerPin, HIGH); //Begins a picture.
    delay(cameraFireLength); //Delays for how long the "button" needs to be pressed.
    digitalWrite(optocouplerPin, LOW); //Finishes a picture.
    cameraAutoOffTimer = millis(); //Resets the camera timer.
  }
  if(millis() - cameraTimer < cameraPictureFrequency || PROBE_STATE.compareTo("ASCENT") == 0) return; //Checks to see if the camera should take another picture.
  digitalWrite(optocouplerPin, HIGH); //Begins a picture.
  delay(cameraFireLength); //Delays for how long the "button" needs to be pressed.
  digitalWrite(optocouplerPin, LOW); //Finishes a picture.
  cameraTimer = millis(); //Resets the camera timer.

  /* OLD CAMERA SERVO CODE
  if(millis() - cameraAutoOffTimer > cameraAutoOffPreventionFrequency && PROBE_STATE.compareTo("ASCENT") == 0){ //Checks to see if the auto off prevention feature should fire.
    cameraServo.getPicture(); //Takes a picture.
    cameraAutoOffTimer = millis(); //Resets the timer.
  }
  if(!cameraServo.timerExpired() || PROBE_STATE.compareTo("ASCENT") == 0) return; //Checks to see if its time to take a picture.
  cameraServo.getPicture(); //Takes a picture.
  cameraServo.beginTimer(cameraPictureFrequency); //Resets the timer.
  */
}


/*================================================== END CAMERA FUNCTIONS ==================================================*/

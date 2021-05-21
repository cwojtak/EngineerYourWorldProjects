//probe_descent.ino
//Connor Wojtak and Sadie Sokolowski
//3-23-2021
//Developed for Engineering Your World at Ravenwood High School under #Redacted#.
//A prototype for the descent subsystem of the camera probe.

//Imports
#include <EYW.h>

#define buttonPin 2
#define ledPin 4
#define soundPin 5
#define triggerPin 6
#define echoPin 7


//================================================== DESCENT DEFINITIONS ==================================================\\
//Range Finder Object
EYW::RangeFinder theRangeFinder; //Instantiates a new range finder.

//Timer and Alarm Definitions
boolean proximityAlarmState = false; //Contains whether the alarm is sounding.
int proximityAlarmSoundTime = 15; //Sets the time in seconds for how long the proximity alarm will sound after detection.
int proximityDistance = 10; //Sets in centimeters the distance in which the range sensor will set off an alarm.
//================================================== END DESCENT DEFINITIONS ==================================================\\

void setup() {
  Serial.begin(9600);
  soundInitializer();
  rangeFinderInitializer();
}

void loop() {
   rangeUpdate();
}

void soundInitializer(){
  pinMode(soundPin, OUTPUT); //Set the speaker pin to output.
}

void rangeFinderInitializer(){
  theRangeFinder.begin(); //Initialize the range finder.
  theRangeFinder.alarm(); //Sound an alarm until the range finder senses an object at 10 cm.
  theRangeFinder.calibrate(10); //Calibrate the range finder at 10 cm.
  //Use the below function to calibrate the range finder by hand.
  //theRangeFinder.calibrate(200, x);
}

void rangeUpdate() {
  if(theRangeFinder.getDistance() < proximityDistance && proximityAlarmState == false){ //Checks the current height to see if we are within range of the ground and if we are already in the alarm state.
    proximityAlarmState = true; //Sets the alarm state.
  }
  if(theRangeFinder.getDistance() < proximityDistance && proximityAlarmState == true){
    tone(soundPin, 440); //Begins the default tone.
    theRangeFinder.led(true); //Turns on the LED.
    delay(250); //Waits 0.25 seconds.
    noTone(soundPin); //Stops the default tone.
    delay(100); //Waits 0.1 seconds.
    theRangeFinder.led(false); //Turns off the LED.
  }
  if(theRangeFinder.getDistance() > proximityDistance && proximityAlarmState == true){
    proximityAlarmState = false; //Sets the alarm state.
  }
}

//Connor Wojtak
//3-18-2021
//Produced for Engineering Your World at Ravenwood High School under #Redacted#.
//probe_bluetooth.ino
//Purpose: Allow for Bluetooth control of the Arduino to deploy probes using the SparkFun RN-41 Bluetooth controller.

//This program uses modified code from:
/*
 * https://learn.sparkfun.com/tutorials/using-the-bluesmirf#example-code-using-command-mode
 * The University of Texas at Austin - UTeachEngineering - Engineer Your World: Bluetooth_code_v2_0 by Jesse DeWald
 */

//When pairing with the unit, if you need a pairing code, it is by default 1234 for Sparkfun modules.

//Imports
#include "Arduino.h"
#include <Servo.h>
#include <SoftwareSerial.h>

//Pin Definitions
int bluetoothTx = 1; 
int bluetoothRx = 0;
int servoPin = 3;

//Servo Definitions
Servo theServo;
int loadPos = 10; //Servo position to load the payload.
int launchPos = 150; //Servo position to launch the payload.
int countdownLength = 3; //How long, in seconds, the countdown to launch is.

//Sets the max length of commands parsed.
int bufferLength = 32;

//Instantiates a SoftwareSerial object to interface with the module.
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup() {
  bluetoothInitializer();
}

void loop() {
  bluetoothUpdate();
}

void bluetoothInitializer(){
  //Begin Bluetooth Communication
  bluetooth.begin(115200); //Starts at 115200 bauds.
  //Enter Command Mode
  bluetooth.print("$");
  bluetooth.print("$");
  bluetooth.print("$");
  //Wait for the module to catch up.
  delay(100);
  //Initialization
  bluetooth.println("SN,EYW-ProbeLauncher"); //Names the unit.
  bluetooth.println("U,9600,N"); //Sets the baud rate to 9600 bauds to ensure stability.
  bluetooth.begin(9600); //Restarts communication at 9600 bauds.
  //Wait for the module to catch up.
  //Initalize the Servo
  theServo.attach(servoPin); //Attaches the servo object to the servo pin.
  theServo.write(loadPos); //Moves the servo to the default position.
}

void bluetoothUpdate(){
  if(bluetooth.available()) { //Triggered when the Bluetooth module receives data.
    while(bluetooth.available()) { //Reads data while some still exists. 
      char command[bufferLength]; //Creates a 32 character array.
      readCommand(command); //Reads the command from the Serial buffer.
      String formattedCommand(command); //Turns the character array into a String.
      formattedCommand.toUpperCase(); //Puts the command in upper case so that any command, whether it is in lower case or upper case, can be read.
      if(formattedCommand.compareTo("LAUNCH") == 0){ //Checks to see if the launch command was sent.
        launch();
      } else if(formattedCommand.compareTo("RESET") == 0){ //Checks to see if the reset command was sent.
        reset();
      } else if(formattedCommand.compareTo("TEST") == 0) { //Checks to see if the unit should enter range test mode.
        while(true){ //Prints out the altitude of the device every second. 
          delay(1000);
          bluetooth.println("Altitude: ");
          bluetooth.print(getAltitude());
          bluetooth.print(" m.\n");
        }
      } else { //No readable commands have been sent yet.
        bluetooth.print("\nUnknown Command: ");
        bluetooth.print(formattedCommand);
        bluetooth.println("\nAcceptable Commands: Launch \t\t Reset\n\n");
      }
      while(bluetooth.available()) bluetooth.read(); //Reads and clears the Serial buffer until there is nothing remaining (clears the last sent command).
    }
  }
}

void launch(){
  for(int i = countdownLength; i > 0; i--){ //Executes a countdown.
    bluetooth.print(i);
    bluetooth.print("...\n");
    delay(1000);
  }
  bluetooth.print("Launch in progress!\n\n");
  theServo.write(launchPos); //Sets the servo to the default position.
  delay(500); //Gives the servo time to reach the launch position.
}

void reset(){
  theServo.write(loadPos); //Sets the servo to the default position.
  delay(500); //Gives the servo time to reach the default position.
}


//Reads the string in the buffer and appends a null terminator to the end. Adapted from Jesse DeWald's original program.
void readCommand(char commandBuffer[])
{
  int numBytes = 0;                                        //Stores the number of characters in the Serial buffer.
  numBytes = bluetooth.readBytes(commandBuffer,bufferLength); //Reads the bytes from the receive buffer until the bufferLength is reached. 
                                                           //Returns the number of bytes (characters) in string.
  if (commandBuffer[numBytes-1] == 10 || commandBuffer[numBytes-1] == 13) //Checks for a newline or carriage return before the null terminator.
  {
    numBytes--; //If there is a CR or NL, move the array pointer to that position.
  }
  commandBuffer[numBytes] = '\0'; //Overwrites the last character with a null terminator; this allows the use of terminal programs that do not send CR or NL characters.
}

int getAltitude(){
  return 0; //TODO: Implement altimeter or ultrasound to measure the altitude of the device.
}


/* Some OLD CODE  
 *  if(bluetooth.available()) { //Triggered when the Bluetooth module receives data.
    while(bluetooth.available()) { //Reads data while some still exists. 
      char command[bufferLength]; //Creates a 32 character array.
      readCommand(command); //Reads the command from the Serial buffer.
      String formattedCommand(command); //Turns the character array into a String.
      formattedCommand.toUpperCase(); //Puts the command in upper case so that any command, whether it is in lower case or upper case, can be read.
      if(formattedCommand.compareTo("LAUNCH") == 0){ //Checks to see if the launch command was sent.
        launch();
      } else if(formattedCommand.compareTo("RESET") == 0){ //Checks to see if the reset command was sent.
        reset();
      } else if(formattedCommand.compareTo("TEST") == 0) { //Checks to see if the unit should enter range test mode.
        while(true){ //Prints out the altitude of the device every second. 
          delay(1000);
          bluetooth.println("Altitude: ");
          bluetooth.print(getAltitude());
          bluetooth.print(" m.\n");
        }
      } else { //No readable commands have been sent yet.
        bluetooth.print("\nUnknown Command: ");
        bluetooth.print(formattedCommand);
        bluetooth.println("\nAcceptable Commands: Launch \t\t Reset\n\n");
      }
      while(bluetooth.available()) bluetooth.read(); //Reads and clears the Serial buffer until there is nothing remaining (clears the last sent command).
    }
  }

  //Begin Bluetooth Communication
  bluetooth.begin(115200); //Starts at 115200 bauds.
  //Enter Command Mode
  bluetooth.print("$");
  bluetooth.print("$");
  bluetooth.print("$");
  //Wait for the module to catch up.
  delay(100);
  //Initialization
  bluetooth.println("SN,EYW-ProbeLauncher"); //Names the unit.
  bluetooth.println("U,9600,N"); //Sets the baud rate to 9600 bauds to ensure stability.
  bluetooth.begin(9600); //Restarts communication at 9600 bauds.
  //Wait for the module to catch up.
  //Initalize the Servo
  theServo.attach(servoPin); //Attaches the servo object to the servo pin.
  theServo.write(loadPos); //Moves the servo to the default position.
*/

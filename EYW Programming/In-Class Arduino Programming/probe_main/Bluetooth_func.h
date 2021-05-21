#ifndef Bluetooth_func_h
#define Bluetooth_func_h

/*---------------------------------------------------------Function Definitions-----------------------------------------------------*/
void toggleServo(void);                         //handles all servo movements according to launch-flag

void launch(void);                              //sets the servo to the release position 
void load (void);                               //sets the servo to the load position
void countDown(void);                           //starts a countdown to launch

void processCommand(void);                      //Calls the following three functions to process a serial command
void readCommand(char commandBuffer[]);         //reads the string in the buffer and appends a null terminator to the end
void parseCommand (String);                     //compares the string to acceptable commands: LAUNCH and RESET
void flushBuffer(void);                         //flushes the buffer of anything over 32 characters

void startBT(int sP);                   //setup function to set interrupts and begin serial communications
void startBT(void);                             //"overloaded function" to provide default values to startBT
void setServo(int load, int launch, int cD);    //setup function to set the servo limits and the countdown length
void setServo(void);                            //"overloaded function" to provide default values to setServo
/*---------------------------------------------------------Function Definitions-----------------------------------------------------*/


/*---------------------------------------------------------Setup Functions----------------------------------------------------------*/
//setup function to set interrupts and begin serial communications
void startBT(int sP)
{
  servoPin = sP;              //sets the button pin and the servo pin
  Serial.begin(9600);                         //Start serial communications at 9600
                                              //note that this may need to be changed to match your module
  delay(1000);                                //wait 1 sec to establish a connection
  
  Serial.println("\n\n**ARDUINO HAS RESET**");
  Serial.println("(If this was unintended, your batteries may need to be replaced) \n\n\n"); 
}
//setup function to set interrupts and begin serial communications
//defaults to Button on Pin # 2, and Servo on Pin # 3
void startBT(void)
{
  Serial.begin(9600);                         //Start serial communications at 9600
                                              //note that this may need to be changed to match your module
  delay(1000);                                //wait 1 sec to establish a connection
  
  Serial.println("\n\n**ARDUINO HAS RESET**");
  Serial.println("(If this was unintended, your batteries may need to be replaced) \n\n\n"); 
}

//setup function to set the servo limits and the countdown length
void setServo(int load, int launch, int cD)
{
  loadPos = load; launchPos = launch; countdownLength = cD;   //set the load and launch positions, and set the countdown length
  myservo.attach(servoPin);                                   // attaches the servo to the pin set in startBT
  myservo.write(loadPos);                                     //default the servo to the hold position
  delay(250);
}
//setup function to set the servo limits and the countdown length
//default to Load = 0, Launch = 150, and Countdown = 3
void setServo(void)
{
  myservo.attach(servoPin);                 //attaches the servo to the pin set in startBT
  myservo.write(loadPos);                   //default the servo to the hold position
  delay(250);
}

/*---------------------------------------------------------Command Functions----------------------------------------------------------*/
//Calls the other command functions to process a serial command
void processCommand(void)
{
   char command[bufferLength];                   //creates a character array of 32 bytes
   readCommand(command);                         //reades each charcter in the buffer into the character array
   String commandStr(command);                   //converts the character array to a string
   commandStr.toUpperCase();                     //change all commands to uppercase for case-free comparison
   parseCommand(commandStr);                     //compares the string to acceptable commands
   flushBuffer();                                //clears the Serial Receive buffer
}

//reads the string in the buffer and appends a null terminator to the end
void readCommand(char commandBuffer[])
{
  int numBytes = 0;                                        //variable to store the numver of characters in the Serial Buffer
  numBytes = Serial.readBytes(commandBuffer,bufferLength); //Reads bytes from the receive buffer until the bufferlength is reached. 
                                                           //Returns number of bytes (characters) in string.
  if (commandBuffer[numBytes-1] == 10 || commandBuffer[numBytes-1] == 13) //check for a newline or carriage return before the null terminator
  {
    numBytes--; //if there is a CR or NL, move array pointer to that position
  }
  commandBuffer[numBytes] = '\0'; //overwrite the last character with a null terminator
                                  //this allows use of terminal programs that do/not send CR or NL characters
  return;
}

//compares the string to acceptable commands: LAUNCH and RESET
void parseCommand(String command)
{
  String loadStr = String("RESET");          //Creates a comparison string for Reset
  String launchStr = String("LAUNCH");       //Creates a comparison string for Launch
  String verboseStr = String("VERBOSE");     //Creates a comparison string for Verbose
  String ascentStr = String("ASCENT");       //Creates a comparison string for Ascent
  String descentStr = String("DESCENT");     //Creates a comparison string for Descent
  String landingStr = String("LANDING");     //Creates a comparison string for Landing
  String autoDropStr = String("AUTODROP");   //Creates a comparison string for Autodrop
  String panicStrOne = String("PANIC");      //Creates a comparison string for Panic
  String panicStrTwo = String("PPP");        //Creates a comparison string for Panic

  //compareTo will return a zero if the command matches
  if (command.compareTo(loadStr) == 0)
  {
    //check if the system is already in loading position
    if (!launchedFlag) Serial.println("System is already RESET");
    //if system has launched
    else
    {
      launchedFlag = true;                   //set the launched flag to true
      toggleServo();                         //toggle the servo back to loading position
    }

  }
  //compareTo will return a zero if the command matches
  else if (command.compareTo(launchStr) == 0) 
  {
    //check if the system is already launched
    if (launchedFlag) Serial.println("System needs to be RESET before it can LAUNCH");
    //if system is in load position
    else 
    {
      launchedFlag = false;                  //set the launched flag to false
      countDown();                           //start the coutdown
      toggleServo();                         //toggle to the servo to launch
    }
  }
  else if (command.compareTo(verboseStr) == 0){ //Checks to see if the command is "Verbose."
      verboseState = !verboseState; //Toggles whether telemetry should be outputted to the terminal. The default is true.
  }
  else if (command.compareTo(ascentStr) == 0){
    changeStateTo = "ASCENT";
  }
  else if (command.compareTo(descentStr) == 0){
    changeStateTo = "DESCENT";
  }
  else if (command.compareTo(landingStr) == 0){
    changeStateTo = "LANDING";
  }
  else if (command.compareTo(autoDropStr) == 0){
    autoDropState = !autoDropState;
  }
  else if (command.compareTo(panicStrOne) == 0 || command.compareTo(panicStrTwo)){
    changeStateTo = "PANIC";
    digitalWrite(4, HIGH);
    tone(5, 440);
    Serial.print(F("\nPANIC MODE ACTIVATED"));
  }
  //if compareTo did not find a match, print the unknown command and list known commands
  else 
  {
    Serial.print("\n Unknown Command: "); 
    Serial.println(command); 
    Serial.println("\nAcceptable Commands:\nLaunch \t\t Reset \t\t Verbose \t\t Ascent \t\t Descent \t\t Landing \t\t Panic");
  }

}

//flushes the buffer of anything over 32 characters
void flushBuffer(void)
{
  while(Serial.available()) Serial.read(); //will read (and clear) the Serial Buffer until there is nothing remaining.
}

/*---------------------------------------------------------Servo Functions----------------------------------------------------------*/

//handles all servo movements according to launch-flag
void toggleServo(void)
{
  if (!launchedFlag)launch();      //if the system hasn't launched, launch
  else load();                     //else set the servo to load
  launchedFlag = !launchedFlag;    //toggle the launch flag
}

//starts a countdown to launch
void countDown (void)
{
  //start the coundownt from countdownLength to zero
  for (int i = countdownLength; i > 0; i--)
  {
    Serial.print(i);
    Serial.println("...");
    delay(1000);
  }
  Serial.println("Launch!!\n\n");

}

//sets the servo to the release position
void launch(void)
{

  myservo.write(launchPos);    //set the servo to the launch position
  delay (500);                 //wait 1/2 second to allow servo to reach position
  Serial.println("**Reset Servo before Launch!**\n\n");
}

//sets the servo to the load position
void load (void)
{
  myservo.write(loadPos); //set the servo to the launch position
  delay (500);            //wait 1/2 second to allow servo to reach position
  Serial.println("Ready To Launch!");
}
/*---------------------------------------------------------Servo Functions----------------------------------------------------------*/


#endif

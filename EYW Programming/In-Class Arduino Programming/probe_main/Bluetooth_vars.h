#ifndef Bluetooth_vars_h
#define Bluetooth_vars_h

/*This file contains the default variable values. DO NOT change these unless you know what you're doing*/

Servo myservo;             // create servo object to control a servo

static int servoPin = 3;  //arduino pin connected to servo
static int buttonPin = 2; //arduino pin connected to pushbutton (must be Pin 2 or 3 to work with interuppt handler)
static int intNum;        //interuppt number (can be either 0 or 1)

volatile static boolean launchedFlag = false;   //Launch flag: has the system launched? True or false

                
static int loadPos = 45;              //servo position to load the payload
static int launchPos = 145;          //servo position to release the payload - LAUNCH!

static const int bufferLength = 30;  //command buffer can contain 30 characters plus a carriage return plus null terminator
static int countdownLength = 3;      //length of the countdown to launch, in seconds.

static boolean verboseState = true;
static boolean autoDropState = true;
static String changeStateTo = "NO";

/*This file contains the default variable values. DO NOT change these unless you know what you're doing*/

#endif

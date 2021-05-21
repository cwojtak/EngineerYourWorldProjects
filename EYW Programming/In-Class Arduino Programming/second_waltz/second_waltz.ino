//second_waltz
//Connor Wojtak - connor.wojtak@gmail.com
//2-12-2021
//Designed for Engineering Your World with #Redacted# at Ravenwood High School.
//A program that outputs auditorily Shostakovich's Second Waltz.

//Global frequency definitions
//Modified from bhagman's Tone.h source code: https://github.com/bhagman/Tone/blob/master/Tone.h
//Purpose: Creates preprocessor directives for each note that replace each reference to a note frequency in the source code with the associated number during compilation (these are not variables as they do not exist at runtime).
#define NB0  31
#define NC1  33
#define NCS1 35
#define ND1  37
#define NDS1 39
#define NE1  41
#define NF1  44
#define NFS1 46
#define NG1  49
#define NGS1 52
#define NA1  55
#define NAS1 58
#define NB1  62
#define NC2  65
#define NCS2 69
#define ND2  73
#define NDS2 78
#define NE2  82
#define NF2  87
#define NFS2 93
#define NG2  98
#define NGS2 104
#define NA2  110
#define NAS2 117
#define NB2  123
#define NC3  131
#define NCS3 139
#define ND3  147
#define NDS3 156
#define NE3  165
#define NF3  175
#define NFS3 185
#define NG3  196
#define NGS3 208
#define NA3  220
#define NAS3 233
#define NB3  247
#define NC4  262
#define NCS4 277
#define ND4  294
#define NDS4 311
#define NE4  330
#define NF4  349
#define NFS4 370
#define NG4  392
#define NGS4 415
#define NA4  440
#define NAS4 466
#define NB4  494
#define NC5  523
#define NCS5 554
#define ND5  587
#define NDS5 622
#define NE5  659
#define NF5  698
#define NFS5 740
#define NG5  784
#define NGS5 831
#define NA5  880
#define NAS5 932
#define NB5  988
#define NC6  1047
#define NCS6 1109
#define ND6  1175
#define NDS6 1245
#define NE6  1319
#define NF6  1397
#define NFS6 1480
#define NG6  1568
#define NGS6 1661
#define NA6  1760
#define NAS6 1865
#define NB6  1976
#define NC7  2093
#define NCS7 2217
#define ND7  2349
#define NDS7 2489
#define NE7  2637
#define NF7  2794
#define NFS7 2960
#define NG7  3136
#define NGS7 3322
#define NA7  3520
#define NAS7 3729
#define NB7  3951
#define NC8  4186
#define NCS8 4435
#define ND8  4699
#define NDS8 4978

//Rhythm definitions
//Purpose: Creates preprocessor directives for each note that replace each reference to a note duration in the source code with the associated number during compilation (these are not variables as they do not exist at runtime).
#define sNote 0.25 //Sixteenth note beat portion
#define eNote 0.5 //Eighth note beat portion
#define qNote 1 //Quarter note beat portion
#define hNote 2 //Half note beat portion
#define dHNote 3 //Dotted half note beat portion

//Time definitions
const int notePause = 10; //Pause time; Note: Half the true total pause heard
const double tempoBPS = 3.00; //Tempo in beats per second

//Pin definition
const int buzzerPin = 12; //Output pin


//Song definition
//Array Format: An array of notes, with each note being defined as a separate array of length 2.
//Inner Array Format: Each inner array contains the note frequency as the first element and the length in beats.
//Note: Measure beginnings are marked for the first beat of each measure.
const double songNotes[][2] = {
  {NFS4, dHNote}, //Begin measure 1
  {ND4, hNote}, //Begin measure 2
  {NCS4, qNote},
  {NB3, dHNote + qNote}, //Contains measure 3 and begins 4
  {NB3, qNote},
  {NCS4, qNote},
  {ND4, qNote}, //Begin measure 5
  {NB3, qNote},
  {ND4, qNote},
  {NFS4, hNote}, //Begin measure 6
  {NG4, qNote},
  {NFS4, dHNote}, //Begin measure 7
  {NE4, dHNote}, //Begin measure 8
  {NE4, dHNote}, //Begin measure 9
  {NCS4, hNote}, //Begin measure 10
  {NB3, qNote},
  {NAS3, dHNote + qNote}, //Contains measure 11 and begins 12
  {NFS3, qNote},
  {NAS3, qNote},
  {NCS4, qNote}, //Begin measure 13
  {NAS3, qNote},
  {NCS4, qNote},
  {NE4, qNote}, //Begin measure 14
  {NFS4, qNote},
  {NG4, qNote},
  {NF4, dHNote}, //Begin measure 15
  {NFS4, dHNote}, //Begin measure 16
  {ND5, dHNote}, //Begin measure 17
  {NCS5, hNote}, //Begin measure 18
  {NB4, qNote},
  {NA4, hNote}, //Begin measure 19
  {NG4, qNote},
  {NE4, dHNote}, //Begin measure 20
  {NCS5,dHNote}, //Begin measure 21
  {NB4, hNote}, //Begin measure 22
  {NA4, qNote},
  {NA4, hNote}, //Begin measure 23
  {NFS4, qNote}
  };

//setup(): program entry-point
void setup() {
  for(int i = 0; i < (sizeof(songNotes) / sizeof(songNotes[0])); i++){ //Loop through the set of notes.
    playNote(songNotes[i]); //Play the selected note.
  }
}

//loop(): called repeatedly after setup()
void loop() {
    //Not used.
}

//playNote(): plays the specified note structure
//Parameters (1): const unsigned double note[2] - a one dimensional array containing the note data, with the first element containing the note frequency and the second element containing the duration of the note in beats. See songNotes[] for more information.
//Returns: Nothing
void playNote(const double note[2]){
  delay(notePause); //Delay the note for a certain amount of millisecond after the beat.
  unsigned int len = (note[1] / tempoBPS * 1000) - (notePause * 2); //Calculate the true length of the note.
  tone(buzzerPin, note[0]); //Begin playing the note.
  delay(len); //Delay for the true length of the note.
  noTone(buzzerPin); //End the note.
  delay(notePause); //Delay for a certain amount of milliseconds before the next beat.
}

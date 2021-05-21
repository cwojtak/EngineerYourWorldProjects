//dual_tones_button
//Connor Wojtak
//2-23-2021
//A program to that outputs auditorily Shostakovich's Second Waltz with a bass and treble line.

#include <Tone.h>

//Global frequency definitions
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

//Song definition
double songBassNotes[][2] = {
  {NC3, 1}, //Measure 1
  {NC4, 1},
  {NC4, 1},
  {NG3, 1}, //M2
  {NC4, 1},
  {NC4, 1},
  {NC3, 1}, //M3
  {NC4, 1},
  {NC4, 1},
  {NG3, 1}, //M4
  {NC4, 1},
  {NC4, 1},
  {NC3, 1}, //M5
  {NG3, 1},
  {NG3, 1},
  {NG3, 1}, //M6
  {NG3, 1},
  {NG3, 1},
  {NC3, 1}, //M7
  {NDS3, 1},
  {NDS3, 1},
  {NG3, 1}, //M8
  {NDS3, 1},
  {NDS3, 1},
  {NC3, 1}, //M9
  {NDS3, 1},
  {NDS3, 1},
  {NG3, 1}, //M10
  {NDS3, 1},
  {NDS3, 1},
  {ND3, 1}, //M11
  {NGS3, 1},
  {NGS3, 1},
  {NG3, 1}, //M12
  {NGS3, 1},
  {NGS3, 1},
  {ND3, 1}, //M13
  {NGS3, 1},
  {NGS3, 1},
  {NG3, 1}, //M14
  {NGS3, 1},
  {NGS3, 1},
  {ND3, 1}, //M15
  {NF3, 1},
  {NF3, 1},
  {NG3, 1}, //M16
  {NB3, 1},
  {NF3, 1},
  {ND3, 1}, //M17
  {NF3, 1},
  {NB3, 1},
  {NG3, 1}, //M18
  {NF4, 1},
  {NF4, 1},
  {NC3, 1}, //M19
  {NDS4, 1},
  {NDS4, 1},
  {NG3, 1}, //M20
  {NDS4, 1},
  {NDS4, 1},
  {NC3, 1}, //M21
  {NDS4, 1},
  {NDS4, 1},
  {NG3, 1}, //M22
  {NC4, 1},
  {NC4, 1},
  {NF3, 1}, //M23
  {NGS3, 1},
  {NGS3, 1},
  {NF3, 1}, //M24
  {NF3, 1},
  {NF3, 1},
  {NG3, 1}, //M25
  {ND4, 1},
  {ND4, 1},
  {NG3, 1}, //M26
  {NDS4, 1},
  {ND4, 1},
  {NDS3, 1}, //M27
  {NDS4, 1},
  {NDS4, 1},
  {NGS3, 1}, //M28
  {NC3, 1},
  {ND3, 1},
  {NG3, 1}, //M29
  {NDS4, 1},
  {NDS4, 1},
  {NGS3, 1}, //M30
  {ND3, 1},
  {ND3, 1},
  {NGS3, 1}, //M31
  {ND3, 1},
  {ND3, 1},
  {NGS3, 1}, //M32
  {NC3, 1},
  {NDS3, 1},
  {NG3, 1}, //M33
  {NDS3, 1},
  {NDS3, 1},
  {NGS3, 1}, //M34
  {NDS3, 1},
  {NDS3, 1},
  {NG3, 1}, //M35
  {NDS3, 1},
  {NDS3, 1},
  {NGS3, 1}, //M36
  {NC3, 1},
  {NDS3, 1},
  {NG3, 1}, //M37
  {NDS3, 1},
  {NDS3, 1},
  {NG3, 1}, //M38
  {NF3, 1},
  {NF3, 1},
  {NC3, 1}, //M39
  {0, 1},
  {NB3, 1},
  {NC4, 1},
  {0, 2}
};

double songTrebleNotes[][2] = {
  {0, 12}, //Measures 1-4
  {NG5, 3}, //Measure 5
  {NDS5, 2},//Measure 6
  {ND5, 1},
  {NC5, 4}, //Measures 7 + 8
  {NC5, 1},
  {ND5, 1},
  {NDS5, 1}, //Measure 9
  {NC5, 1},
  {NDS5, 1},
  {NG5, 2}, //Measure 10
  {NGS5, 1},
  {NG5, 3}, //Measure 11
  {NF5, 3}, //Measure 12
  {NF5, 3}, //Measure 13
  {ND5, 2}, //Measure 14
  {NC5, 1},
  {NB4, 4}, //Measures 15 + 16
  {NG4, 1},
  {NB4, 1},
  {ND5, 1}, //Measure 17
  {NB4, 1},
  {ND5, 1},
  {NF5, 1}, //Measure 18
  {NG5, 1},
  {NGS5, 1},
  {NFS5, 3}, //Measure 19
  {NG5, 3}, //Measure 20
  {NDS6, 3}, //Measure 21
  {ND6, 2}, //Measure 22
  {NC6, 1},
  {NAS5, 2}, //Measure 23
  {NGS5, 1},
  {NF5, 3}, //Measure 24
  {ND6, 3}, //Measure 25
  {NC6, 2}, //Measure 26
  {NAS5, 1},
  {NAS5, 3}, //Measure 27
  {0, 1}, //Measure 28
  {NDS6, 1},
  {NF6, 1},
  {NG6, 1}, //Measure 29
  {NG6, 0.5},
  {NF6, 0.5},
  {NG6, 0.5},
  {NGS6, 0.5},
  {NF6, 1}, //Measure 30
  {NF6, 0.5},
  {NDS6, 0.5},
  {NF6, 0.5},
  {NG6, 0.5},
  {NDS6, 1}, //Measure 31
  {0, 1},
  {NG6, 1},
  {0, 1}, //Measure 32
  {NDS6, 1},
  {NF6, 1},
  {NG6, 1}, //Measure 33
  {NG6, 0.5},
  {NF6, 0.5},
  {NG6, 0.5},
  {NGS6, 0.5},
  {NF6, 1}, //Measure 34
  {NF6, 0.5},
  {NDS6, 0.5},
  {NF6, 0.5},
  {NG6, 0.5},
  {NDS6, 1}, //Measure 35
  {0, 1},
  {NG6, 1},
  {0, 1}, //Measure 36
  {NC7, 1},
  {ND7, 1},
  {NDS7, 1}, //Measure 37
  {NDS7, 0.5}, 
  {ND7, 0.5},
  {NDS7, 0.5},
  {NF7, 0.5},
  {ND7, 1}, //Measure 38
  {ND7, 0.5},
  {NC7, 0.5},
  {ND7, 0.5},
  {NDS7, 0.5},
  {NDS7, 1}, //Measure 39
  {0, 1},
  {ND7, 1},
  {NDS7, 1}, //Measure 40
  {0, 2}
};

//Rhythm/Time definitions
const unsigned int notePause = 20;
const byte subdivisionMultiplier = 4;
const double tempoBPS = 3.00 * subdivisionMultiplier;

//Note counters
unsigned int bassNoteCounter = 0;
unsigned int trebleNoteCounter = 0;

unsigned int currentBassNote = 0;
unsigned int remainingBassBeats = 0;
unsigned int currentTrebleNote = 0;
unsigned int remainingTrebleBeats = 0;

//Pin definition
const unsigned int bassBuzzerPin = 12;
const unsigned int trebleBuzzerPin = 8;
const unsigned int buttonPin = 3;

//Button definiton
boolean triggered = false;

//Tone object definitions
Tone notePlayer[2];

//setup(): program entry-point
void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  notePlayer[0].begin(bassBuzzerPin); //Set bass player to use buzzerPin.
  notePlayer[1].begin(trebleBuzzerPin); //Set treble player to use buzzerPin.
}

//loop(): called repeatedly after setup()
void loop() {
  if(digitalRead(buttonPin) == LOW){
    triggered = true;
  }
  if(!triggered) return;
  while(bassNoteCounter < sizeof(songBassNotes) / sizeof(songBassNotes[0]) && trebleNoteCounter < sizeof(songTrebleNotes) / sizeof(songTrebleNotes[0])){
    if(remainingBassBeats == 0 && remainingTrebleBeats == 0){
      notePlayer[0].stop();
      notePlayer[1].stop();
      delay(notePause); //Delay the note for a certain amount of milliseconds after the beat.
      currentBassNote = songBassNotes[bassNoteCounter][0];
      remainingBassBeats = songBassNotes[bassNoteCounter][1] * subdivisionMultiplier;
      currentTrebleNote = songTrebleNotes[trebleNoteCounter][0];
      remainingTrebleBeats = songTrebleNotes[trebleNoteCounter][1] * subdivisionMultiplier;
      bassNoteCounter++;
      trebleNoteCounter++;
      unsigned int len = (1 / tempoBPS * 1000) - notePause; //Calculate the true length of the note.
      if(currentBassNote != 0) notePlayer[0].play(currentBassNote); //Play the bass note.
      if(currentTrebleNote != 0) notePlayer[1].play(currentTrebleNote); //Play the treble note.
      delay(len); //Delay for the true length of the note.
      remainingBassBeats--;
      remainingTrebleBeats--;
    } 
    else if (remainingBassBeats == 0 && remainingTrebleBeats != 0){
      notePlayer[0].stop();
      delay(notePause);
      currentBassNote = songBassNotes[bassNoteCounter][0];
      remainingBassBeats = songBassNotes[bassNoteCounter][1] * subdivisionMultiplier;
      bassNoteCounter++;
      unsigned int len = (1 / tempoBPS * 1000) - notePause; //Calculate the true length of the note.
      if(currentBassNote != 0) notePlayer[0].play(currentBassNote); //Play the bass note.
      delay(len); //Delay for the true length of the note.
      remainingBassBeats--;
      remainingTrebleBeats--;
    } 
    else if (remainingBassBeats != 0 && remainingTrebleBeats == 0){
      notePlayer[1].stop();
      delay(notePause);
      currentTrebleNote = songTrebleNotes[trebleNoteCounter][0];
      remainingTrebleBeats = songTrebleNotes[trebleNoteCounter][1] * subdivisionMultiplier;
      trebleNoteCounter++;
      unsigned int len = (1 / tempoBPS * 1000) - notePause; //Calculate the true length of the note.
      if(currentTrebleNote != 0) notePlayer[1].play(currentTrebleNote); //Play the treble note.
      delay(len); //Delay for the true length of the note.
      remainingBassBeats--;
      remainingTrebleBeats--;
    } 
    else if (remainingBassBeats != 0 && remainingTrebleBeats != 0){
      unsigned int len = (1 / tempoBPS * 1000) - notePause; //Calculate the true length of the note.
      delay(len); //Delay for the true length of the note.
      remainingBassBeats--;
      remainingTrebleBeats--;
    }
  }
  notePlayer[0].stop();
  notePlayer[1].stop();
  trebleNoteCounter = 0;
  bassNoteCounter = 0;
  remainingBassBeats = 0;
  remainingTrebleBeats = 0;
  currentTrebleNote = 0;
  currentBassNote = 0;
  triggered = false;
}

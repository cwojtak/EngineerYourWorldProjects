//dual_tones_threaded
//Connor Wojtak
//2-23-2021
//A program to that outputs auditorily Shostakovich's Second Waltz with a bass and treble line.

#include <Tone.h>
#include <Thread.h>

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
const unsigned int songBassNotes[][2] = {
  {NC3, 1},
  {NC4, 1},
  {NC4, 1},
  {NG3, 1},
  {NC4, 1},
  {NC4, 1},
  {NC3, 1},
  {NC4, 1},
  {NC4, 1},
  {NG3, 1},
  {NC4, 1},
  {NC4, 1},
  {NC3, 1},
  {NE3, 1},
  {NE3, 1}
};

const unsigned int songTrebleNotes[][2] = {
  {0, 12},
  {NDS5, 3}
};

//Rhythm/Time definitions
const unsigned int notePause = 10; //This value is half the true total pause.
const double tempoBPS = 3.00;

//Note counters
unsigned short bassNoteCounter = 0;
unsigned short trebleNoteCounter = 0;
const unsigned short bassNoteTotal = (sizeof(songBassNotes) / sizeof(songBassNotes[0]));
const unsigned short trebleNoteTotal = (sizeof(songTrebleNotes) / sizeof(songTrebleNotes[0]));

//Pin definition
const unsigned int buzzerPin = 12;

//Tone object definition
Tone notePlayer[2];

//Thread definitions
Thread bassThread = Thread();
Thread trebleThread = Thread();

//setup(): program entry-point
void setup() {
  notePlayer[0].begin(buzzerPin); //Set bass player to use buzzerPin.
  notePlayer[1].begin(buzzerPin); //Set treble player to use buzzerPin.

  bassThread.onRun(bassThreadPlayer);
  trebleThread.onRun(trebleThreadPlayer);
  bassThread.setInterval(1);
  trebleThread.setInterval(1);
}

//loop(): called repeatedly after setup()
void loop() {
  if(bassThread.shouldRun()){
    bassThread.run();
  }
  if(trebleThread.shouldRun()){
    trebleThread.run();
  }
}

void bassThreadPlayer(){
  if(bassNoteCounter >= bassNoteTotal){
    return;
  }
  //for(int i = 0; i < (sizeof(songBassNotes) / sizeof(songBassNotes[0])); i++){ //Loop through the set of notes.
    playNote(songBassNotes[bassNoteCounter], notePlayer[0]); //Play the selected note.
    bassNoteCounter++;
  //}
}

void trebleThreadPlayer(){
  if(trebleNoteCounter >= trebleNoteTotal){
    return;
  }
  //for(int i = 0; i < (sizeof(songBassNotes) / sizeof(songBassNotes[0])); i++){ //Loop through the set of notes.
    playNote(songBassNotes[trebleNoteCounter], notePlayer[0]); //Play the selected note.
    trebleNoteCounter++;
  //}
}

//playNote(): plays the specified note structure
void playNote(const unsigned int note[2], Tone player){
  if(note[0] == 0) { return; }
  delay(notePause); //Delay the note for a certain amount of millisecond after the beat.
  unsigned int len = (note[1] / tempoBPS * 1000) - (notePause * 2); //Calculate the true length of the note.
  player.play(note[0]); //Play the note.
  delay(len); //Delay for the true length of the note.
  player.stop(); //End the note.
  delay(notePause); //Delay for a certain amount of milliseconds before the next beat.
}

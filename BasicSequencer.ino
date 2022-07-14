#include "pitches.h"

#define PLAY 2
#define LED 3
#define STOP 4
#define SPEAKER 8
#define OUP 5
#define ODOWN 6
#define END -1

int notes[] = {NOTE_A1,
NOTE_A2,
NOTE_A3,
NOTE_A4,
NOTE_A5,
NOTE_A6,
NOTE_A7,
NOTE_AS1,
NOTE_AS2,
NOTE_AS3,
NOTE_AS4,
NOTE_AS5,
NOTE_AS6,
NOTE_AS7,
NOTE_B0,
NOTE_B1,
NOTE_B2,
NOTE_B3,
NOTE_B4,
NOTE_B5,
NOTE_B6,
NOTE_B7,
NOTE_C1,
NOTE_C2,
NOTE_C3,
NOTE_C4,
NOTE_C5,
NOTE_C6,
NOTE_C7,
NOTE_C8,
NOTE_CS1,
NOTE_CS2,
NOTE_CS3,
NOTE_CS4,
NOTE_CS5,
NOTE_CS6,
NOTE_CS7,
NOTE_CS8,
NOTE_D1,
NOTE_D2,
NOTE_D3,
NOTE_D4,
NOTE_D5,
NOTE_D6,
NOTE_D7,
NOTE_D8,
NOTE_DS1,
NOTE_DS2,
NOTE_DS3,
NOTE_DS4,
NOTE_DS5,
NOTE_DS6,
NOTE_DS7,
NOTE_DS8,
NOTE_E1,
NOTE_E2,
NOTE_E3,
NOTE_E4,
NOTE_E5,
NOTE_E6,
NOTE_E7,
NOTE_F1,
NOTE_F2,
NOTE_F3,
NOTE_F4,
NOTE_F5,
NOTE_F6,
NOTE_F7,
NOTE_FS1,
NOTE_FS2,
NOTE_FS3,
NOTE_FS4,
NOTE_FS5,
NOTE_FS6,
NOTE_FS7,
NOTE_G1,
NOTE_G2,
NOTE_G3,
NOTE_G4,
NOTE_G5,
NOTE_G6,
NOTE_G7,
NOTE_GS1,
NOTE_GS2,
NOTE_GS3,
NOTE_GS4,
NOTE_GS5,
NOTE_GS6,
NOTE_GS7,
};

int melody[] = {
NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_A4,
NOTE_G4, NOTE_C5, NOTE_AS4, NOTE_A4,                   
NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_FS4, NOTE_DS4, NOTE_D4,
NOTE_C4, NOTE_D4,0,                                 

NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_A4,
NOTE_G4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_AS4, NOTE_C5, NOTE_AS4, NOTE_A4,      //29               //8
NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_FS4, NOTE_DS4, NOTE_D4,
NOTE_C4, NOTE_D4,0,                                       

NOTE_D4, NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_DS5, NOTE_D5,
NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_C5,
NOTE_C4, NOTE_D4, NOTE_DS4, NOTE_FS4, NOTE_D5, NOTE_C5,
NOTE_AS4, NOTE_A4, NOTE_C5, NOTE_AS4,             //58

NOTE_D4, NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_DS5, NOTE_D5,
NOTE_C5, NOTE_D5, NOTE_C5, NOTE_AS4, NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_C5, NOTE_G4,
NOTE_A4, 0, NOTE_AS4, NOTE_A4, 0, NOTE_G4,
NOTE_G4, NOTE_A4, NOTE_G4, NOTE_FS4, 0,

NOTE_C4, NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_DS4,
NOTE_C4, NOTE_D4, 0,
NOTE_C4, NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_DS4,
NOTE_C4, NOTE_D4, END

};

int notesPlayed[100];

int noteDurations[] = {       //duration of the notes
8,4,8,4,
4,4,4,12,
4,4,4,4,4,4,
4,16,4,

8,4,8,4,
4,2,1,1,2,1,1,12,
4,4,4,4,4,4,
4,16,4,

4,4,4,4,4,4,
4,4,4,12,
4,4,4,4,4,4,
4,4,4,12,

4,4,4,4,4,4,
2,1,1,2,1,1,4,8,4,
2,6,4,2,6,4,
2,1,1,16,4,

4,8,4,4,4,
4,16,4,
4,8,4,4,4,
4,20,
};
 
int playButton = 0;
int stopButton = 0;
int octaveUpButton = 0;
int octaveUpCtr = 0;
int octaveDownButton = 0;
int octaveDownCtr = 0;
int currentNote = 0;

byte list_end = 0;

int speed = 90;

void setup() {
  Serial.begin(9600);
  pinMode(PLAY, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(SPEAKER, OUTPUT);
  pinMode(STOP, INPUT);
  pinMode(OUP, INPUT);
  pinMode(ODOWN, INPUT);
}

void appendToList(int element)
{
  if(list_end < 100)
  {
    notesPlayed[list_end++] = element;
  }
}

void playMusic(int currentNote)
{
  for (int thisNote = currentNote; melody[thisNote]!=-1; thisNote++) 
  {
    if(digitalRead(STOP) != LOW)
    {
      if(digitalRead(OUP) == LOW)
      {

        Serial.println("Current Note OUP:");
        Serial.println(melody[thisNote]);
        Serial.println("Octave Increasing");
        findNextOctave(thisNote, octaveUpCtr);
      }
      else if(octaveDownButton == LOW)
      {
        Serial.println("Current Note ODOWN:");
        Serial.println(melody[thisNote]);
        Serial.println("Octave Decreasing");
        findPrevOctave(thisNote, octaveDownCtr);
      }
      else
      {
       currentNote = thisNote;
        Serial.println("Current Note:");
        Serial.println(melody[thisNote]);
        int noteDuration = speed*noteDurations[thisNote];
        appendToList(melody[thisNote]);
        tone(SPEAKER, melody[thisNote],noteDuration*.95);
        delay(noteDuration);
        noTone(SPEAKER);
      }
    }
  }     
}

void findNextOctave(int currentNote, int ctr)
{
  int nextOctave;
  for (int thisNote = currentNote; melody[thisNote]!=-1; thisNote++) 
  {
    if(digitalRead(STOP) != LOW)
    {
      if(digitalRead(ODOWN) != LOW)
      {
        for(int i = 0; i<=sizeof(notes); i++)
        {
          if(notes[i] == melody[thisNote])
          {
            nextOctave = i+1;
          }
        }
        int noteDuration = speed*noteDurations[currentNote];
        tone(SPEAKER, notes[nextOctave],noteDuration*.95);
        appendToList(notes[nextOctave]);
        Serial.println("Increasing Octave:");
        Serial.println(notes[nextOctave]);
        delay(noteDuration);
        noTone(SPEAKER);
      }
      else
      {
        findPrevOctave(currentNote, ctr);
      }
    }
  }
}

void findPrevOctave(int currentNote, int ctr)
{
int prevOctave;
  for (int thisNote = currentNote; melody[thisNote]!=-1; thisNote++) 
  {
    if(digitalRead(STOP) != LOW)
    {
      if(digitalRead(OUP) != LOW)
      {
        for(int i = 0; i<=sizeof(notes); i++)
        {
          if(notes[i] == melody[thisNote])
          {
            prevOctave = i-1;
          }
        }
        int noteDuration = speed*noteDurations[currentNote];
        tone(SPEAKER, notes[prevOctave],noteDuration*.95);
        appendToList(notes[prevOctave]);
        Serial.println("Decreasing Octave:");
        Serial.println(notes[prevOctave]);
        delay(noteDuration);
        noTone(SPEAKER);
      }
      else
      {
        findNextOctave(currentNote, ctr);
      }
    }
  }
}

void loop() {
  playButton = digitalRead(PLAY);
  stopButton = digitalRead(STOP);
  octaveDownButton = digitalRead(ODOWN);
  octaveUpButton = digitalRead(OUP);

  if(playButton == LOW)
  {
    digitalWrite(LED, HIGH);
    Serial.print("ON button pressed");
    Serial.println("LED IS ON");
    digitalWrite(SPEAKER, HIGH);
    playMusic(currentNote);
  }
  else if(playButton == HIGH)
  {
    digitalWrite(SPEAKER, LOW);
    digitalWrite(LED, (HIGH)^1);
  }

  if(stopButton == LOW)
  {
    digitalWrite(SPEAKER, LOW);
    noTone(8);
    digitalWrite(LED, LOW);
    Serial.println("STOP button pressed");
    Serial.println("LED IS OFF");
    Serial.println("NOTES PLAYED");
    for(int i = 0; i <= sizeof(notesPlayed); i++)
    {
      Serial.println(notesPlayed[i]);
    }
  }

 
}

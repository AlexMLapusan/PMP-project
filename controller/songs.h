#include <Volume.h>
#include "pitches.h"

Volume vol;

//Mario main theme melody
int mario[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo ( duration in miliseconds )
int marioTempo[] = {
  83, 83, 83, 83,
  83, 83, 83, 83,
  83, 83, 83, 83,
  83, 83, 83, 83,

  83, 83, 83, 83,
  83, 83, 83, 83,
  83, 83, 83, 83,
  83, 83, 83, 83,

  100, 100, 100,
  83, 83, 83, 83,
  83, 83, 83, 83,
  83, 83, 83, 83,

  83, 83, 83, 83,
  83, 83, 83, 83,
  83, 83, 83, 83,
  83, 83, 83, 83,

  100, 100, 100,
  83, 83, 83, 83,
  83, 83, 83, 83,
  83, 83, 83, 83,
};






//NEVER GONNA GIVE YOU UP

int never[] =
{ NOTE_AS4, NOTE_AS4, NOTE_GS4, NOTE_GS4,
  NOTE_F5, NOTE_F5, NOTE_DS5, NOTE_AS4, NOTE_AS4, NOTE_GS4, NOTE_GS4, NOTE_DS5, NOTE_DS5, NOTE_CS5, NOTE_C5, NOTE_AS4,
  NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5,
  NOTE_CS5, NOTE_DS5, NOTE_C5, NOTE_AS4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_DS5, NOTE_CS5,
  NOTE_AS4, NOTE_AS4, NOTE_GS4, NOTE_GS4,
  NOTE_F5, NOTE_F5, NOTE_DS5, NOTE_AS4, NOTE_AS4, NOTE_GS4, NOTE_GS4, NOTE_GS5, NOTE_C5, NOTE_CS5, NOTE_C5, NOTE_AS4,
  NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5,
  NOTE_CS5, NOTE_DS5, NOTE_C5, NOTE_AS4, NOTE_GS4, 0, NOTE_GS4, NOTE_DS5, NOTE_CS5, 0
};

int neverTempo[] =
{ 1, 1, 1, 1,
  3, 3, 6, 1, 1, 1, 1, 3, 3, 3, 1, 2,
  1, 1, 1, 1,
  3, 3, 3, 1, 2, 2, 2, 4, 8,
  1, 1, 1, 1,
  3, 3, 6, 1, 1, 1, 1, 3, 3, 3, 1, 2,
  1, 1, 1, 1,
  3, 3, 3, 1, 2, 2, 2, 4, 8, 4
};


//ALL STAR
int allStar[] = { NOTE_AS4, NOTE_FS4, NOTE_FS4, NOTE_E4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_E4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_AS4, 0, NOTE_AS4, NOTE_CS5, NOTE_B4, NOTE_CS5, NOTE_DS5, NOTE_FS5, NOTE_FS5, NOTE_GS5, NOTE_FS5, NOTE_FS4, NOTE_FS4, NOTE_GS4, NOTE_FS4, NOTE_AS4, NOTE_GS4, NOTE_GS4, NOTE_FS4, NOTE_GS4, NOTE_GS4, NOTE_AS4, NOTE_DS4,NOTE_DS4 }; //Tones/notes
int allStarTempo[] = { 50, 100, 25, 25, 50, 100, 25, 25, 50, 100, 100, 100, 25, 100, 100, 50, 50, 50, 50, 50, 50, 100, 50, 50, 50, 50, 50, 100, 100, 100, 50, 50, 50, 200, 50 }; //Tone/note lengths*/


int beethovenNotes[] = {

  
  NOTE_E4,  NOTE_E4,  NOTE_F4,  NOTE_G4,//1
  NOTE_G4,  NOTE_F4,  NOTE_E4,  NOTE_D4,
  NOTE_C4,  NOTE_C4,  NOTE_D4,  NOTE_E4,
  NOTE_E4, NOTE_D4,  NOTE_D4,
  
  NOTE_E4,  NOTE_E4,  NOTE_F4,  NOTE_G4,//4
  NOTE_G4,  NOTE_F4,  NOTE_E4,  NOTE_D4,
  NOTE_C4,  NOTE_C4,  NOTE_D4,  NOTE_E4,
  NOTE_D4,  NOTE_C4,  NOTE_C4,

  NOTE_D4,  NOTE_D4,  NOTE_E4,  NOTE_C4,//8
  NOTE_D4,  NOTE_E4,  NOTE_F4,  NOTE_E4, NOTE_C4,
  NOTE_D4,  NOTE_E4,  NOTE_F4,  NOTE_E4, NOTE_D4,
  NOTE_C4,  NOTE_D4,  NOTE_G3,

  NOTE_E4,  NOTE_E4,  NOTE_F4,  NOTE_G4,//12
  NOTE_G4,  NOTE_F4,  NOTE_E4,  NOTE_D4,
  NOTE_C4,  NOTE_C4,  NOTE_D4,  NOTE_E4,
  NOTE_D4,  NOTE_C4,  NOTE_C4
  
};

int beethovenNotesDuration[] = {

  4,  4,  4,  4,//1
  4,  4,  4,  4,
  4,  4,  4,  4,
  6, 2,  8,

  4,  4,  4,  4,//4
  4,  4,  4,  4,
  4,  4,  4,  4,
  6,  2,  8,

  4,  4,  4,  4,//8
  4,  2,  2,  4, 4,
  4,  2,  2,  4, 4,
  4,  4,  8,

  4,  4,  4,  4,//12
  4,  4,  4,  4,
  4,  4,  4,  4,
  6,  2,  8
  
};

void playMario(){
    Serial1.println("Mario Theme\n");
    int size = sizeof(mario) / sizeof(int);
    int _volume = 255;
    for (int thisNote = 0; thisNote < size; thisNote++) 
    {
      if(mario[thisNote] == 0){
        _volume = 1;
      }else{
        _volume = 255;
      }
      int noteDuration = marioTempo[thisNote] ;
      int pauseBetweenNotes = noteDuration * 1.30;
      vol.tone(mario[thisNote], _volume);
      vol.delay(noteDuration);
      vol.tone(20000,1);
      vol.delay(pauseBetweenNotes);
    }

}

void playNever()
{

    Serial1.println("Never gonna give you up\n");
    int size = sizeof(never) / sizeof(int);
    int _volume = 255;
    for (int thisNote = 0; thisNote < size; thisNote++) 
    {
      if(never[thisNote] == 0){
        _volume = 1;
      }else{
      _volume = 255;
      } 
      int noteDuration = neverTempo[thisNote] * 50;
      int pauseBetweenNotes = noteDuration * 1.30;
     
      vol.tone(never[thisNote], _volume);
      vol.delay(noteDuration);
     
      vol.tone(20000,1);
      vol.delay(pauseBetweenNotes);
    }
}

void playBeethoven()
{
    Serial1.println("Ode to Joy - Beethoven's Symphony No. 9\n");
    int size = sizeof(beethovenNotes) / sizeof(int);
    int _volume = 255;
    for (int thisNote = 0; thisNote < size; thisNote++) 
    {
      if(beethovenNotes[thisNote] == 0){
        _volume = 1;
      }else{
      _volume = 255;
      } 
      int noteDuration = beethovenNotesDuration[thisNote] * 50;
      int pauseBetweenNotes = noteDuration * 1.30;
     
      vol.tone(beethovenNotes[thisNote], _volume);
      vol.delay(noteDuration);
     
      vol.tone(20000,1);
      vol.delay(pauseBetweenNotes);
    }
}

void playRandomTune(){
  vol.begin();
  delay(50);
	int song = random(4);
	switch (song){
      case 0:
        playMario(); 
        break;
      case 1:
        playNever();
        break;
	    case 2:
        playBeethoven();
        break;
      default: break;
    }
  delay(50);
}

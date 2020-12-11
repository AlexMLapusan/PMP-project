#include <Volume.h>
#include "pitches.h"
#include "songs.h"

#define LED_PIN 50
#define INTERRUPT_PIN 3

Volume vol;
  
int response = 0;
boolean ledOn = false;

int command;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(3), sendNotification, FALLING);
  vol.begin();
  boolean start = false;
  // initialize both serial ports:
  Serial.begin(115200);
  Serial3.begin(115200);
  Serial.println("Waiting for the start signal");
  //this bit is necessary to 
  while(!start){ 
    response = Serial3.read();
    if(response != -1)
      Serial.println(response);
    if(response == 66){           //the esp will send a 66 after it successfully connected to a wi-fi network
      Serial.println("Got it!"); 
      start = true;
    }
  }

  delay(10);
  Serial1.begin(115200);          //we must only start the serial port that interacts both ways with the esp only AFTER it finished setting up
}

void loop() {
  Serial.println(digitalRead(3));
  if(Serial1.available()){
    command = Serial1.read();
    switch (command){
      case 1:
        handleLed(); 
        break;
      case 2:
        playNever();
        break;
      default: break;
    }
    Serial.println(command);
  }
  delay(10);
}

void handleLed(){
  Serial.println("Lighting up led.");
  if(!ledOn){
    ledOn = true;
    digitalWrite(LED_PIN, HIGH);
  }else{
    ledOn = false;
    digitalWrite(LED_PIN, LOW);  
  }
}

void sendNotification(){
  Serial1.println("Interrupted!");
  Serial1.write(69);
}

void playMario()
{
    Serial.println(" 'Mario Theme'");
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
      Serial.println(mario[thisNote]);
      vol.tone(mario[thisNote], _volume);
      Serial.println("tone works");
      vol.delay(noteDuration);
      Serial.println("delay works");
      vol.tone(20000,1);
      vol.delay(pauseBetweenNotes);
    }

}
void playNever()
{
    Serial.println(" 'Never gonna give you up'");
    int size = sizeof(never) / sizeof(int);
    int _volume = 255;
    for (int thisNote = 0; thisNote < size; thisNote++) 
    {
      if(never[thisNote] == 0){
        _volume = 1;
      }else{
      _volume = 255;
      }
      //to calculate the note duration simply multiply the tempo with 50   
      int noteDuration = neverTempo[thisNote] * 50;
      int pauseBetweenNotes = noteDuration * 1.30;
     
      vol.tone(never[thisNote], _volume);
      vol.delay(noteDuration);
     
      //vol.noTone() function is buggy, so we will simulate like this
      vol.tone(20000,1);
      vol.delay(pauseBetweenNotes);
    }

}

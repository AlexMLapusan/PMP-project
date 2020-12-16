#include "songs.h"

#define LED_PIN 50
#define PWM_LED_PIN 44
#define PHOTOCELL_PIN A10
#define INTERRUPT_PIN 3
  
int response = 0;
boolean ledOn = false;
boolean lightsOn = false;
int decodedCommand = -1;
int command = "";
int lightLevel = 1024;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PWM_LED_PIN, OUTPUT);
  pinMode(PHOTOCELL_PIN, INPUT);
  pinMode(3, INPUT_PULLUP);
  setLedBrightness(255);
  attachInterrupt(digitalPinToInterrupt(3), turnOffLights, FALLING);
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

String waitForCommand(){
  String command = "";
  while(!Serial1.available()){
    if(!lightsOn){
      lightLevel = analogRead(PHOTOCELL_PIN);
      Serial.println(lightLevel);
      if (lightLevel < 5){
        Serial1.write(69); //send the "lights on" command to the esp which will in turn send a request for a notification
        lightsOn = true;
      }
    }
    delay(10);
  }; //busy waiting
  command = Serial1.readStringUntil('\n');
  return command;
}

void loop() {
//  Serial.println(digitalRead(3));
  String command = waitForCommand();
  Serial.println(command);
  decodedCommand = decodeCommand(command);
  Serial.println(decodedCommand);
  switch (decodedCommand){
    case 1: 
      handleLed(); 
      break;
    case 2:
      playRandomTune();
      break;
    case 3:
      while(!Serial1.available()); //busy waiting
      int brightness = Serial1.read();
      setLedBrightness(brightness);
      break;
    default: break;
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

void setLedBrightness(int brightness){
  Serial.print("Led brightness: ");
  Serial.println(brightness);
  analogWrite(PWM_LED_PIN, brightness);
}

void turnOffLights(){
  lightsOn = false;
}

int decodeCommand(String command){
  Serial.print(command);
  if(command == "toggleLed") return 1;
  if(command == "singSong") return 2;
  if(command == "setBrightness") return 3;
  return -1;
}

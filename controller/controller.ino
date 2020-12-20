#include "songs.h"

#define LED_PIN 40
#define PWM_LED_PIN 44
#define PHOTOCELL_PIN A10
#define INTERRUPT_PIN 3
#define LIGHT_LOWER_LIMIT 5
#define LIGHT_UPPER_LIMIT 30

int response = 0;
boolean lightsOn = false;
int decodedCommand = -1;
int command = "";
int lightLevel = 1024;

void setup() {
  //setup
  randomSeed(analogRead(0));
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PWM_LED_PIN, OUTPUT);
  pinMode(PHOTOCELL_PIN, INPUT);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(3), manuallyTurnOffLights, FALLING);
  
  //initializations
  setLedBrightness(255);
  lightsOn = false;
  digitalWrite(LED_PIN, LOW);
  boolean start = false;
  
  //The following usage of 2 different ports is necesssary because of the way the connections between the ESP and the arduino board is made
  //I.E. the RX and TX of the ESP must be connected to the TX and RX of the board HOWEVER if they are connected while the ESP is being set up
  //they will interfer and so it will result in a fail. To avoid that I connected the TX pin of the ESP to the third serial receiver of the arduino mega board
  //and I sent "66" as an "all good" code after the ESP has finished its setup. Upon receiving it, we can safely turn on the Serial1 port in order to communicate with the ESP 
  // initialize both serial ports:
  Serial.begin(115200);
  Serial3.begin(115200);
  Serial.println("Waiting for the start signal");
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
    lightLevel = analogRead(PHOTOCELL_PIN);
//    if(lightLevel < LIGHT_LOWER_LIMIT || lightLevel > LIGHT_UPPER_LIMIT)
//      Serial.println(lightLevel);
//    Serial.println(lightsOn);
    if(lightsOn){
      if (lightLevel > LIGHT_UPPER_LIMIT){
        Serial1.print("turn_off_lights\n");    //send the "lights off" command to the esp which will in turn send a request for a notification
        delay(500);
        lightsOn = false;
      }
    }
    else{
      if (lightLevel < LIGHT_LOWER_LIMIT){
        Serial1.print("turn_on_lights\n");  //send the "lights on" command to the esp which will in turn send a request for a notification
        delay(500);
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
  decodedCommand = decodeCommand(command); //After getting the command (which is a string) this function turns it into a number so that I can use a case statement
//  Serial.println(decodedCommand);
  switch (decodedCommand){
    case 0: 
      turnOnLights();
      break;
    case 1: 
      turnOffLights(); 
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

void turnOnLights(){
  digitalWrite(LED_PIN, HIGH);
  lightsOn = true;
//  Serial1.print("lights_notice\n");
//  delay(10);
//  Serial1.print("on\n");
}

void turnOffLights(){
  digitalWrite(LED_PIN, LOW);  
  lightsOn = false;
//  Serial1.print("lights_notice\n");
//  delay(10);
//  Serial1.print("off\n");
}

void setLedBrightness(int brightness){
  Serial.print("Led brightness: ");
  Serial.println(brightness);
  analogWrite(PWM_LED_PIN, brightness);
}

int decodeCommand(String command){
  Serial.print(command);
  if(command == "toggle_on") return 0;
  if(command == "toggle_off") return 1;
  if(command == "singSong") return 2;
  if(command == "setBrightness") return 3;
  return -1;
}

void manuallyTurnOffLights(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200)
  {
    Serial1.print("turn_off_lights\n");    //send the "lights off" command to the esp which will in turn send a request for a notification
    lightsOn = false;
  }
  last_interrupt_time = interrupt_time;
}

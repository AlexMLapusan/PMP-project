int ledPin = 50;
int response = 0;
boolean ledOn = false;

int command;

void setup() {
  pinMode(ledPin, OUTPUT);
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
  if(Serial1.available()){
    command = Serial1.read();
    switch (command){
      case 1:
        handleLed(); 
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
    digitalWrite(ledPin, HIGH);
  }else{
    ledOn = false;
    digitalWrite(ledPin, LOW);  
  }
}

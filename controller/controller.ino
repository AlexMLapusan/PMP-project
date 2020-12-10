int response = 0;

void setup() {
 
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
//  Serial.println(digitalRead(3));
  delay(10);
}

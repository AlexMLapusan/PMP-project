#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

ESP8266WiFiMulti wifiNetworks;     // ESP8266wifiNetworks helps in case we want to have a list of possible wifi networks

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

void rootHandler();              //http handlers
void _404Page();

void setup(void){
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');
  
  wifiNetworks.addAP("HUAWEI-gSeE", "n8qEw9E5");   // add Wi-Fi networks you want to connect to
    
  Serial.println("Connecting:");
  
  while (wifiNetworks.run() != WL_CONNECTED) { // connect to the strongest network avaibale (thank you ESP8266wifiNetworks)
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());        //get network name             
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());     //get network IP      
  
  server.on("/", HTTP_GET, rootHandler);     // call the rootHandler when someone accesses the base url
  server.on("/LED", HTTP_GET, ledHandler);      //test a simple get request
  server.on("/play_tune", HTTP_GET, playTune);
  server.on("/setBrightness", HTTP_POST, pwmHandler);
  
  server.onNotFound(_404Page);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
  delay(10);
  Serial.write(66); //start code that will be intercepted by the arduino board
//  Serial.print("start\n");
  delay(10);
}

void loop(void){
  server.handleClient();                    // Listen for HTTP requests from clients

  if(Serial.available()){
    int received = Serial.read();
    if(received == 69){
      Serial.println("The button was pushed");
      sendNotification();
    }
  }
}

void rootHandler() {                         // When URI / is requested, send a web page with a button to toggle the LED
  server.send(200, "text/html", "<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>");
}

void ledHandler(){
  delay(10);
  Serial.print("toggleLed\n");  
  server.send(200, "text/html", "Led toggle command received.");
}

void playTune(){
//  Serial.println("Playing a random tune!");
  delay(10);
  Serial.print("singSong\n");
  server.send(200, "text/html", "Command to sing a song received.");
}

void pwmHandler(){
  delay(10);
  
  if(! server.hasArg("value")){
    Serial.print(server.args());
    server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
    return;
  }
  int brightness = server.arg("value").toInt();
  Serial.print("setBrightness\n");
  delay(10);
  Serial.write(brightness);  
  server.send(200, "text/html", "Led brightness changed.");
}
void _404Page(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void sendNotification(){
  Serial.println("Sending the request, check the phone.");
  HTTPClient http;  //Declare an object of class HTTPClient
  http.begin("http://maker.ifttt.com/trigger/button_pressed/with/key/batTuFYn3swI5si3qZy4J0pIkoVtVkpLtJGy_gwLCjE");  //Specify request destination
  
  int httpCode = http.GET();                      //Send the request
   
  if (httpCode > 0) { //Check the returning code
    String payload = http.getString();            //Get the request response payload
    Serial.println(payload);                      //Print the response payload
  }
   
  http.end();   //Close connection
}

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

ESP8266WiFiMulti wifiNetworks;     // ESP8266wifiNetworks helps in case we want to have a list of possible wifi networks

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

String pageLink = "http://654cc130ba61.ngrok.io"; //link provided by Ngrok

String command="";

String lightsStatus = "off";
String PWMValue = "255";
String lastTune = "None";

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
  server.on("/LED", HTTP_POST, ledHandler);     
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
  while(!Serial.available()){
    server.handleClient();                    // Listen for HTTP requests from clients  
  };
  command = Serial.readStringUntil('\n');
  
//  Serial.println("Sending "+ command +" command");
  switchHandler(command); //command can be "turn_on_lights" or "turn_off_lights"
                          //the command will be send to IFTTT which will send another request acordingly
  
}

void rootHandler() {                         // When URI / is requested, send a web page
  String page = "<head>"
  "<title>Status</title>"
//  "<meta http-equiv=\"refresh\" content=\"60\">"
  "<meta name=\"Alex L.\" content=\"\">"
  "<script src=\"http://code.jquery.com/jquery-latest.min.js\"></script>"
  "</head>"
  "<body>"
    "<div class=\"container\">"
      "<div id=\"lights_status\"> Lights: " + lightsStatus + "  </div> "
        "<button id = \"off\"type=\"button\">Turn off</button> "
        "<button id = \"on\"type=\"button\">Turn on</button> "
      "<div> PWM led value: " + PWMValue + " </div>"
      "<div> Last sung tune:" + lastTune + "</div>"
    "</div>"
    "<script>"
      "const settings = {"
        "\"url\": \"" + pageLink + "/LED\","
        "\"method\": \"POST\","
        "\"timeout\": 0,"
        "\"headers\": {"
        "\"Content-Type\": \"application/x-www-form-urlencoded\""
      "},"
    "};"  
    "jQuery(\"#on\").click(()=>{"
      "settings[\"data\"] = {\"value1\" : \"on\"};"
      "jQuery.ajax(settings).done(jQuery(\"#lights_status\").html(\"Lights: on\"));"
    "});"
    "jQuery(\"#off\").click(()=>{"
      "settings[\"data\"] = {\"value1\" : \"off\"};"
      "jQuery.ajax(settings).done(jQuery(\"#lights_status\").html(\"Lights: off\"));"
    "});"
  "</script>"
  "</body>";
  server.send(200, "text/html", page);
}



/*This function handles the request responsible with turning the lights on/off*/
void ledHandler(){
  delay(10);
  if(! server.hasArg("value1")){
    Serial.print(server.args());
    server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
    return;
  }
  
  String postData = server.arg("value1");  //value1 argument should "on" or "off"
  Serial.print("toggle_"+postData+"\n");  
   
  sendNotification("Lights turned " + postData);
  
  lightsStatus = postData; //update data for the root page
  
  server.send(200, "text/html", "Led toggle command received.");
}

void playTune(){
//  Serial.println("Playing a random tune!");
  delay(10);
  Serial.print("singSong\n");
  while(!Serial.available()){
    server.handleClient();                    // Listen for HTTP requests from clients  
  };
  lastTune = Serial.readStringUntil('\n');
  server.send(200, "text/html", "Command to sing a song received.");
}

/*This function handles the request responsible with commanding the arduino to play a random tune on the speaker*/
void pwmHandler(){
  delay(10);
  
  if(! server.hasArg("value")){
    Serial.print(server.args());
    server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
    return;
  }
  PWMValue = server.arg("value");
  int brightness = PWMValue.toInt();
  Serial.print("setBrightness\n");
  delay(10);
  if(brightness < 0){
    brightness = 0;
  }
  if(brightness > 255){
    brightness = 255;
  }
  Serial.write(brightness);
  server.send(200, "text/html", "Led brightness changed.");
}

void _404Page(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

/*Function responsible to send a GET request to IFTTT using webhooks to catch that request
  The command has one of the following values: "turn_on_lights", "turn_off_lights"
*/
void switchHandler(String command){
  HTTPClient http;  //Declare an object of class HTTPClient
  http.begin("http://maker.ifttt.com/trigger/" + command + "/with/key/byWzVFB5tx8JiCI4v9vWGI");  //Specify request destination
  
  int httpCode = http.GET();                      //Send the request
   
  if (httpCode > 0) { //Check the returning code
    String payload = http.getString();            //Get the request response payload
//    Serial.println(payload);                      //Print the response payload, this was used while debugging
  }
 
  http.end();   //Close connection
}

//TODO: make status web page

/*Function responsible to send a GET request to IFTTT using webhooks which will as a result send a notification to the phone
  The message we want to be displayed in the notification is passed as a parameter to this function
*/
void sendNotification(String message){
//  Serial.println("Sending the notification with message: "+ message);
  HTTPClient http;  //Declare an object of class HTTPClient
  http.begin("http://maker.ifttt.com/trigger/notification/with/key/batTuFYn3swI5si3qZy4J0pIkoVtVkpLtJGy_gwLCjE");  //Specify request destination

  http.addHeader("Content-Type", "application/json");
  
  int httpCode = http.POST("{\"value1\":\"" + message + "\", \"value2\":\"" + pageLink + "\"}" );                    //Send the request
   
  if (httpCode > 0) { //Check the returning code
    String payload = http.getString();            //Get the request response payload
//    Serial.println(payload);                      //Print the response payload (useful while debugging)
  }
   
  http.end();   //Close connection
}

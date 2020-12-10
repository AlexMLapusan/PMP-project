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

  server.onNotFound(_404Page);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
  delay(10);
  Serial.write(66); //start code that will be intercepted by the arduino board
  delay(10);
}

void loop(void){
  server.handleClient();                    // Listen for HTTP requests from clients
  
}

void rootHandler() {                         // When URI / is requested, send a web page with a button to toggle the LED
  server.send(200, "text/html", "<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>");
}

void _404Page(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

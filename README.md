# PMP-project

This project is meant to provide a starting point in working with and ESP8266 and an arduino Mega board. The main focus is on the serial communication between the Wi-Fi module and the arduino board, as well as the communication between the ESP and "the actual internet".

# Prerequisites

For this particular project I used the following:
* [ESP8266](https://www.robofun.ro/wireless/esp8266-wifi.html) 
* [Programmer UART USB ESP-01 ESP-8266](https://filafill.com/en/home/1518-programator-uart-usb-esp-01-esp-8266.html) 
* [Arduino Mega board](https://filafill.com/en/electronics/71-development-board-mega-2560-arduino-compatible.html?search_query=arduino+mega&results=118)
* Simple photocell
* Speaker
* 2 LEDs
* 3 resistors
* [IFTTT](https://ifttt.com/home) account
* [Ngrok](https://ngrok.com/product) ready to go

# Functionalities

The functionalities are simple examples of what can be achieved using different type of requests (POST/GET) made by or to the Wi-Fi module.
I created some specific applets using IFTTT. For those applets I used IFTTT's integration with Cortana, Webhooks and app notifications (you need to have their app installed on your phone). 
For Cortana I added the following commands:
1. "Set brightness to <*number*>" which will set the PWM value of an output pin to which I connected an LED (it could have been anything that works with a pwm input). This usage could be extended for example to control a fan.
1. "Sing something" which picks a random tune from the ones defined in the songs.h file. I will not get into details about how the songs are actually played on the speaker since it is beyond the purpose of this project. For more information about the library "Volume.h" please refer to [this link](https://github.com/connornishijima/arduino-volume1). You can find many more tunes [here](https://github.com/robsoncouto/arduino-songs).

Moving on to the more convoluted part of the project is the control of an LED using APIs (I am using a simple LED for lack of more suitable materials, however it could easily be replaced with an actual source of light such as a lamp controlled with the help of a relay).
The "lights" can be turned on/off in 3 ways. There's an automatic mode where the "lights" are turned on/off according to the readings of a photocell, they can be turned off from a physical button connected to the board (that has a function attached to it using attachInterrupt(), I do not recommend this since it is a little buggy sometimes) and last but not least we can control the lights from the status web page (I will describe how to access this page later on).

# [Ngrok](https://ngrok.com/product)

Ngrok can expose any local server to the public internet fast and easy. After getting the public link to your local server (explanations on how to do that can be found on their site) we need to replace it in the applets made on IFTTT and in the ESP's code ("pageLink" variable). In addition they offer a very useful inspect tool available at "127.0.0.1:4040/inspect".
The reason using ngrok is required is due to the fact that IFTTT can't make requests to local servers nor can it receive any from them.

# Data flow

First things first, the ESP module and the Arduino Mega board are programmed separately, however they need to communicate with each other. I explained in the code's comments in more detail how this communication is done so I will keep it out of the README.
I will now summarize how every command works:
1. "Set brightness to <*number*>": IFTTT sends a POST request with a single value in its body, the pair {"value":number}. The link of the request is "link_provided_by_ngrok/setBrightness"
2. "Sing something" is a simple GET request to "link_provided_by_ngrok/play_tune"
3. The LED toggling is more interesting since it makes use of the ESP as a client as well as a server. When the readings on the photocell are low enough the board sends a serial command to the ESP which intercepts it and sends a GET request to IFTTT (working as a client). As a consequence, IFTTT sends a request back to the ESP (who now acts as a HOST). The request is a POST request with one parameter {"value1":"on"/"off"}. The esp receives the request and sends the corresponding command to the board.
As an addition, when the status of the lights change the ESP also sends a POST request to IFTTT with 2 parameters. The request has as a result the display of a notification on the phone. The parameters are as follows: the first one is the message displayed and the second one is the link to which the user should be redirected should the notification be clicked (In my case the root address). Link for the request: "link_provided_by_ngrok/LED". 

All these can be separately tested with the help of any service capable to make web requests. I used [Postman](https://www.postman.com/) for this purpose.
Accessing the root address will display a very simple page with 3 stats that also has 2 buttons used to turn the lights on/off. (Must be refreshed after making changes)

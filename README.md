# garageDooorGA
Garage door opener using Google Assistant, largely based on garageDoorNotifier

### Setup
* Download Arduino IDE https://www.arduino.cc/en/main/software
* Install the ESP8266 thing Addon
  *  File > Preferences > Additional Board Manager URLs
  *  Add this http://arduino.esp8266.com/stable/package_esp8266com_index.json
  *  Ok > Ok 
  * Tools > Boards > Boards Manager. There should be a couple new entries in addition to the standard Arduino boards. Look for esp8266. Click on that entry, then select Install.
  *  Full guide is here: https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/installing-the-esp8266-arduino-addon

### Circuit
The circuit is pretty simple. A GPIO of the Sparkfun Thing ESP8266 activates a relay. The relay is hotwired into a garage door opener button to fake "press" the button. 

### Programming
* Download/clone this github source code
* Open the project in Arduino IDE
* click the WifiCredentials.h tab & enter in your SSID and password
* save
* Connect PC to ESP8266 using USB/DB9
* Tools > Boards > Sparkfun ESP8266 Thing
* Tools > Port > Select your port
* Click upload

### Operation
* The ESP8266 connects to your home wifi & hosts a local website
* Pressing the button on the website triggers an HTTP GET request to the ESP8266. The Code reacts to this request by toggling the relay on & off

//---------------------------------------------------------------
// includes
//---------------------------------------------------------------
#include <ESP8266WiFi.h>
#include "WifiCredentials.h"

// refs : https://randomnerdtutorials.com/esp8266-web-server/

//---------------------------------------------------------------
// macros
//---------------------------------------------------------------
// Comment this out to disable prints and save space 
#define DBG_ENABLED 1
#if DBG_ENABLED
   #define DBG_PRINT(fn, msg) debug_print(fn, msg)
#else
  #define DBG_PRINT(fn, msg) 0; // do nothing   
#endif

//---------------------------------------------------------------
// Types
//---------------------------------------------------------------
typedef unsigned int   UInt32; 
typedef unsigned short UInt16;
typedef unsigned char  UInt8;
typedef int   Int32;
typedef short Int16;
typedef char  Int8;

//---------------------------------------------------------------
// constants
//---------------------------------------------------------------
String mVersionNumber        = "-------VERSION 0.1-------";
const Int32 LED_PIN          = LED_BUILTIN;     // ESP8266 onboard, green LED
const Int32 TOGGLE_DURATION  = 1000;  // full second
const Int32 BUTTON_PIN       = 15;    // button pin

//---------------------------------------------------------------
// member variables
//---------------------------------------------------------------
WiFiServer server(80); // Set web server port number to 80
String header = "";    // Variable to store the HTTP request

//---------------------------------------------------------------
// setup
//
// setup the wifi, pins, and assign interupts
//---------------------------------------------------------------
void setup()
{
  #if DBG_ENABLED
  Serial.begin(115200);
  #endif

  // print the version number
  DBG_PRINT("", ""); // intentional blank line
  DBG_PRINT("", "--------------------------");
  DBG_PRINT("", mVersionNumber );
  DBG_PRINT("", "--------------------------");
  
  // setup the hardware pins
  pinMode(BUTTON_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(BUTTON_PIN, LOW); 
  digitalWrite(LED_PIN, LOW);
  
  // connect to wifi
  DBG_PRINT("setup", " connecting to wifi ");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED_PIN, HIGH); // blink the LED button to indicate connection status
    delay(250);
    digitalWrite(LED_PIN, LOW); // blink the LED button to indicate connection status
    delay(250);

    DBG_PRINT("setup", "." );
  }

  Serial.print("Wifi connecte. ip address = ");
  Serial.println(WiFi.localIP() );

  // set the LED to solid to indicate connected
  digitalWrite(LED_PIN, HIGH); // blink the LED button to indicate connection status
  
  // start the webserver
  server.begin();
}

//---------------------------------------------------------------
// loop
// serve any web requests
//---------------------------------------------------------------
void loop()
{
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) 
  {                             // If a new client connects,
    DBG_PRINT("loop", "New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client

    while (client.connected())  // loop while the client's connected
    {            
      if (client.available())   // if there's bytes to read from the client,
      {             
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;

        if (c == '\n') 
        {
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:

          if (currentLine.length() == 0) 
          {
            // parse any specific requests
            parseHTTPRequest();
            setHTTPHeader(client);

            // Web Page Heading
            client.println("<body><h1>Garage Door Web Server</h1>");

            // Display info for the open/close button of the garage door 
            client.println("<p>Click to open/close the Garage Door </p>");
            client.println("<p><a href=\"/garage/toggle\"><button class=\"button\">OPEN / CLOSE</button></a></p>");
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    DBG_PRINT("loop", "Client disconnected.");
    DBG_PRINT("loop", "");
  }
}

//---------------------------------------------------------------
// parseHTTPRequest
// parse the http request
//---------------------------------------------------------------
void parseHTTPRequest()
{
  // turns the GPIOs on and off

  if (header.indexOf("GET /garage/open") >= 0)
  {
    DBG_PRINT("parseHTTPRequest", "open garage door");
    //toggleGarageDoorButton();
    digitalWrite(LED_PIN, HIGH);
  }
  else if (header.indexOf("GET /garage/close") >= 0)
  {
    DBG_PRINT("parseHTTPRequest", "close garage door");
    //toggleGarageDoorButton();
    digitalWrite(LED_PIN, LOW);
  }
  else if (header.indexOf("GET /garage/toggle") >= 0)
  {
    DBG_PRINT("parseHTTPRequest", "open/close garage door");
    //toggleGarageDoorButton();
    digitalWrite(LED_PIN, LOW);
    delay(500);
    digitalWrite(LED_PIN, HIGH);
  } 
}

//---------------------------------------------------------------
// toggleGarageDoorButton
// press and release the button for the garage door
//---------------------------------------------------------------
void toggleGarageDoorButton()
{
  digitalWrite(BUTTON_PIN, HIGH);
  delay(TOGGLE_DURATION);
  digitalWrite(BUTTON_PIN, LOW);
}

//---------------------------------------------------------------
// setHTTPHeader
// set the http client header
//---------------------------------------------------------------
void setHTTPHeader(WiFiClient &client)
{
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();

  // Display the HTML web page
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  // CSS to style the on/off buttons 
  // Feel free to change the background-color and font-size attributes to fit your preferences
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  client.println(".button2 {background-color: #77878A;}</style></head>");
}

//---------------------------------------------------------------
// debug_print
//
// easy way to enable/disable debug prints. 
// change the macro at the top of the file
//---------------------------------------------------------------
void debug_print(String fn, String msg)
{
  Serial.println(fn + ": " + msg); 
}

//Includes
#include "WiFiEsp.h"
#include <Arduino.h>
#include "Flowerpot.h"
#include "WifiCredentials.h" //ssid und pass als char arrays
#include "PubSubClient.h"

//Defines

//Globale Variablen
int status = WL_IDLE_STATUS;
Flowerpot pots[]= {Flowerpot(1,2,20,10,600),Flowerpot(3,4,20,10,600),Flowerpot(5,6,20,10,600),Flowerpot(7,8,20,10,600)};


void setup()
{
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  Serial3.begin(115200);
  // initialize ESP module
  WiFi.init(&Serial3);

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("Connected!");
}


void loop()
{
  delay(10000);
  Serial.println("Still running!");

}
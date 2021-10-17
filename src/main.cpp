//Includes
#include "WiFiEsp.h"
#include <Arduino.h>
#include "Flowerpot.h"
#include "WifiCredentials.h" //ssid und pass als char arrays
#include "PubSubClient.h"
#include "arduino-timer.h"

//Defines

//Globale Variablen
Timer<10,millis,int> pumptimer;
int status = WL_IDLE_STATUS;
Flowerpot pots[]= {Flowerpot(A3,7,20,10,900,&pumptimer),Flowerpot(A2,6,20,10,900,&pumptimer),Flowerpot(A1,5,20,10,900,&pumptimer),Flowerpot(A0,4,20,10,900,&pumptimer)};


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
//Includes
#include "WiFiEsp.h"
#include <Arduino.h>
#include "Flowerpot.h"
#include "WifiCredentials.h" //ssid und pass als char arrays
#include "PubSubClient.h"
#include "arduino-timer.h"

//Defines
#define MINHUMID 20
#define WATERINGSECONDS 10
#define WATERINGCOOLDOWN 900

//Globale Variablen
Timer<10,millis,int> timer;
int status = WL_IDLE_STATUS;
Flowerpot pots[]= {
  Flowerpot(A3,7,MINHUMID,WATERINGSECONDS,WATERINGCOOLDOWN,&timer),
  Flowerpot(A2,6,MINHUMID,WATERINGSECONDS,WATERINGCOOLDOWN,&timer),
  Flowerpot(A1,5,MINHUMID,WATERINGSECONDS,WATERINGCOOLDOWN,&timer),
  Flowerpot(A0,4,MINHUMID,WATERINGSECONDS,WATERINGCOOLDOWN,&timer)
};


bool checkPots(int a){
  int numPots = sizeof(pots)/sizeof(Flowerpot);
  int humidities[numPots];
  for (int i = 0; i < numPots; i++)
  {
    pots[i].process();
    humidities[i]=pots[i].humidity;
  }
  //TODO: humidities per mqtt senden
  return true;
}

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

  //Timer zum regelmäßig messen und falls nötig gießen
  timer.every(10000,checkPots);

  //TODO: MQTT Setup, callback für wässern und callback für automatik-schalter
  
}

void loop()
{
  timer.tick();
}
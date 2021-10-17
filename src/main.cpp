//Includes
#include "WiFiEsp.h"
#include "WiFiEspClient.h"
#include <Arduino.h>
#include "Flowerpot.h"
#include "Credentials.h"
#include "PubSubClient.h"
#include "arduino-timer.h"

//Defines
#define MINHUMID 20
#define WATERINGSECONDS 10
#define WATERINGCOOLDOWN 900
void mqttCallback(char* topic, byte* payload, unsigned int length);

//Globale Variablen
Timer<10,millis,int> timer;
Flowerpot pots[]= {
  Flowerpot(A3,7,MINHUMID,WATERINGSECONDS,WATERINGCOOLDOWN,&timer),
  Flowerpot(A2,6,MINHUMID,WATERINGSECONDS,WATERINGCOOLDOWN,&timer),
  Flowerpot(A1,5,MINHUMID,WATERINGSECONDS,WATERINGCOOLDOWN,&timer),
  Flowerpot(A0,4,MINHUMID,WATERINGSECONDS,WATERINGCOOLDOWN,&timer)
};
WiFiEspClient wifiClient;
PubSubClient mqtt(mqttIP,1883,mqttCallback,wifiClient);


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

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  //TODO: Was sinnvolles machen
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup()
{
  // USB Serial
  Serial.begin(9600);

  // Serial3 Wemos 2560 Wifi -> ESP866 AT-Firmware
  Serial3.begin(115200);
  WiFi.init(&Serial3);
  int status = WL_IDLE_STATUS;
  while ( status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
  }
  Serial.print("Connected to ");
  Serial.println(ssid);

  //Timer zum regelmäßig messen und falls nötig gießen
  timer.every(10000,checkPots);

  //MQTT
  if (mqtt.connect(mqttID, mqttUser, mqttPass)) {
    mqtt.subscribe("plantWatering");
    mqtt.subscribe("plantAutomatik");
  }
}

void loop()
{
  timer.tick();
  mqtt.loop();
}
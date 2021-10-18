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
#define CHECKINTERVAL 10
void mqttCallback(char* topic, byte* payload, unsigned int length);
bool checkPots(int a);

//Globale Variablen
Timer<10,millis,int> timer;
Flowerpot pots[]= {
  Flowerpot(A3,7,MINHUMID,WATERINGSECONDS,WATERINGCOOLDOWN,&timer),
  Flowerpot(A2,6,MINHUMID,WATERINGSECONDS,WATERINGCOOLDOWN,&timer),
  Flowerpot(A1,5,MINHUMID,WATERINGSECONDS,WATERINGCOOLDOWN,&timer),
  Flowerpot(A0,4,MINHUMID,WATERINGSECONDS,WATERINGCOOLDOWN,&timer)
};
WiFiEspClient wifiClient;
PubSubClient mqtt(mqttIP,mqttPort,mqttCallback,wifiClient);
auto checkTask = timer.every(CHECKINTERVAL*1000,checkPots);


bool checkPots(int a){
  int numPots = sizeof(pots)/sizeof(Flowerpot);
  String humString;
  for (int i = 0; i < numPots; i++)
  {
    pots[i].process();
    humString += pots[i].humidity;
    humString += ";";
  }
  mqtt.beginPublish("plantWatering/humidities",humString.length(),false);
  mqtt.println(humString);
  mqtt.endPublish();
  return true;
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  if (topic == "plantWatering/water") {    
    String firstLetter(payload[0]);
    if (firstLetter.toInt()>0 && firstLetter.toInt()<=4) pots[firstLetter.toInt()-1].water(WATERINGSECONDS);
    else Serial.println("Received some payload I didn't expect on plantWatering/water!");
  } else if (topic == "plantWatering/auto") {
    timer.cancel(checkTask);
    if ((char)payload[0]==1) checkTask = timer.every(CHECKINTERVAL*1000,checkPots);
  } else {
    Serial.println("Received some topic I didn't expect!");
    Serial.print("[");
    Serial.print(topic);
    Serial.print("] ");
    for (unsigned int i=0;i<length;i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
  }
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
  

  //MQTT
  if (mqtt.connect(mqttID, mqttUser, mqttPass)) {
    mqtt.subscribe("plantWatering/water");
    mqtt.subscribe("plantWatering/auto");
  }
}

void loop()
{
  timer.tick();
  mqtt.loop();
}
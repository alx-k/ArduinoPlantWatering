#include "Flowerpot.h"

static bool pumpOff(int pin) {
    digitalWrite(pin,LOW);
    return false;
}

bool Flowerpot::needsWater() {
    if(humidity<minHumid) {
    return true;
    }
    return false;
}

bool Flowerpot::wateringAllowed() {
    if(millis()-lastWatered>waterCool*1000) {
    return true;
    }
    return false;
}

Flowerpot::Flowerpot(uint8_t sensorPin, int pumpPin, int minHumidity, int wateringSeconds, int cooldownSeconds, Timer<10,millis,int>* timer){
    this->sensorPin=sensorPin;
    this->pumpPin=pumpPin;
    this->minHumid=minHumidity;
    this->waterDur=wateringSeconds;
    this->waterCool=cooldownSeconds;
    this->timer=timer;
    init();
}

void Flowerpot::init() {
    this->lastWatered = 0;
    pinMode(sensorPin,INPUT);
    pinMode(pumpPin,OUTPUT);
    pinMode(sensorPowerPin,OUTPUT);
    digitalWrite(pumpPin,LOW);
    digitalWrite(sensorPowerPin,LOW);
    getHumidity();
}

int Flowerpot::getHumidity() { //public für mqtt
    digitalWrite(sensorPowerPin,HIGH);
    int reading = analogRead(sensorPin);
    digitalWrite(sensorPowerPin,LOW);
    //TODO: Mapping
    this->humidity=reading;
    return this->humidity;
}

void Flowerpot::water(int duration) {  //public für mqtt
    digitalWrite(pumpPin,HIGH);
    timer->in(duration*1000,pumpOff,pumpPin);
    this->lastWatered=millis(); //Theoretisch +(duration*1000) für den Endzeitpunkt des Gießens
}

void Flowerpot::process() {
    //messen und falls nötig wässern
    getHumidity();
    if (needsWater()) {
        if (wateringAllowed()) {
            water(waterDur);
        }
    }
}
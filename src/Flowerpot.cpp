#include "Flowerpot.h"

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

Flowerpot::Flowerpot(int sensorPin, int pumpPin, int minHumidity, int wateringSeconds, int cooldownSeconds){
    this->sensorPin=sensorPin;
    this->pumpPin=pumpPin;
    this->minHumid=minHumidity;
    this->waterDur=wateringSeconds;
    this->waterCool=cooldownSeconds;
    init();
}

void Flowerpot::init() {
    this->lastWatered = 0;
    getHumidity();
}

int Flowerpot::getHumidity() { //public für mqtt
    int reading;
    //hier sensor auslesen
    this->humidity=reading;
    return this->humidity;
}

void Flowerpot::water(int duration) {  //public für mqtt
    //hier pumpe laufen lassen
    this->lastWatered=millis();
}

void Flowerpot::compute() {
    //automatisch messen und falls nötig wässern
    getHumidity();
    if (needsWater()) {
        if (wateringAllowed()) {
            water(waterDur);
        }
    }
}

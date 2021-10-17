#ifndef FLOWERPOT_H
#define FLOWERPOT_H
#define POWERPIN 8

#include <Arduino.h>
#include "arduino-timer.h"

class Flowerpot{
  private:
    unsigned long int lastWatered;
    uint8_t sensorPin;
    int pumpPin;
    int minHumid;
    unsigned int waterCool;
    int waterDur;
    int sensorPowerPin = POWERPIN;
    Timer<10,millis,int>* timer;

    bool needsWater();
    bool wateringAllowed();
    int getHumidity();
    void init();


  public:
    int humidity;
    Flowerpot(uint8_t sensorPin, int pumpPin, int minHumidity, int waterSeconds, int cooldown, Timer<10,millis,int>* timer);
    void water(int duration);
    void process();
};

#endif
#ifndef FLOWERPOT_H
#define FLOWERPOT_H

#include <Arduino.h>

class Flowerpot{
  private:
    int humidity;
    unsigned long int lastWatered;
    int sensorPin;
    int pumpPin;
    int minHumid;
    unsigned int waterCool;
    int waterDur;

    bool needsWater();
    bool wateringAllowed();

  public:
    Flowerpot(int sensorPin, int pumpPin, int minHumidity, int waterSeconds, int cooldown);
    void init();
    int getHumidity();
    void water(int duration);
    void compute();
};

#endif
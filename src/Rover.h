#ifndef ROVER_H
#define ROVER_H

#include <Arduino.h>
#include <AccelDC.h>
#include "Environment.h"
#include "config.h"

class Rover{
public:
    Rover(Environment *environment);
    void begin();
    void setMotorsEnabled(bool enabled);
    void setMotorsConfig(float minSpeed, float acceleration);
    void setTargetSpeed(float speed);
    void moveForMillis(unsigned long t);
    void setBackwards(bool backwards);
    void brake();
    bool run();
private:
    Environment *env;
    AccelDC *leftMot;
    //AccelDC *rightMot;
};

#endif
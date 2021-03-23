#ifndef ACCEL_DC_H
#define ACCEL_DC_H

#include <Arduino.h>

class AccelDC{
public:
    AccelDC(uint8_t input1, uint8_t input2, uint8_t enable);
    void setConfig(float minSpeed, float acceleration, float maxSpeed);
    void run();
private:
    uint8_t in1;
    uint8_t in2;
    uint8_t en;
    float minSpd;
    float accel;
    float maxSpd;
    float currentPwm;
};

#endif
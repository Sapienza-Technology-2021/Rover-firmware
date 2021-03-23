#ifndef ACCEL_DC_H
#define ACCEL_DC_H

#include <Arduino.h>

#define TIME_RESOLUTION_MS 10
// mps : 5m/s = pwm : 255
#define MPS_TO_PWM(x) ((int) (x/51))

class AccelDC {
public:
    AccelDC(uint8_t input1, uint8_t input2, uint8_t enable);
    void begin();
    void setEnabled(bool enabled);
    void setConfig(float minSpeed, float acceleration);
    void setTargetSpeed(float speed);
    void moveForMillis(unsigned long t);
    void run();
private:
    int write(float mps);
    uint8_t in1;
    uint8_t in2;
    uint8_t en;
    float currentSpd;
    float minSpd;
    float accel;
    float targetSpd;
    unsigned long lastRunTime;
    unsigned long moveForMillisVal;
    unsigned long moveForMillisCurrent;
};

#endif
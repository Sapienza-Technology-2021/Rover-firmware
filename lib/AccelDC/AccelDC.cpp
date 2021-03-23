#include "AccelDC.h"

AccelDC::AccelDC(uint8_t input1, uint8_t input2, uint8_t enable) {
    in1 = input1;
    in2 = input2;
    en = enable;
    accel = 0;
    minSpd = 0;
}

void AccelDC::setConfig(float minSpeed, float acceleration, float maxSpeed) {
    accel = acceleration;
    minSpd = minSpeed;
    maxSpd = maxSpeed;
}

void AccelDC::run() {
    if (currentPwm < )
}
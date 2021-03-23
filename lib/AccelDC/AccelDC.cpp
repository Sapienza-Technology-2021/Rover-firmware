#include "AccelDC.h"

AccelDC::AccelDC(uint8_t input1, uint8_t input2, uint8_t enable) {
    in1 = input1;
    in2 = input2;
    en = enable;
    accel = 0;
    minSpd = currentSpd = targetSpd = 0;
    moveForMillisVal = 0;
    lastRunTime = 0;
}

void AccelDC::begin() {
    pinMode(in1, OUTPUT);
    analogWrite(in1, LOW);
    pinMode(in2, OUTPUT);
    analogWrite(in2, LOW);
    pinMode(en, OUTPUT);
    analogWrite(en, HIGH);
}

void AccelDC::setEnabled(bool enabled) {
    analogWrite(en, !enabled);
}

int AccelDC::write(float mps) {
    currentSpd = constrain(mps, -targetSpd, targetSpd);
    int val = MPS_TO_PWM(currentSpd);
    if (val >= 0) {
        analogWrite(in2, LOW);
        analogWrite(in1, val);
    } else {
        analogWrite(in1, LOW);
        analogWrite(in2, -val);
    }
}

void AccelDC::setConfig(float minSpeed, float acceleration) {
    accel = acceleration;
    minSpd = minSpeed;
}

void AccelDC::setTargetSpeed(float speed) {
    targetSpd = speed;
}

void AccelDC::moveForMillis(unsigned long t) {
    moveForMillisVal = t;
    moveForMillisCurrent = 0;
}

void AccelDC::run() {
    unsigned long time = millis();
    unsigned long dt = time - lastRunTime;
    if (dt < TIME_RESOLUTION_MS) return;
    lastRunTime = time;

    // Time-based acceleration curve
    if (moveForMillisVal != 0) {
        if (moveForMillisCurrent == 0) {
            write(minSpd);
        } else if (moveForMillisCurrent < (moveForMillisVal / 2)) {
            write(currentSpd + (accel * dt));
        } else if (moveForMillisCurrent < moveForMillisVal) {
            write(currentSpd - (accel * dt));
        } else {
            write(0);
            moveForMillisVal = moveForMillisCurrent = 0;
            return;
        }
        moveForMillisCurrent += dt;
    }
}
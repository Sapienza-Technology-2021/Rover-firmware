#include "Rover.h"

Rover::Rover(Environment *environment) {
    env = environment;
    leftMot = new AccelDC(LEFT_MOT_IN1, LEFT_MOT_IN2, LEFT_MOT_EN);
    //rightMot = new AccelDC();
}

void Rover::begin() {
    leftMot->begin();
    //rightMot->begin();
}

void Rover::setBackwards(bool backwards) {
    leftMot->setBackwards(backwards);
    //rightMot->setBackwards(backwards);
}

void Rover::setMotorsEnabled(bool enabled) {
    Serial.print("LRover setEnabled ");
    Serial.println(enabled ? "true" : "false");
    leftMot->setEnabled(enabled);
    //rightMot->setEnabled(enabled);
}

void Rover::setMotorsConfig(float minSpeed, float acceleration) {
    leftMot->setConfig(minSpeed, acceleration);
    //rightMot->setConfig(minSpeed, acceleration);
}

void Rover::setTargetSpeed(float speed) {
    Serial.print("LRover setTargetSpeed ");
    Serial.println(speed);
    leftMot->setTargetSpeed(speed);
    //rightMot->setTargetSpeed(speed);
}

void Rover::moveForMillis(unsigned long t) {
    Serial.print("LRover moveForMillis ");
    Serial.println(t);
    leftMot->moveForMillis(t);
    //rightMot->moveForMillis(t);
}

void Rover::brake() {
    Serial.println("LRover brake");
    leftMot->brake();
    //rightMot->brake();
}

bool Rover::run() {
    return (leftMot->run()) /*|| (rightMot->run())*/;
}
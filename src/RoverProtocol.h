#ifndef ROVER_H
#define ROVER_H

#include <Arduino.h>
#include "config.h"

extern unsigned long lastRoverSerialSend;

void roverProtocolManage(Rover *rover, Navigation *nav, Environment *env);
boolean roverSerialEvent(Rover *rover, Navigation *nav, Environment *env);

#endif
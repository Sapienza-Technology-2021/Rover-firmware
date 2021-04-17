#include "main.h"

Environment env;
Rover rover(&env);
double sensorBuffer[3];

void setup() {
    Serial.begin(SERIAL_SPEED);
    Serial.setTimeout(SERIAL_TIMEOUT);
    env.begin();
    rover.begin();
    rover.setMotorsConfig(MOTORS_MIN_SPEED, MOTORS_DEF_ACCEL);
}

void loop() {
    rover.run();

    if (env.readDistances()) {
        Serial.print('D');
        Serial.println(env.getDistance1());
    }
    if (env.readIMU()) {
        if (env.getAccel(sensorBuffer)) {
            Serial.print('A');
            printArray(sensorBuffer);
        }
        if (env.getGyro(sensorBuffer)) {
            Serial.print('G');
            printArray(sensorBuffer);
        }
        double tmp = env.getCompass();
        if (!isnan(tmp)) {
            Serial.print('M');
            Serial.print(tmp);
            Serial.print('%');
            Serial.println(rover.getTargetHeading());
        }
        tmp = env.getTemp();
        if (!isnan(tmp)) {
            Serial.print('T');
            Serial.println(env.getTemp(), 2);
        }
    }
}

void serialEvent() {
    do {
        if (Serial.read() == '>') {
            delay(10);
            char rcv = Serial.read();
            Serial.print("LFoundCmd = ");
            Serial.println(rcv);
            switch (rcv) {
                case 'C': {
                    Serial.print("C");
                    Serial.print(UUID);
                    Serial.println();
                    break;
                }

                case 'E': {
                    rover.setMotorsEnabled(Serial.read() == '1');
                    break;
                }

                case 'V': {
                    rover.setTargetSpeed(Serial.parseInt());
                    break;
                }

                case 'T': {
                    long ms = Serial.parseInt();
                    if (ms > 0) {
                        rover.setBackwards(false);
                        rover.moveForMillis(ms);
                    } else if (ms < 0) {
                        rover.setBackwards(true);
                        rover.moveForMillis(-ms);
                    } else {
                        rover.brake();
                    }
                    break;
                }

                case 'S': {
                    rover.brake();
                    break;
                }
            }
        }
    } while (Serial.available());
}

void printArray(double data[]) {
    Serial.print(data[0], 3);
    Serial.print('%');
    Serial.print(data[1], 3);
    Serial.print('%');
    Serial.println(data[2], 3);
}
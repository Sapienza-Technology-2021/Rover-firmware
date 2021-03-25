#include "main.h"

Environment env;
Rover rover(&env);

void setup() {
    Serial.begin(SERIAL_SPEED);
    Serial.setTimeout(SERIAL_TIMEOUT);
    // env.begin();
    rover.begin();
    rover.setMotorsConfig(MOTORS_MIN_SPEED, MOTORS_DEF_ACCEL);
}

void loop() {
    rover.run();

    /*if (env.readSensors()) {
        double* data = env.getAccel();
        Serial.print('A');
        printArray(data);
        Serial.print('%');
        data = env.getGyro();
        Serial.print('G');
        printArray(data);
        Serial.print('%');
        data = env.getCompass();
        Serial.print('M');
        printArray(data);
        float temperature = env.getTemp();
        Serial.print('T');
        Serial.print(temperature, 2);
        Serial.println('%');
    }*/
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
                    rover.moveForMillis(Serial.parseInt());
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

void printArray(double *data) {
    Serial.print(data[0], 3);
    Serial.print('%');
    Serial.print(data[1], 3);
    Serial.print('%');
    Serial.print(data[2], 3);
    Serial.println('%');
}
#include "main.h"

Environment env;
Rover rover(&env);

void setup() {
    Serial.begin(SERIAL_SPEED);
    Serial.setTimeout(SERIAL_TIMEOUT);
    env.begin();
}

void loop() {
    if (env.readSensors()) {
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
    }
}

void serialEvent() {
    while (Serial.available()) {
		if (Serial.read() == '>') {
			Serial.println("LFoundCmd");
			switch (Serial.read()) {
			case 'C': {
				Serial.print("C");
				Serial.print(UUID);
				Serial.println();
				break;
			}
			}
		}
	}
}

void printArray(double* data) {
    Serial.print(data[0], 3);
    Serial.print('%');
    Serial.print(data[1], 3);
    Serial.print('%');
    Serial.print(data[2], 3);
    Serial.println('%');
}
#include "RoverProtocol.h"

void roverProtocolManage(Rover *rover, Navigation *nav, Environment *env) {
	unsigned long currentTime = millis();
	if (currentTime - lastRoverSerialSend >= ROVER_SEND_DELAY) {
		// Spara sensori sulla seriale
		lastRoverSerialSend = currentTime;
	}
}

boolean roverSerialEvent(Rover *rover, Navigation *nav, Environment *env) {
	while (Serial.available()) {
		if (Serial.read() == '$') {
			Serial.println("LFoundCmd");
			switch (Serial.read()) {
			case 'C': {
				Serial.print("C");
				Serial.print(FIRMWARE_VERSION);
				Serial.print(",");
				Serial.print(focuser->getCurrentPos());
				Serial.print(",");
				Serial.print(focuser->getSpeed());
				Serial.print(",");
				Serial.print(focuser->isHoldControlEnabled());
				Serial.print(",");
				Serial.print(focuser->getBacklash());
				Serial.print(",");
				Serial.print(focuser->getDirReverse());
				Serial.print(",");
				Serial.print(ENABLE_DEVMAN);
				Serial.println();
				break;
			}

			case 'H': {
				boolean b = (Serial.parseInt() > 0);
				Serial.print(F("LHoldControl="));
				Serial.println(b);
				focuser->setHoldControlEnabled(b);
				return true;
			}
			}
		}
	}
	return false;
}
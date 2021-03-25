#include <AccelDC.h>

#define IN1 5
#define IN2 11
#define EN 8

AccelDC motor(IN1, IN2, EN);
bool dir = false;

void setup() {
  motor.begin();
  motor.setEnabled(true);
  motor.setConfig(20, 0.005);
  motor.setTargetSpeed(200);
  motor.moveForMillis(15000);
}

void loop() {
  if (!motor.run()) {
    delay(1000);
    dir = !dir;
    motor.setBackwards(dir);
    motor.moveForMillis(8000);
  }
}

#ifndef CONFIG_H
#define CONFIG_H

#define SERIAL_SPEED 115200
#define SERIAL_TIMEOUT 100
#define UUID "4b7caa5d-2634-44f3-ad62-5ffb1e08d73f"

#define BATTERY_PIN 10

// 0 = dummy
// 1 = Adafruit 10 DoF LSM303DLHC+L3GD20+BMP180
// 2 = SparkFun ICM20948
#define IMU_TYPE 1

#define DIST_SENS1_TRIG 7
#define DIST_SENS1_ECHO 6

#define MOTORS_DEF_ACCEL 0.005
#define MOTORS_MIN_SPEED 25

#define LEFT_MOT_IN1 5
#define LEFT_MOT_IN2 11
#define LEFT_MOT_EN 8

#endif
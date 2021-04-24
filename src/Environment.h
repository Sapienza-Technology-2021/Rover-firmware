#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <Arduino.h>
#include <math.h>

#include "config.h"

#if IMU_TYPE == 1
#include <Wire.h>
// IMU library: https://github.com/adafruit/Adafruit_LSM303DLHC
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_BMP085_U.h>
#elif IMU_TYPE == 2
#include <Wire.h>
// IMU library: https://github.com/sparkfun/SparkFun_ICM-20948_ArduinoLibrary
#include <ICM_20948.h>
#endif

#define IMU_READ_INTERVAL 100
#define IMU_INTEGRATION_COUNT 3
#define INVALID_VALUE (0.0 / 0.0)

#define DIST_READ_INTERVAL 400

#define MAGNETIC_DECLINATION 0.0

class Environment {
   private:
#if IMU_TYPE == 1
    Adafruit_LSM303_Accel_Unified imuAccel = Adafruit_LSM303_Accel_Unified(10000);
    Adafruit_LSM303_Mag_Unified imuMag = Adafruit_LSM303_Mag_Unified(10001);
    Adafruit_L3GD20_Unified imuGyro = Adafruit_L3GD20_Unified(10002);
    Adafruit_BMP085_Unified imuBmp = Adafruit_BMP085_Unified(10003);
#elif IMU_TYPE == 2
    ICM_20948_I2C imu;
#endif
    unsigned long lastDistRead;
    unsigned long lastIMURead;
    uint8_t imuIntegrationCount;
    float temperature;
    bool imuReady;
    double imuMatrix[3][3];
    double imuFinalMatrix[3][3];
    double distance1;

   public:
    Environment();

    void begin();

    float readbattery();

    bool readIMU();

    bool readDistances();

    bool getAccel(double array[]);

    bool getGyro(double array[]);

    double getCompass();

    float getTemp();

    double getDistance1();
};

#endif
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <Arduino.h>

#include "config.h"
// IMU library: https://github.com/sparkfun/SparkFun_ICM-20948_ArduinoLibrary
#include "ICM_20948.h"

#define IMU_READ_INTERVAL 100
#define IMU_INTEGRATION_COUNT 3
#define INVALID_VALUE (0.0 / 0.0)

#define DIST_READ_INTERVAL 400

class Environment {
   private:
    ICM_20948_I2C imu;
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

    double *getAccel();

    double *getGyro();

    double *getCompass();

    float getTemp();

    double getDistance1();
};

#endif
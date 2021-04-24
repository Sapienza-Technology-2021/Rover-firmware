#include "Environment.h"

Environment::Environment() {
    lastIMURead = 0;
    imuIntegrationCount = 0;
    imuReady = false;
    temperature = distance1 = 0.0;
}

void Environment::begin() {
#if IMU_TYPE == 1
    imuMag.enableAutoRange(true);
    while (!imuMag.begin()) {
        Serial.println("LIMU non available, trying again...");
        delay(500);
    }
    while (!imuAccel.begin()) {
        Serial.println("LIMU non available, trying again...");
        delay(500);
    }
    while (!imuGyro.begin()) {
        Serial.println("LIMU non available, trying again...");
        delay(500);
    }
    Serial.println("LIMU ok");
#elif IMU_TYPE == 2
    Wire.begin();
    Wire.setClock(400000);
    bool initialized = false;
    while (!initialized) {
        imu.begin(Wire, 1);
        if (imu.status == ICM_20948_Stat_Ok) {
            Serial.println("LIMU ok");
            initialized = true;
        } else {
            Serial.println("LIMU non available, trying again...");
            delay(500);
        }
    }
#endif
    pinMode(DIST_SENS1_TRIG, OUTPUT);
    pinMode(DIST_SENS1_ECHO, INPUT);
}

bool Environment::readIMU() {
    // Adesso filtra i dati con una media di tre valori
    // TODO: provare altri filtri, come passa-basso e Kalman
    unsigned long t = millis();
#if IMU_TYPE == 0
    if (t - lastIMURead >= 2000) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                imuFinalMatrix[i][j] = random(-10, 10);
            }
        }
        imuReady = true;
        lastIMURead = t;
        return true;
    }
    return false;
#elif IMU_TYPE == 1
    if (t - lastIMURead >= IMU_READ_INTERVAL) {
        sensors_event_t event;
        imuAccel.getEvent(&event);
        imuMatrix[0][0] += (double)event.acceleration.x;
        imuMatrix[0][1] += (double)event.acceleration.y;
        imuMatrix[0][2] += (double)event.acceleration.z;
        imuGyro.getEvent(&event);
        imuMatrix[1][0] += (double)event.gyro.x;
        imuMatrix[1][1] += (double)event.gyro.y;
        imuMatrix[1][2] += (double)event.gyro.z;
        imuMag.getEvent(&event);
        imuMatrix[2][0] += (double)event.magnetic.x;
        imuMatrix[2][1] += (double)event.magnetic.y;
        imuMatrix[2][2] += (double)event.magnetic.z;
        /*imuBmp.getEvent(&event);
        Serial.println("ciao4");
        if (event.pressure) {
            imuBmp.getTemperature(&temperature);
        }*/
        imuIntegrationCount++;
        if (imuIntegrationCount == IMU_INTEGRATION_COUNT) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    imuFinalMatrix[i][j] = imuMatrix[i][j] / IMU_INTEGRATION_COUNT;
                    imuMatrix[i][j] = 0;
                }
            }
            imuIntegrationCount = 0;
            imuReady = true;
            return true;
        }
        lastIMURead = t;
        return false;
    }
    return false;
#elif IMU_TYPE == 2
    if ((t - lastIMURead >= IMU_READ_INTERVAL) && imu.dataReady()) {
        imu.getAGMT();
        imuMatrix[0][0] += (double)imu.accX();
        imuMatrix[0][1] += (double)imu.accY();
        imuMatrix[0][2] += (double)imu.accZ();
        imuMatrix[1][0] += (double)imu.gyrX();
        imuMatrix[1][1] += (double)imu.gyrY();
        imuMatrix[1][2] += (double)imu.gyrZ();
        imuMatrix[2][0] += (double)imu.magX();
        imuMatrix[2][1] += (double)imu.magY();
        imuMatrix[2][2] += (double)imu.magZ();
        temperature = imu.temp();
        imuIntegrationCount++;
        if (imuIntegrationCount == IMU_INTEGRATION_COUNT) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    imuFinalMatrix[i][j] = imuMatrix[i][j] / IMU_INTEGRATION_COUNT;
                    imuMatrix[i][j] = 0;
                }
            }
            imuIntegrationCount = 0;
            imuReady = true;
            return true;
        }
        lastIMURead = t;
        return false;
    }
    return false;
#endif
}

bool Environment::readDistances() {
    unsigned long t = millis();
    if (t - lastDistRead >= DIST_READ_INTERVAL) {
        digitalWrite(DIST_SENS1_TRIG, LOW);
        delayMicroseconds(2);
        digitalWrite(DIST_SENS1_TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(DIST_SENS1_TRIG, LOW);
        distance1 = pulseIn(DIST_SENS1_ECHO, HIGH, 30000) * 0.017;
        lastDistRead = t;
        return true;
    }
    return false;
}

double Environment::getDistance1() { return distance1; }

bool Environment::getAccel(double array[]) {
    if (imuReady) {
        array[0] = imuFinalMatrix[0][0];
        array[1] = imuFinalMatrix[0][1];
        array[2] = imuFinalMatrix[0][2];
        return true;
    } else {
        return false;
    }
}

bool Environment::getGyro(double array[]) {
    if (imuReady) {
        array[0] = imuFinalMatrix[1][0];
        array[1] = imuFinalMatrix[1][1];
        array[2] = imuFinalMatrix[1][2];
        return true;
    } else {
        return false;
    }
}

double Environment::getCompass() {
    if (imuReady) {
        double heading = atan2(imuFinalMatrix[2][1], imuFinalMatrix[2][0]);
        double twoPi = 2 * PI;
        if (heading < 0)
            heading += twoPi;
        else if (heading > twoPi)
            heading -= twoPi;
        return (heading * 180.0 / PI) + MAGNETIC_DECLINATION;
    }
    return INVALID_VALUE;
}

float Environment::getTemp() { return imuReady ? temperature : INVALID_VALUE; }

float Environment::readbattery() {
    unsigned int val = analogRead(BATTERY_PIN);
    return float(val) / 64;
}
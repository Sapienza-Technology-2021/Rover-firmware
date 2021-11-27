#ifndef AccelStepper_h
#define AccelStepper_h

#include <Arduino.h>

class AccelStepper {
   public:
    AccelStepper(uint8_t stepPin, uint8_t dirPin);

    void stop();
    bool isRunning();
    void move(long relative);
    void moveTo(long absolute);

    boolean run();
    boolean runSpeed();
    void runToPosition();
    boolean runSpeedToPosition();
    void runToNewPosition(long position);

    float maxSpeed();
    void setMaxSpeed(float speed);
    void setAcceleration(float acceleration);
    void setSpeed(float speed);
    float speed();

    long distanceToGo();
    long targetPosition();
    long currentPosition();
    void setCurrentPosition(long position);

    void setMinPulseWidth(unsigned int minWidth);

   protected:
    typedef enum {
        // Counter-Clockwise
        DIRECTION_CCW = 0,
        // Clockwise
        DIRECTION_CW = 1
    } Direction;

    void computeNewSpeed();

    virtual void step(long step);

    boolean _direction;

   private:
    uint8_t _dirPin;
    uint8_t _stepPin;

    long _currentPos;
    long _targetPos;
    float _speed;
    float _maxSpeed;

    float _acceleration;
    unsigned long _stepInterval;
    unsigned long _lastStepTime;
    unsigned int _minPulseWidth;
    long _n;
    float _c0;
    float _cn;
    float _cmin;
};

#endif

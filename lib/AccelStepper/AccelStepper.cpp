#include "AccelStepper.h"

AccelStepper::AccelStepper(uint8_t stepPin, uint8_t dirPin) {
    _currentPos = 0;
    _targetPos = 0;
    _speed = 0.0;
    _maxSpeed = 1.0;
    _acceleration = 1.0;
    _stepInterval = 0;
    _minPulseWidth = 1;
    _lastStepTime = 0;
    _stepPin = stepPin;
    pinMode(_stepPin, OUTPUT);
    _dirPin = dirPin;
    pinMode(_dirPin, OUTPUT);
    _direction = DIRECTION_CCW;
    _n = 0;
    _cn = 0.0;
    _cmin = 1.0;
    _c0 = 0.676 * sqrt(2.0) * 1000000.0;
}

void AccelStepper::moveTo(long absolute) {
    if (_targetPos != absolute) {
        _targetPos = absolute;
        computeNewSpeed();
    }
}

void AccelStepper::move(long relative) { moveTo(_currentPos + relative); }

boolean AccelStepper::runSpeed() {
    if (!_stepInterval) return false;
    unsigned long time = micros();
    if (time - _lastStepTime >= _stepInterval) {
        if (_direction == DIRECTION_CW) {
            // Clockwise
            _currentPos += 1;
        } else {
            // Anticlockwise
            _currentPos -= 1;
        }
        step(_currentPos);
        _lastStepTime = time;  // Caution: does not account for costs in step()
        return true;
    } else {
        return false;
    }
}

long AccelStepper::distanceToGo() { return _targetPos - _currentPos; }

long AccelStepper::targetPosition() { return _targetPos; }

long AccelStepper::currentPosition() { return _currentPos; }

void AccelStepper::setCurrentPosition(long position) {
    _targetPos = _currentPos = position;
    _n = 0;
    _stepInterval = 0;
    _speed = 0.0;
}

void AccelStepper::computeNewSpeed() {
    long distanceTo = distanceToGo();
    long stepsToStop = (long)((_speed * _speed) / (2.0 * _acceleration));
    if (distanceTo == 0 && stepsToStop <= 1) {
        _stepInterval = 0;
        _speed = 0.0;
        _n = 0;
        return;
    }

    if (distanceTo > 0) {
        if (_n > 0) {
            // Currently accelerating, need to decel now? Or maybe going the wrong way?
            if ((stepsToStop >= distanceTo) || _direction == DIRECTION_CCW) _n = -stepsToStop;  // Start deceleration
        } else if (_n < 0) {
            // Currently decelerating, need to accel again?
            if ((stepsToStop < distanceTo) && _direction == DIRECTION_CW) _n = -_n;  // Start accceleration
        }
    } else if (distanceTo < 0) {
        // We are clockwise from the target
        // Need to go anticlockwise from here, maybe decelerate
        if (_n > 0) {
            // Currently accelerating, need to decel now? Or maybe going the wrong way?
            if ((stepsToStop >= -distanceTo) || _direction == DIRECTION_CW) _n = -stepsToStop;  // Start deceleration
        } else if (_n < 0) {
            // Currently decelerating, need to accel again?
            if ((stepsToStop < -distanceTo) && _direction == DIRECTION_CCW) _n = -_n;  // Start accceleration
        }
    }

    // Need to accelerate or decelerate
    if (_n == 0) {
        // First step from stopped
        _cn = _c0;
        _direction = (distanceTo > 0) ? DIRECTION_CW : DIRECTION_CCW;
    } else {
        // Subsequent step. Works for accel (n is +_ve) and decel (n is -ve).
        _cn = _cn - ((2.0 * _cn) / ((4.0 * _n) + 1));  // Equation 13
        _cn = max(_cn, _cmin);
    }
    _n++;
    _stepInterval = _cn;
    _speed = 1000000.0 / _cn;
    if (_direction == DIRECTION_CCW) _speed = -_speed;
}

boolean AccelStepper::run() {
    if (runSpeed()) computeNewSpeed();
    return _speed != 0.0 || distanceToGo() != 0;
}

void AccelStepper::setMaxSpeed(float speed) {
    if (speed < 0.0) speed = -speed;
    if (_maxSpeed != speed) {
        _maxSpeed = speed;
        _cmin = 1000000.0 / speed;
        if (_n > 0) {
            _n = (long)((_speed * _speed) / (2.0 * _acceleration));
            computeNewSpeed();
        }
    }
}

float AccelStepper::maxSpeed() { return _maxSpeed; }

void AccelStepper::setAcceleration(float acceleration) {
    if (acceleration == 0.0) return;
    if (acceleration < 0.0) acceleration = -acceleration;
    if (_acceleration != acceleration) {
        _n = _n * (_acceleration / acceleration);
        _c0 = 0.676 * sqrt(2.0 / acceleration) * 1000000.0;
        _acceleration = acceleration;
        computeNewSpeed();
    }
}

void AccelStepper::setSpeed(float speed) {
    if (speed == _speed) return;
    speed = constrain(speed, -_maxSpeed, _maxSpeed);
    if (speed == 0.0)
        _stepInterval = 0;
    else {
        _stepInterval = fabs(1000000.0 / speed);
        _direction = (speed > 0.0) ? DIRECTION_CW : DIRECTION_CCW;
    }
    _speed = speed;
}

float AccelStepper::speed() { return _speed; }

// Subclasses can override
void AccelStepper::step(long step) {
    digitalWrite(_dirPin, _direction);
    digitalWrite(_stepPin, HIGH);
    delayMicroseconds(_minPulseWidth);
    digitalWrite(_stepPin, LOW);
}

void AccelStepper::setMinPulseWidth(unsigned int minWidth) { _minPulseWidth = minWidth; }

void AccelStepper::runToPosition() {
    while (run())
        ;
}

boolean AccelStepper::runSpeedToPosition() {
    if (_targetPos == _currentPos) return false;
    if (_targetPos > _currentPos)
        _direction = DIRECTION_CW;
    else
        _direction = DIRECTION_CCW;
    return runSpeed();
}

void AccelStepper::runToNewPosition(long position) {
    moveTo(position);
    runToPosition();
}

void AccelStepper::stop() {
    if (_speed != 0.0) {
        long stepsToStop = (long)((_speed * _speed) / (2.0 * _acceleration)) + 1;  // Equation 16 (+integer rounding)
        if (_speed > 0)
            move(stepsToStop);
        else
            move(-stepsToStop);
    }
}

bool AccelStepper::isRunning() { return !(_speed == 0.0 && _targetPos == _currentPos); }

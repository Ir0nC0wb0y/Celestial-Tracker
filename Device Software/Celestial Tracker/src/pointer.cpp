#include "pointer.h"

Pointer::Pointer() {
    return;
}

bool Pointer::AccumulateMove(float new_az, float new_el) {
    // Set Up
    //_angle_az_new = new_az;
    //_angle_el_new = new_el;
    Serial.print("Accumulation: "); Serial.print(abs(new_az - _angle_az)); Serial.print(" , "); Serial.println(abs(new_el - _angle_el));

    if ( abs(new_az - _angle_az) > _min_az | abs(new_el - _angle_el) > _min_el ) {
        Serial.print("Moving to new position (Az,El): "); Serial.print(new_az); Serial.print(" , "); Serial.println(new_el);
        MoveTo(new_az,new_el);
        return true;
    } else {
        Serial.println("Not large enough move");
        return false;
    }
}

void Pointer::SetAccumulation(float min_az, float min_el) {
    _min_az = abs(min_az);
    _min_el = abs(min_el);
    return;
}

void Pointer::MoveTo(float angle_az, float angle_el) {
    //Serial.println("Moving to new position (Az,El): "); Serial.print(angle_az); Serial.print(" , "); Serial.println(angle_el);
    // Calculate length of move
    float move_az_cw  = 0.0;
    float move_az_ccw = 0.0;
    float move_el_cw  = 0.0;
    float move_el_ccw = 0.0;

    // Calculate CW move
    move_az_cw = fmod(((angle_az + 360.0) - _angle_az), 360.0);
    move_el_cw = fmod(((angle_el + 360.0) - _angle_el), 360.0);
    //Serial.println("Clockwise moves (Az,El): "); Serial.print(move_az_cw); Serial.print(" , "); Serial.println(move_el_cw);

    // Calculate CCW move
    move_az_ccw = -1 * fmod(((_angle_az + 360.0) - angle_az), 360.0);
    move_el_ccw = -1 * fmod(((_angle_el + 360.0) - angle_el), 360.0);
    //Serial.println("Clockwise moves (Az,El): "); Serial.print(move_az_ccw); Serial.print(" , "); Serial.println(move_el_ccw);

    // Choose closest move - Az
    float move_az = 0.0;
    if (abs(move_az_ccw) >= move_az_cw) {
        move_az = move_az_cw;
    } else {
        move_az = move_az_ccw;
    }

    // Choose closest move - El
    float move_el = 0.0;
    if (abs(move_el_ccw) >= move_el_cw) {
        move_el = move_el_cw;
    } else {
        move_el = move_el_ccw;
    }
    //Serial.println("Shortest move (Az,El): "); Serial.print(move_az); Serial.print(" , "); Serial.println(move_el);

    // Make the move
    _CalcSteps(move_az, move_el);
    _CalcSpeed();

    // Set current position
    // This will forever remain between 0 <= angle < 360
    _angle_az = fmod(angle_az, 360.0);
    _angle_el = fmod(angle_el, 360.0);

    stepper_az.move(_steps_az);
    stepper_el.move(_steps_el);
    stepper_az.setMaxSpeed(_speed_az);
    stepper_el.setMaxSpeed(_speed_el);

    return;
}

void Pointer::MoveDirect(float move_az, float move_el) {
    // Determine Steps
    _CalcSteps(move_az, move_el);

    // Determine Speed
    _CalcSpeed();

    // Set updated position
    // This will forever remain between 0 <= angle < 360
    _angle_az = fmod(_angle_az + move_az, 360.0);
    _angle_el = fmod(_angle_el + move_el, 360.0);

    stepper_az.move(_steps_az);
    stepper_el.move(_steps_el);
    stepper_az.setMaxSpeed(_speed_az);
    stepper_el.setMaxSpeed(_speed_el);

    return;
}

void Pointer::SetZeroPosition(float angleAz, float angleEl) {
    _angle_az = fmod(angleAz, 360.0);
    _angle_el = fmod(angleEl, 360.0);

    stepper_az.setCurrentPosition(0);
    stepper_el.setCurrentPosition(0);

    Serial.print("Setting new position (Az,El): ");
    Serial.print(_angle_az);
    Serial.print(" , ");
    Serial.println(_angle_el);

    return;
}

int Pointer::GetStepsAz() const {
    return _steps_az;
}

int Pointer::GetStepsEl() const {
    return _steps_el;
}

int Pointer::GetSpeedAz() const {
    return _speed_az;
}

int Pointer::GetSpeedEl() const {
    return _speed_el;
}

int Pointer::GetCurrentAz() const {
    return _angle_az;
}

int Pointer::GetCurrentEl() const {
    return _angle_el;
}

void Pointer::_CalcSteps(float move_az, float move_el) {
    // Determine Steps
    _steps_az = move_az * STEPS_PER_DEGREE;
    _steps_el = move_el * STEPS_PER_DEGREE - _steps_az;

    //Serial.println("Steps Calc'd (Az,El): "); Serial.print(_steps_az); Serial.print(" , "); Serial.println(_steps_el);

    return;
}

void Pointer::_CalcSpeed() {
    // Determine Speed
    _speed_az = BASE_SPEED;
    _speed_el = BASE_SPEED * abs((float)_steps_el / (float)_steps_az);

    if ( _speed_el > MAX_SPEED) {
        _speed_az = BASE_SPEED * (BASE_SPEED/(float)_speed_el);
        _speed_el = BASE_SPEED;
    }

    //Serial.println("Speeds Calc'd (Az,El): "); Serial.print(_speed_az); Serial.print(" , "); Serial.println(_speed_el);

    return;
}
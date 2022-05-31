#pragma once
#include <Arduino.h>
#include <math.h>
#include "pin_def.h"

//#define GEAR_RATIO        3.84615384615
//#define FULL_ROTATION     4076
#define STEPS_PER_DEGREE  43.547
#define BASE_SPEED 800
#define MAX_SPEED  1000

class Pointer {
public:
    Pointer();
    bool AccumulateMove(float move_az, float move_el);
    void SetAccumulation(float min_az, float min_el);
    void MoveTo(float angle_az, float angle_el);
    void MoveDirect(float move_az, float move_el);
    void SetZeroPosition(float angle_az, float angle_el);
    int GetStepsAz() const;
    int GetStepsEl() const;
    int GetSpeedAz() const;
    int GetSpeedEl() const;
    int GetCurrentAz() const;
    int GetCurrentEl() const;

private:
    void _CalcSteps(float move_az, float move_el);
    void _CalcSpeed();

    float _angle_az = 0; // Current Az position
    float _angle_el = 0; // Current El position
    float _angle_az_last = 0;
    float _angle_el_last = 0;
    float _angle_az_new = 0;
    float _angle_el_new = 0;
    int _steps_az = 0;
    int _steps_el = 0;
    float _move_az = 0;
    float _move_el = 0;
    int _speed_az = 0;
    int _speed_el = 0;
    float _min_az = 0.0;
    float _min_el = 0.0;
};
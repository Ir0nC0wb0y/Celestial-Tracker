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
    bool AccumulateMove(float move_Az, float move_El);
    void SetAccumulation(float min_Az, float min_El);
    void MoveTo(float angle_Az, float angle_El);
    void MoveDirect(float move_Az, float move_El);
    void SetZeroPosition(float angleAz, float angleEl);
    int GetStepsAz() const;
    int GetStepsEl() const;
    int GetSpeedAz() const;
    int GetSpeedEl() const;
    int GetCurrentAz() const;
    int GetCurrentEl() const;

private:
    void _CalcSteps(float move_Az, float move_El);
    void _CalcSpeed();

    float _angle_Az = 0; // Current Az position
    float _angle_El = 0; // Current El position
    float _angle_Az_last = 0;
    float _angle_El_last = 0;
    float _angle_Az_new = 0;
    float _angle_El_new = 0;
    int _steps_Az = 0;
    int _steps_El = 0;
    float _move_Az = 0;
    float _move_El = 0;
    int _speed_Az = 0;
    int _speed_El = 0;
    float _min_Az = 0.0;
    float _min_El = 0.0;
};
#pragma once
#include <Arduino.h>

//#define GEAR_RATIO        3.84615384615
//#define FULL_ROTATION     4076
#define STEPS_PER_DEGREE  43.547
#define BASE_SPEED 800
#define MAX_SPEED  1000

// This class should be used to track its location
class Position {
  public:
    Position();
    void MoveNearest(float new_Az, float new_El);
    void MoveDirect(float move_Az, float move_El);
    int getStepsAz();
    int getStepsEl();
    int getSpeedAz();
    int getSpeedEl();
    void setPosition(float angleAz, float angleEl);

  private:
    float _angle_Az = 0;
    float _angle_El = 0;
    float _angle_Az_last = 0;
    float _angle_El_last = 0;
    int _steps_Az = 0;
    int _steps_El = 0;
    float _move_Az = 0;
    float _move_El = 0;
    int _speed_Az = 0;
    int _speed_El = 0;
};

// New class should be used for the movement
class Move {
  public:

  private:

};
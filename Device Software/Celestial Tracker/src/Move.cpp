#include "Move.h"

Position::Position(void) {
}

void Position::MoveNearest(float new_Az, float new_El) {
  // Set Up
  _angle_Az_last = _angle_Az;
  _angle_El_last = _angle_El;
  _angle_Az = new_Az;
  _angle_El = new_El;

  // Determine closest approach to new angle
  
  // Convert angle move to steps

}

void Position::MoveDirect(float move_Az, float move_El) {
  // Determine Steps
  _steps_Az = move_Az * STEPS_PER_DEGREE;
  _steps_El = move_El * STEPS_PER_DEGREE - _steps_Az;

  // Determine Speed
  _speed_Az = BASE_SPEED;
  _speed_El = BASE_SPEED * abs((float)_steps_El / (float)_steps_Az);
  if ( _speed_El > MAX_SPEED) {
    _speed_Az = BASE_SPEED * (BASE_SPEED/(float)_speed_El);
    _speed_El = BASE_SPEED;
  }

}

void Position::setPosition(float angleAz, float angleEl) {
  _angle_Az = angleAz;
  _angle_El = angleEl;
  Stepper_Az.setCurrentPosition(0);
  Stepper_El.setCurrentPosition(0);
  Serial.print("Setting new position (Az,El): ");
  Serial.print(_angle_Az);
  Serial.print(" , ");
  Serial.println(_angle_El);
}

int Position::getStepsAz() {
  return _steps_Az;
}

int Position::getStepsEl() {
  return _steps_El;
}

int Position::getSpeedAz() {
  return _speed_Az;
}

int Position::getSpeedEl() {
  return _speed_El;
}
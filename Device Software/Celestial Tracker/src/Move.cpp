#include "Move.h"

Position::Position(void) {
}

bool Position::AccumulateMove(float new_Az, float new_El) {
  // Set Up
  _angle_Az_new = new_Az;
  _angle_El_new = new_El;
  
  if ( abs(_angle_Az_new - _angle_Az) > _min_Az | abs(_angle_El_new - _angle_El) > _min_El ) {
    MoveTo(new_Az,new_El);
    return true;
  } else {
    Serial.println("Not large enough move");
    return false;
  }
}

void SetAccumulation(float min_Az, float min_El) {
  _min_Az = abs(min_Az);
  _min_El = abs(min_El);
}

void Position::MoveTo(float angle_Az, float angle_El) {
  // Calculate length of move
  float move_Az_cw = 0.0;
  float move_Az_ccw = 0.0;
  float move_El_cw = 0.0;
  float move_El_ccw = 0.0;
  
  // Calculate CW move
  move_Az_cw = ((angle_Az + 360) - _angle_Az) % 360;
  move_El_cw = ((angle_El + 360) - _angle_El) % 360;
  
  // Calculate CCW move
  move_Az_ccw = -1 * (((_angle_Az + 360) - angle_Az) % 360);
  move_El_ccw = -1 * (((_angle_El + 360) - angle_El) % 360);
  
  // Choose closest move - Az
  float move_Az = 0.0;
  if (abs(move_Az_ccw) >= move_Az_cw) {
    move_Az = move_Az_cw;
  } else {
    move_Az = move_Az_ccw;
  }
  
  // Choose closest move - El
  float move_El = 0.0;
  if (abs(move_El_ccw) >= move_El_cw) {
    move_El = move_El_cw;
  } else {
    move_El = move_El_ccw;
  }
  
  // Make the move
  _Calc_steps(float move_Az, float move_El);
  _Calc_speed();
  
  // Set current position
  // This will forever remain between 0 <= angle < 360
  _angle_Az = angle_Az % 360;
  _angle_El = angle_El % 360;
}

void Position::MoveDirect(float move_Az, float move_El) {
  // Determine Steps
  _Calc_steps(float move_Az, float move_El);
  
  // Determine Speed
  _Calc_speed();
  
  // Set updated position
  // This will forever remain between 0 <= angle < 360
  _angle_Az = (angle_Az + move_Az) % 360;
  _angle_El = (angle_El + move_El) % 360;
}

void Position::setZeroPosition(float angleAz, float angleEl) {
  _angle_Az = angleAz % 360;
  _angle_El = angleEl % 360;
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

int Position::getCurrentAz() {
  return _angle_Az;
}

int Position::getCurrentEl() {
  return _angle_El;
}

void Position::_Calc_steps(float move_Az, float move_El) {
  // Determine Steps
  _steps_Az = move_Az * STEPS_PER_DEGREE;
  _steps_El = move_El * STEPS_PER_DEGREE - _steps_Az;
}

void Position::_Calc_speed() {
  // Determine Speed
  _speed_Az = BASE_SPEED;
  _speed_El = BASE_SPEED * abs((float)_steps_El / (float)_steps_Az);
  if ( _speed_El > MAX_SPEED) {
    _speed_Az = BASE_SPEED * (BASE_SPEED/(float)_speed_El);
    _speed_El = BASE_SPEED;
  }
}

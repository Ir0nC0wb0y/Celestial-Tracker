#include <Arduino.h>
#include "Homing.h"

///////////////////////////// Interrupt Handling /////////////////////////////
bool _intAz = false;
void _interruptAz() {
  _intAz = true;
}

void Homing::_setupInterruptAz(bool _setInt) {
  if (_setInt) {
    attachInterrupt(PIN_H_AZ,_interruptAz,CHANGE); // This needs to be adjusted to RISING/FALLING as appropriate
    _intAz = false;
  } else {
    detachInterrupt(PIN_H_AZ);
    _intAz = false;
  }
}

bool _intEl = false;
void _interruptEl() {
  _intEl = true;
}

void Homing::_setupInterruptEl(bool _setInt) {
  if (_setInt) {
    attachInterrupt(PIN_H_EL,_interruptEl,CHANGE); // This needs to be adjusted to RISING/FALLING as appropriate
    _intAz = false;
  } else {
    detachInterrupt(PIN_H_EL);
    _intAz = false;
  }
}

bool Homing::isHomed() {
  if ( _homingStateAz == 0 && _homingStateEl == 0) {
    return true;
  } else {
    return false;
  }
}

void Homing::goHome() {
  //attach interrupts
  _setupInterruptAz(true);
  _setupInterruptEl(true);
  
  //set motor speed
  Stepper_Az.setSpeed(HOMING_SPEED_FAST);
  Stepper_El.setSpeed(HOMING_SPEED_FAST);

  //set state machine
  _homingStateAz = 1;
  _homingStateEl = 1;
}

void Homing::runHoming() {
  // State machine handling - Az
  switch (_homingStateAz) {
    case 1:
      if (!_intAz) {
        Stepper_Az.run();
        break;
      } else {
        _homingStateAz = 2;
        Stepper_Az.moveTo(HOMING_RETREAT);
      }

    case 2:
      if (Stepper_Az.distanceToGo() == 0) {
        Stepper_Az.run();
        break;
      } else {
        _homingStateAz = 3;
        Stepper_Az.setSpeed(HOMING_SPEED_SLOW);
      }

    case 3:
      if (!_intAz) {
        Stepper_Az.run();
        break;
      } else {
        _homingStateAz = 0;
      }
      break;

    default: // Not sure how to handle edge cases here
      break;
  }

  // State machine handling - El
  switch (_homingStateEl) {
    case 1:
      if (!_intEl) {
        Stepper_El.run();
        break;
      } else {
        _homingStateEl = 2;
        Stepper_El.moveTo(HOMING_RETREAT);
      }

    case 2:
      if (Stepper_El.distanceToGo() == 0) {
        Stepper_El.run();
        break;
      } else {
        _homingStateEl = 3;
        Stepper_El.setSpeed(HOMING_SPEED_SLOW);
      }

    case 3:
      if (!_intEl) {
        Stepper_El.run();
        break;
      } else {
        _homingStateEl = 0;
      }
      break;

    default: // Not sure how to handle edge cases here
      break;
  }
}




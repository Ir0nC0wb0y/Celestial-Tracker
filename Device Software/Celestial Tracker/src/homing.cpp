#include <Arduino.h>
#include "Homing.h"

Homing::Homing(void) {
}

///////////////////////////// Interrupt Handling /////////////////////////////
bool _intAz = false;
void _interruptAz() {
  _intAz = true;
}

void Homing::_setupInterruptAz(bool _setInt) {
  if (_setInt) {
    Serial.println("Setting up Az Interrupt");
    attachInterrupt(PIN_H_AZ,_interruptAz,FALLING); // This needs to be adjusted to RISING/FALLING as appropriate
    _intAz = false;
  } else {
    Serial.println("Destroying Az Interrupt");
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
    Serial.println("Setting up Az Interrupt");
    attachInterrupt(PIN_H_EL,_interruptEl,FALLING); // This needs to be adjusted to RISING/FALLING as appropriate
    _intAz = false;
  } else {
    Serial.println("Destroying El Interrupt");
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
  Serial.println("Setting up Homing");
  //attach interrupts
  _setupInterruptAz(true);
  _setupInterruptEl(true);
  
  //set motor speed
  Stepper_Az.setSpeed(HOMING_SPEED_FAST);
  Stepper_El.setSpeed(HOMING_SPEED_FAST);

  //set state machine
  _homingStateAz = 1;
  _homingStateEl = 1;
  Serial.println("Moving Az/El to State 1, Fast turn");
}

void Homing::runHoming() {
  // State machine handling - Az
  if (_nextReport <= millis()) {
   Serial.print("Homing State (Az,El), Endstop State (Az,El): "); Serial.print(_homingStateAz); Serial.print(" , "); Serial.print(_homingStateEl); Serial.print(" , ");
   Serial.print(digitalRead(PIN_H_AZ)); Serial.print(" , "); Serial.print(digitalRead(PIN_H_EL));
   Serial.println();
   _nextReport = millis() + 250;
  }

  switch (_homingStateAz) {
    case 1:
      if (!_intAz) {
        Stepper_Az.runSpeed();
      } else {
        _homingStateAz = 2;
        Stepper_Az.setCurrentPosition(0);
        Stepper_Az.moveTo(HOMING_RETREAT);
        Serial.println("Moving Az to State 2, Retreat");
      }
      break;

    case 2:
      if (Stepper_Az.distanceToGo() == 0) {
        Stepper_Az.run();
      } else {
        _homingStateAz = 3;
        _intAz = false;
        Stepper_Az.setSpeed(HOMING_SPEED_SLOW);
        Serial.println("Moving Az to State 3, Slow Approach");
      }
      break;
      

    case 3:
      if (!_intAz) {
        Stepper_Az.runSpeed();
      } else {
        _homingStateAz = 0;
        _setupInterruptAz(false);
        Serial.println("Found Home Position: AZ!!");
      }
      break;

    default: // Not sure how to handle edge cases here
      break;
  }

  // State machine handling - El
  switch (_homingStateEl) {
    case 1:
      if (!_intEl) {
        Stepper_El.runSpeed();
        break;
      } else {
        _homingStateEl = 2;
        Stepper_El.moveTo(HOMING_RETREAT);
        Serial.println("Moving El to State 2, Retreat");
      }

    case 2:
      if (Stepper_El.distanceToGo() == 0) {
        Stepper_El.run();
        break;
      } else {
        _homingStateEl = 3;
        _intEl = false;
        Stepper_El.setSpeed(HOMING_SPEED_SLOW);
        Serial.println("Moving El to State 3, Slow Approach");
      }

    case 3:
      if (!_intEl) {
        Stepper_El.runSpeed();
        break;
      } else {
        _homingStateEl = 0;
        _setupInterruptEl(false);
        Serial.println("Found Home Position: El!!");
      }
      break;

    default: // Not sure how to handle edge cases here
      break;
  }
}




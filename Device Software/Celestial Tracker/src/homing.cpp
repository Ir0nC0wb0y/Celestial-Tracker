#include <Arduino.h>
#include "Homing.h"

Homing::Homing() {
    return;
}

///////////////////////////// Interrupt Handling /////////////////////////////
static bool int_az = false;
static bool int_el = false;

static void InterruptAz() {
    int_az = true;
    return;
}

static void InterruptEl() {
    int_el = true;
    return;
}

void Homing::_SetupInterruptAz(bool setInt) {
    if (setInt) {
        Serial.println("Setting up Az Interrupt");
        attachInterrupt(PIN_H_AZ, InterruptAz, FALLING); // This needs to be adjusted to RISING/FALLING as appropriate
        int_az = false;
    } else {
        Serial.println("Destroying Az Interrupt");
        detachInterrupt(PIN_H_AZ);
        int_az = false;
    }

    return;
}

void Homing::_SetupInterruptEl(bool set_int) {
    if (set_int) {
        Serial.println("Setting up Az Interrupt");
        attachInterrupt(PIN_H_EL, InterruptEl, FALLING); // This needs to be adjusted to RISING/FALLING as appropriate
        int_az = false;
    } else {
        Serial.println("Destroying El Interrupt");
        detachInterrupt(PIN_H_EL);
        int_az = false;
    }

    return;
}

bool Homing::IsHomed() {
    if ( _homing_state_az == 0 && _homing_state_el == 0) {
        return true;
    } else {
        return false;
    }
}

void Homing::GoHome() {
    Serial.println("Setting up Homing");

    //attach interrupts
    _SetupInterruptAz(true);
    _SetupInterruptEl(true);

    //set motor speed
    stepper_az.setSpeed(HOMING_SPEED_FAST);
    stepper_el.setSpeed(-1 * HOMING_SPEED_FAST);

    //set state machine
    _homing_state_az = 1;
    _homing_state_el = 1;
    Serial.println("Moving Az/El to State 1, Fast turn");

    return;
}

void Homing::RunHoming() {
    // State machine handling - Az
    if (_next_report <= millis()) {
        Serial.print("Homing State (Az,El), Endstop State (Az,El): "); Serial.print(_homing_state_az); Serial.print(" , "); Serial.print(_homing_state_el); Serial.print(" , ");
        Serial.print(digitalRead(PIN_H_AZ)); Serial.print(" , "); Serial.print(digitalRead(PIN_H_EL));
        Serial.println();
        _next_report = millis() + 250;
    }

    switch (_homing_state_az) {
    case 1:
        if (!int_az) {
            stepper_az.runSpeed();
        } else {
            _homing_state_az = 2;
            stepper_az.setCurrentPosition(0);
            stepper_az.moveTo(HOMING_RETREAT);
            Serial.println("Moving Az to State 2, Retreat");
        }
        break;

    case 2:
        if (stepper_az.distanceToGo() == 0) {
            stepper_az.run();
        } else {
            _homing_state_az = 3;
            int_az = false;
            stepper_az.setSpeed(HOMING_SPEED_SLOW);
            Serial.println("Moving Az to State 3, Slow Approach");
        }
        break;

    case 3:
        if (!int_az) {
            stepper_az.runSpeed();
        } else {
            _homing_state_az = 0;
            _SetupInterruptAz(false);
            Serial.println("Found Home Position: AZ!!");
        }
        break;

    default: // Not sure how to handle edge cases heres
        break;
    }

    // State machine handling - El
    switch (_homing_state_el) {
    case 1:
        if (!int_el) {
            stepper_el.runSpeed();
        } else {
        _homing_state_el = 2;
            stepper_el.moveTo(HOMING_RETREAT);
            Serial.println("Moving El to State 2, Retreat");
        }
        break;

    case 2:
        if (stepper_el.distanceToGo() == 0) {
            stepper_el.run();
        } else {
            _homing_state_el = 3;
            int_el = false;
            stepper_el.setSpeed(HOMING_SPEED_SLOW);
            Serial.println("Moving El to State 3, Slow Approach");
        }
        break;

    case 3:
        if (!int_el) {
            stepper_el.runSpeed();
        } else {
            _homing_state_el = 0;
            _SetupInterruptEl(false);
            Serial.println("Found Home Position: El!!");
        }
        break;

    default: // Not sure how to handle edge cases here
        break;
    }

    return;
}
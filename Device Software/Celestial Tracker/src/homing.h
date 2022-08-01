#pragma once
#include "pin_def.h"

#define HOMING_SPEED_FAST   800
#define HOMING_SPEED_SLOW    50
#define HOMING_RETREAT      435  // Roughly 10 degrees. This is untested
#define HOMING_DIR_AZ         1
#define HOMING_DIR_EL         1


class Homing {
public:
    Homing();
    bool IsHomed(); // this may not be necessary if this is only done during setup
    void GoHome();    // initiates the homing sequence
    void RunHoming(); // maintains state machines

private:
    bool _limit_touched = false;
    int _homing_state_az = -1; //used for state machine
    int _homing_state_el = -1; //used for state machine
    // state -1 = homing class initialized, not run
    // state  1 = turning constant speed
    // state  2 = limit switch engaged, move back ## steps
    // state  3 = turning constant speed, *slower*
    // state  0 = homed
    void _SetupInterruptAz(bool set_int);
    void _SetupInterruptEl(bool set_int);
    unsigned long _next_report = millis();
};
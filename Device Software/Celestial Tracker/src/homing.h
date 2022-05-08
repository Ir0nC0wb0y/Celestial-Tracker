#pragma once
#include "PinDef.h"

#define HOMING_SPEED_FAST   800
#define HOMING_SPEED_SLOW    50
#define HOMING_RETREAT      435  // Roughly 10 degrees. This is untested

// Possible functions
class Homing {
  public:
    Homing();
    bool isHomed(); // this may not be necessary if this is only done during setup
    void goHome();    // initiates the homing sequence
    void runHoming(); // maintains state machines

  private:
    bool _limitTouched = false;
    int _homingStateAz = -1; //used for state machine
    int _homingStateEl = -1; //used for state machine
    // state -1 = homing class initialized, not run
    // state  1 = turning constant speed
    // state  2 = limit switch engaged, move back ## steps
    // state  3 = turning constant speed, *slower*
    // state  0 = homed
    void _setupInterruptAz(bool _setInt);
    void _setupInterruptEl(bool _setInt);
    unsigned long _nextReport = millis();
    
};

#ifndef flipperLED_h
#define flipperLED_h

#include "configs.h"
#include "settings.h"

#include <Arduino.h>

#define B_PIN 4
#define G_PIN 5
#define R_PIN 6

#ifdef HAS_SD
extern Settings settings_obj;
#endif
class flipperLED {

  public:
    void RunSetup();
    void attackLED();
    void sniffLED();
    void offLED();
};

#endif

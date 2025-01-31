#ifndef LedInterface_h
#define LedInterface_h

#include "configs.h"
#include "settings.h"
#include <Arduino.h>
#ifndef MARAUDER_C1B3RT4CKS
    #include <Adafruit_NeoPixel.h>
#endif

#define Pixels 1

#define MODE_OFF 0
#define MODE_RAINBOW 1
#define MODE_ATTACK 2
#define MODE_SNIFF 3
  extern Settings settings_obj;
#ifndef MARAUDER_C1B3RT4CKS

extern Adafruit_NeoPixel strip;
#endif
class LedInterface {

  private:
    uint32_t initTime = 0;

    int current_fade_itter = 1;
    int wheel_pos = 255;
    int wheel_speed = 1; // lower = slower

    uint32_t Wheel(byte WheelPos);

    uint8_t current_mode = MODE_OFF;

    void rainbow();
    void ledOff();
    void attackLed();
    void sniffLed();
  
  public:
    LedInterface();

    void RunSetup();
    void main(uint32_t currentTime);

    void setMode(uint8_t);
    uint8_t getMode();
    
  
};

#endif

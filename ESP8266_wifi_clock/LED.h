// LED used as status indicator

#ifndef LED_h
#define LED_h

#include "Arduino.h"

#define LED_ON  0
#define LED_OFF 1

class LED{
  public:
    uint8_t pin = 0;
    LED(uint8_t pin_number);
    void ON();
    void OFF();
    void init();
    void flash(uint32_t delay_time);
};

#endif

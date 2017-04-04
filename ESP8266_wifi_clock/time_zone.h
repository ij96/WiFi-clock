#ifndef TIME_ZONE_H
#define TIME_ZONE_H

#include "Arduino.h"

class UTC_TZ{   // UTC time zone
  public:
    uint8_t hour = 0;
    uint8_t minute = 0;
    bool sign = true;
    UTC_TZ(uint8_t h, uint8_t m, bool s);
};

#endif

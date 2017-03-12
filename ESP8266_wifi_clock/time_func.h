#ifndef Time_func_h
#define Time_func_h

#include "Arduino.h"
#include <Time.h>
#include <TimeLib.h>

#define EPOCH_OFFSET 2208988800L  // NIST uses a different epoch: midnight UTC6 1900-01-01

void Serial_print_time();

#endif

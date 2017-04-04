#ifndef TIME_FUNC_H
#define TIME_FUNC_H

#include "Arduino.h"
#include <Time.h>
#include <TimeLib.h>
#include "time_zone.h"

#define NIST_EPOCH_OFFSET 2208988800L  // NIST uses a different epoch: midnight UTC 1900-01-01

// system time is UTC+00:00, the displayed local time is adjusted according to time zone

time_t local_time(UTC_TZ tz);
String daytime_UTC();
String daytime_local(UTC_TZ tz);

#endif

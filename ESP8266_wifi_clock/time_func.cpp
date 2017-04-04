#include "time_func.h"

time_t local_time(UTC_TZ tz){
  uint32_t offset = tz.hour*3600L + tz.minute*60L;
  time_t t = now();
  if(tz.sign) t += offset;
  else t -= offset;
  return t;
}

String daytime_UTC(){  // UTC+00:00 time
  char dt[20] = "";
  time_t t = now();
  sprintf(dt,"%d-%02d-%02d %02d:%02d:%02d",year(t),month(t),day(t),hour(t),minute(t),second(t));
  return String(dt);
}

String daytime_local(UTC_TZ tz){
  char dt[26] = "";
  time_t t = local_time(tz);
  sprintf(dt,"%d-%02d-%02d %02d:%02d:%02d%c%02d:%02d",year(t),month(t),day(t),hour(t),minute(t),second(t),tz.sign?'+':'-',tz.hour,tz.minute);
  return String(dt);
}

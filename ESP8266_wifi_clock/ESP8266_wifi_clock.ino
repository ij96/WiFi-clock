// Arduino 1.8.1
// this is to be uploaded to ESP8266
/* Building options:
    Board: "Generic ESP8266 Module"
    Reset method: "nodemcu"
    Debug port: "Disabled"
    Upload speed: "115200"
*/

#include "LED.h"
#include "time_zone.h"
#include "time_func.h"
#include "wifi_func.h"

UTC_TZ local_tz(1,0,1);

LED led(2);                                 // ESP8266 HUZZAH on-board LED at pin 2

char* ssid = "ssid";                        // WiFi SSID
char* password = "password";                // WiFi password
char* host = "time.nist.gov";               // NIST time server
uint16_t http_port = 37;                    // port 37: RFC868 (returns 32-bit unsigned integer)

void setup(){
  Serial.begin(115200);
  led.init();
  delay(100);
}

void loop(){
  setTime(request_time(ssid, password, host, http_port, led));

  Serial.print("UTC time:\t");
  Serial.println(daytime_UTC());

  Serial.print("Local time:\t");
  Serial.println(daytime_local(local_tz));
  
  led.OFF();
  delay(5000);  // avoid refusal from server due to very frequent requests
}

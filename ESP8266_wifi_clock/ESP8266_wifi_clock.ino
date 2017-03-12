// Arduino 1.8.1
// this is to be uploaded to ESP8266
/* Building options:
    Board: "Generic ESP8266 Module"
    Reset method: "nodemcu"
    Debug port: "Disabled"
    Upload speed: "115200"
*/

#include "wifi_func.h"

LED led(2);                                 // ESP8266 HUZZAH on-board LED at pin 2

char* ssid = "ssid";                        // WiFi SSID
char* password = "password";                // WiFi password
char* host = "time.nist.gov";               // NIST time server
uint16_t http_port = 37;                    // port 37: RFC868 (returns 32-bit unsigned integer)

void setup() {
  Serial.begin(115200);
  led.init();
  delay(100);
}

void loop(){
  uint32_t NIST_time = request_time(ssid, password, host, http_port, led);
  setTime(NIST_time);
  
  Serial.print("Converted:\t");
  Serial_print_time();
  Serial.println();
  
  delay(5000);  // avoid refusal from server due to very frequent requests
}

// Arduino 1.8.1
// this is to be uploaded to ESP8266
/* Building options:
    Board: "Generic ESP8266 Module"
    Reset method: "nodemcu"
    Debug port: "Disabled"
    Upload speed: "115200"
*/
// working

#include <Time.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include "LED.h"

#define LED_PIN 2                  // ESP8266 HUZZAH on-board LED

LED led(LED_PIN);

const char* ssid = "ij-phone";              // WiFi SSID
const char* password = "biubiubiu";         // WiFi password
const char* host = "time.nist.gov";         // NIST time server
const uint16_t httpPort = 37;               // port 37: RFC868 (returns 32-bit unsigned integer)
const uint32_t epoch_offset = 2208988800L;  // NIST uses a different epoch: midnight UTC6 1900-01-01

uint32_t NIST_time;

uint32_t convert_4_bytes_to_uint32_t(byte (&buf)[4]){
  uint32_t t = 0;
  for(uint8_t i=0; i<4; i++){
    t += buf[i] << 8*(3-i);
  }
  return t;
}

void Serial_print_time() {
  Serial.print(year());
  Serial.print("-");
  Serial.print(month());
  Serial.print("-");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");
  Serial.print(second());
  Serial.println();
}

void init_hardware() {
  Serial.begin(115200);
  led.init();
  delay(100);
}

void setup() {
  init_hardware();
  
  // connect to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  // set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  delay(100);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    led.flash(250);
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  led.Switch(LED_ON);
}

void loop() {  
  Serial.print("connecting to ");
  Serial.println(host);
  
  // use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // send the request to server
  client.print("HEAD / HTTP/1.1\r\nAccept: */*\r\nUser-Agent: Mozilla/4.0 (compatible; ESP8266 NodeMcu Lua;)\r\n\r\n");

  delay(100);
  
  // read and stores NIST time (as 32-bit unsigned integer)
  byte NIST_time_buffer[4];
  while(client.available()) client.readBytes(NIST_time_buffer, 4);
  NIST_time = convert_4_bytes_to_uint32_t(NIST_time_buffer);

  Serial.println(NIST_time, HEX);
  NIST_time -= epoch_offset;  // remove epoch offset, so time starts on 1970-01-01

  Serial.println();
  Serial.println("closing connection");

  setTime(NIST_time);
  Serial_print_time();
  
  delay(5000);  // avoid refusal from server due to very frequent requests
}

#ifndef WiFi_func_h
#define WiFi_func_h

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include "LED.h"
#include "time_func.h"

void connect_to_wifi(char* ssid, char* password, LED status_led);
bool connect_to_host(WiFiClient& client, char* host, uint16_t http_port, uint8_t retry_limit);
void send_time_request(WiFiClient& client);
uint32_t request_time(char* ssid, char* password, char* host, uint16_t http_port, LED status_led);

#endif

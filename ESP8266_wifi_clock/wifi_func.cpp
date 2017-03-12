#include "wifi_func.h"

void connect_to_wifi(char* ssid, char* password, LED status_led){
  Serial.println();                                       // connect to a WiFi network
  Serial.print("Connecting to WiFi network, SSID: ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);                                    // set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  delay(100);
  
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    status_led.flash(250);
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  status_led.ON();
}

bool connect_to_host(WiFiClient& client, char* host, uint16_t http_port, uint8_t retry_limit){
  Serial.println();                                       // connect to a host
  Serial.print("Connecting to host: ");
  Serial.println(host);

  uint8_t retries = 0;                                    // retry if connection failed
  while(retries < retry_limit){
    if(!client.connect(host, http_port)){
      Serial.println("Connection failed, retrying...");
      Serial.print("Number of retries: ");
      Serial.println(retries);
      delay(5000);
      retries++;
    }
    else{
      Serial.println("Connection successful");
      return true;                                        // connection successful
    }
  }
  Serial.print("Connection failed after ");
  Serial.print(retries);
  Serial.println(" retries.");
  return false;                                           // connection still failed after retries
}

void send_time_request(WiFiClient& client){               // send the request to server
  client.print("HEAD / HTTP/1.1\r\nAccept: */*\r\nUser-Agent: Mozilla/4.0 (compatible; ESP8266 NodeMcu Lua;)\r\n\r\n");
}

uint32_t request_time(char* ssid, char* password, char* host, uint16_t http_port, LED status_led){
  connect_to_wifi(ssid, password, status_led);            // connect to WLAN
  
  WiFiClient client;                                      // use WiFiClient class to create TCP connections

  if(connect_to_host(client, host, http_port, 5)){        // if connected to host
    send_time_request(client);                            // send request to server
    delay(500);                                           // wait for response
    
    byte t_buf[4];                                        // read response (4 bytes)
    while(client.available()) client.readBytes(t_buf, 4); // save response
    
    uint32_t t = 0;                                       // convert 4-byte buffer into uint32_t
    for(uint8_t i=0; i<4; i++) t += t_buf[i] << 8*(3-i);
  
    Serial.print("Raw HEX:\t");
    Serial.println(t, HEX);
    
    t -= EPOCH_OFFSET;                                    // remove epoch offset, so time starts on 1970-01-01
    return t;
  }
  return -1;
}

// Arduino 1.8.1
// this is to be uploaded to ESP8266
// Reset method: "nodemcu"
// Debug port: "Disabled"

#include <ESP8266WiFi.h>

String ssid_buff;
String password_buff;
String host_buff;
uint16_t httpPort;
bool connected_to_WiFi = false;

void setup() {
  Serial.begin(115200);
  delay(100);

  while(!Serial.available());  // wait for Arduino
  bool wifi_info_received = false;
  while(Serial.available() and !wifi_info_received){
    String buff = Serial.readStringUntil('\n');
    if(buff == "WiFi info"){
      ssid_buff = Serial.readStringUntil('\n');
      password_buff = Serial.readStringUntil('\n');
      httpPort = Serial.readStringUntil('\n').toInt();
      host_buff = Serial.readStringUntil('\n');
      wifi_info_received = true;
    }
    delay(100); // wait for buffer to fill up
  }
}

const char* ssid = ssid_buff.c_str();
const char* password = password_buff.c_str();
const char* host = host_buff.c_str();

void loop() {
  Serial.println("ESP8266 echoing received info");
  Serial.print("ssid\t\t"); Serial.println(ssid);
  Serial.print("password\t"); Serial.println(password);
  Serial.print("host\t\t"); Serial.println(host);
  Serial.print("httpPort\t"); Serial.println(httpPort);
  
  if(!connected_to_WiFi){
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
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    connected_to_WiFi = true;
  }
  
  // use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }

  // send the request to server
  client.print("HEAD / HTTP/1.1\r\nAccept: */*\r\nUser-Agent: Mozilla/4.0 (compatible; ESP8266 NodeMcu Lua;)\r\n\r\n");
  delay(100);
  
  // read and stores NIST time (as 32-bit unsigned integer)
  byte NIST_time_buffer[4];
  while(client.available()) {
    client.readBytes(NIST_time_buffer, 4);
  }
  Serial.print("NIST time\n");
  for(uint8_t i=0; i<4; i++) {
    Serial.write(NIST_time_buffer[i]); // pass though Serial
  }
  
  delay(5000);  // avoid refusal from server due to very frequent requests
}

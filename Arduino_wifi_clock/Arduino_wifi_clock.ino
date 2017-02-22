// Arduino 1.8.1
// this is to be uploaded to Arduino UNO

#include <Time.h>
#include <TimeLib.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 2, 3, 4, 5);

const char* HEADER_NIST_TIME_RECEIVED = "NIST time";

const char* ssid        = "ssid";              // WiFi SSID
const char* password    = "password";          // WiFi password
const char* host        = "time.nist.gov";     // NIST time server
const uint16_t httpPort  = 37;                 // port 37: RFC868 (returns 32-bit unsigned integer)

const uint32_t epoch_offset = 2208988800L;     // NIST uses a different epoch: midnight UTC6 1900-01-01

uint32_t NIST_time;

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

void LCD_display_time() {
  lcd.setCursor(0,0);
  lcd.print(year());
  lcd.print("-");
  if(month() < 10) lcd.print(0);
  lcd.print(month());
  lcd.print("-");
  if(day() < 10) lcd.print(0);
  lcd.print(day());
  
  lcd.setCursor(0,1);
  if(hour() < 10) lcd.print(0);
  lcd.print(hour());
  lcd.print(":");
  if(minute() < 10) lcd.print(0);
  lcd.print(minute());
  lcd.print(":");
  if(second() < 10) lcd.print(0);
  lcd.print(second());
}

void send_wifi_info(){
  Serial.print("WiFi info\n");
  Serial.print(ssid);
  Serial.print('\n');
  Serial.print(password);
  Serial.print('\n');
  Serial.print(httpPort);
  Serial.write('\n');
  Serial.print(host);
  Serial.print('\n');
}

void setup() {
  Serial.begin(115200);
  Serial.println("Arduino starting");
  lcd.begin(16,2);
  send_wifi_info();
}

void loop() {  
  while(!Serial.available());   // wait for response from ESP8266
  while(Serial.available()) {
    String tmp = Serial.readStringUntil('\n');
    Serial.println(tmp);        // print Serial data from ESP8266 to monitor
    if(tmp == HEADER_NIST_TIME_RECEIVED) {
      byte NIST_time_buffer[4];
      Serial.readBytes(NIST_time_buffer, 4);
      NIST_time = 0;
      for(uint8_t i=0; i<4; i++) NIST_time += (uint32_t)NIST_time_buffer[i] << 8*(3-i);
      Serial.print("NIST time received: ");
      Serial.println(NIST_time, HEX);
      
      NIST_time -= epoch_offset;  // remove epoch offset, so time starts on 1970-01-01
      setTime(NIST_time);         // set on-board time to new time
      LCD_display_time();         // display time on LCD
      Serial_print_time();        // display time on Serial monitor
      Serial.println();
    }
  }
}

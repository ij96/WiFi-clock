#include "LED.h"

LED::LED(uint8_t pin_number){
  pin = pin_number;
}

void LED::Switch(bool sw){
  digitalWrite(pin, sw);
}

void LED::init(){
  pinMode(pin,OUTPUT);
  Switch(LED_OFF);
}

void LED::flash(uint32_t delay_time){
  Switch(LED_ON);
  delay(delay_time);
  Switch(LED_OFF);
  delay(delay_time);
}

#include "LED.h"

LED::LED(uint8_t pin_number){pin = pin_number;}

void LED::ON(){digitalWrite(pin, LED_ON);}

void LED::OFF(){digitalWrite(pin, LED_OFF);}

void LED::init(){
  pinMode(pin,OUTPUT);
  OFF();
}

void LED::flash(uint32_t delay_time){
  ON();
  delay(delay_time);
  OFF();
  delay(delay_time);
}

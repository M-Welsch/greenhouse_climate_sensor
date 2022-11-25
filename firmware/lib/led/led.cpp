#include <Arduino.h>

#include "led.h"

void Led::setup(uint8_t gpio_pin) {
  ledPin = gpio_pin;
  pinMode(LED_BUILTIN, OUTPUT); 
}

void Led::flash() {
  on();
  delay(50);
  off();
}

void Led::on() {
  digitalWrite(ledPin, LOW);
}

void Led::off() {
  digitalWrite(ledPin, HIGH);
}

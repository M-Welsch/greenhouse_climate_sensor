#include <Arduino.h>

#include "button.h"

void Button::setup(uint8_t gpio_pin) {
    pin = gpio_pin;
    pinMode(pin, INPUT_PULLUP);
}

bool Button::pressed() {
    if (digitalRead(pin))
        return false;
    else
        return true;
}

#ifndef _button_h__
#define _button_h__

#include <stdint.h>


class Button {
    uint8_t pin;
  public:
    void setup(uint8_t gpio_pin);
    bool pressed();
};

#endif
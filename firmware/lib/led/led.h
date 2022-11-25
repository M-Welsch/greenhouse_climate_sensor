#ifndef _led_h__
#define _led_h__

#include <stdint.h>

class Led {
    uint8_t ledPin;
  public:
    void setup(uint8_t gpio_pin);
    void flash();
    void on();
    void off();
};

#endif
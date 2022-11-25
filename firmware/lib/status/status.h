#ifndef _status_data_h_
#define _status_data_h_

#include <stdbool.h>

typedef struct {
  float insideTemperature;
  float insideHumidity;
  float insideDewPoint;
  float batteryVoltage;
  bool batteryUndervoltage;
  char ip_address[16];
} status_t;

#endif
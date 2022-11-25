#ifndef _status_data_h_
#define _status_data_h_

typedef struct {
  float insideTemperature;
  float insideHumidity;
  float insideDewPoint;
  float outsideTemperature;
  float outsideHumidity;
  float outsideDewPoint;
  char ip_address[16];
} status_t;

#endif
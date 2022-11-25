#include <Arduino.h>
#include "status.h"

void log_to_console(status_t *status) {
    Serial.printf("Ti = %1.f, To = %1.f\n", status->insideTemperature, status->outsideTemperature);
}
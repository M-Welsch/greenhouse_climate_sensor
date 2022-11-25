#include <Arduino.h>
#include "status.h"

String concat_json(Status *status);
void add_json_entry(char *buffer, String *json_buffer, const char *name, const float data);
#include <Arduino.h>
#include "status.h"
#include "json.h"

String concat_json(Status *status) {
    char buffer[255];
    String json = "{";
    add_json_entry(buffer, &json, "probe_temperature", status->probe_temperature);
    add_json_entry(buffer, &json, "ambient_temperature", status->ambient_temperature);
    add_json_entry(buffer, &json, "battery_voltage", status->battery_voltage);
    json.concat("\"\":0}");
    return json;
}

void add_json_entry(char *buffer, String *json_buffer, const char *name, const float data) {
    sprintf(buffer, "\"%s\": %.2f,", name, data);
    json_buffer->concat(buffer);
}
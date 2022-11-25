#include <Arduino.h>
#include "status.h"

String concat_data_table(Status *status);
void add_table_row(char *buffer, String *html_buffer, const char *name, const char *unit, float data);
void add_table_row(char *buffer, String *html_buffer, const char *name, const char *unit, int32 data);
void add_table_row(char *buffer, String *html_buffer, const char *name, const char *unit, bool data);
#include "status.h"
#include "DHTesp.h"

#define DHT_INSIDE_GPIO 4

typedef enum {
    dpSUCCESS, dpERROR
} dpError_e;

class Dht {
        DHTesp dhtInside;
    public:
        void setup();
        dpError_e getValues(status_t *status);
};
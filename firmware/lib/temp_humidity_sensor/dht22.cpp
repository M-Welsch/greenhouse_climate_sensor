#include <cmath>

#include "DHTesp.h"

#include "dht22.h"

#define MAGNUS_CONSTANT 6.1078f
#define PARAM_A_ABOVE_ZERO 7.5f
#define PARAM_A_BELOW_ZERO 7.6f
#define PARAM_B_ABOVE_ZERO 237.3f
#define PARAM_B_BELOW_ZERO 240.7f


void Dht::setup() {
    Serial.println("setting up dhts");
    dhtInside.setup(DHT_INSIDE_GPIO, DHTesp::DHT22);
    Serial.println("set up dhts");
}

static inline float _a(const float temperature) {
    return temperature > 0 ? PARAM_A_ABOVE_ZERO : PARAM_A_BELOW_ZERO;
}

static inline float _b(const float temperature) {
    return temperature > 0 ? PARAM_B_ABOVE_ZERO : PARAM_B_BELOW_ZERO;
}

static float _magnusFormula(const float temperature) {
    float a = _a(temperature);
    float b = _b(temperature);
    float exponent = (a * temperature) / (b + temperature);
    return MAGNUS_CONSTANT * pow(10, exponent);
}


float _dewPoint(const float temperature, const float humidity) {
    float magnus = _magnusFormula(temperature);
    float bracket = (magnus * humidity/100) / MAGNUS_CONSTANT;
    float numerator = _b(temperature) * log10(bracket);
    float denominator = _a(temperature) - log10(bracket);
    return numerator/denominator;
}

dpError_e Dht::getValues(status_t *status) {
    delay(dhtInside.getMinimumSamplingPeriod());
    status->insideTemperature = dhtInside.getTemperature();
    status->insideHumidity = dhtInside.getHumidity();
    status->insideDewPoint = _dewPoint(status->insideTemperature, status->insideHumidity);
    return dpSUCCESS;
}
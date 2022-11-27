#include <Arduino.h>

#include <battery_monitor.h>

#define UNDERVOLTAGE_THRESHOLD 3.27  // Source: https://blog.ampow.com/lipo-voltage-chart/

float BatteryMonitor::voltage() {
    int raw_value = analogRead(ANALOGPIN);
    recent_voltage_level = ((float) raw_value) * 0.004556406962632479 -0.086253466499661330;
    return recent_voltage_level;
}

void BatteryMonitor::getVoltage(status_t *status) {
    status->batteryVoltage = voltage();
    status->batteryUndervoltage = undervoltage(status->batteryVoltage);
}

bool BatteryMonitor::undervoltage(const float voltage) {
    return voltage < UNDERVOLTAGE_THRESHOLD;
}
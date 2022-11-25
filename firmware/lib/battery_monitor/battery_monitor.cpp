#include <Arduino.h>

#include <battery_monitor.h>

#define UNDERVOLTAGE_THRESHOLD 3.27  // Source: https://blog.ampow.com/lipo-voltage-chart/

float BatteryMonitor::voltage() {
    int raw_value = analogRead(ANALOGPIN);
    recent_voltage_level = (float) raw_value * 0.00471 - 0.0667;
    return recent_voltage_level;
}

bool BatteryMonitor::undervoltage(const float voltage) {
    return voltage < UNDERVOLTAGE_THRESHOLD;
}
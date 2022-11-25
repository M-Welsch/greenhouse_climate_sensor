#define ANALOGPIN A0

#include "status.h"

class BatteryMonitor {
    private:
        float recent_voltage_level;
    public:
        float voltage();
        void getVoltage(status_t *status);
        bool undervoltage(const float voltage);
};
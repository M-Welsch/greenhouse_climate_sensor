#define ANALOGPIN A0

class BatteryMonitor {
    private:
        float recent_voltage_level;
    public:
        float voltage();
        bool undervoltage(const float voltage);
};
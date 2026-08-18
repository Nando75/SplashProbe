#ifndef BATTERY_MANAGER_H
#define BATTERY_MANAGER_H

#include <Arduino.h>

struct BatteryData {
    float voltage;     // es. 3.90
    int percentage;    // es. 66
};

class BatteryManager {
public:
    BatteryManager();
    void begin();
    BatteryData readBattery();

private:
    static constexpr float LIPO_VOLTAGE_MIN = 3.3f; // 3.3V = 0%
    static constexpr float LIPO_VOLTAGE_MAX = 4.2f; // 4.2V = 100%
};

#endif
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
    const uint16_t MIN_VOLTAGE_MV = 3300; // 3.3V = 0%
    const uint16_t MAX_VOLTAGE_MV = 4200; // 4.2V = 100%
};

#endif
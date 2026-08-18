#include "BatteryManager.h"

BatteryManager::BatteryManager() {
}

void BatteryManager::begin() {}


BatteryData BatteryManager::readBattery() {
    BatteryData data;
    // 1. Reading the actual no-load voltage
    data.voltage = getBatteryVoltage() / 1000.0f;
    // 2. Calculate percentage with your precise float formula
    if (data.voltage <= LIPO_VOLTAGE_MIN) {
        data.percentage = 0;
    } else if (data.voltage >= LIPO_VOLTAGE_MAX) {
        data.percentage = 100;
    } else {
        float percentage = (data.voltage - LIPO_VOLTAGE_MIN) / (LIPO_VOLTAGE_MAX - LIPO_VOLTAGE_MIN) * 100.0f;
        data.percentage = static_cast<int>(percentage);
    }
    return data;
}



#pragma once
#include <OneWire.h>
#include <DallasTemperature.h>
#include "sys.h"

class DS18Manager {
public:
    DS18Manager();

    void begin();
    void updateData();
    
    float getAirTemperature() const;
    float getWaterTemperature() const;
    void printAddresses();
    void setAirOffset(float off);
    void setWaterOffset(float off);
    
private:
    OneWire _oneWire;
    DallasTemperature _sensors;
    /*
        DS18B20 found:
        Sensor 0: 28 5E C7 61 54 22 8 38 
        Sensor 1: 28 21 FA 97 54 22 8 94
    */
    DeviceAddress _airSensor = {0x28, 0x5E, 0xC7, 0x61, 0x54, 0x22, 0x08, 0x38};
    DeviceAddress _waterSensor = {0x28, 0x21, 0xFA, 0x97, 0x54, 0x22, 0x08, 0x94};
    
    float _tempAir = NAN;
    float _tempWater = NAN;
    bool findSensors();
    bool isValidAddress(const DeviceAddress addr);
    float toF(float c) const;
    void setAirSensor(const DeviceAddress addr);
    void setWaterSensor(const DeviceAddress addr);
    float _airOffset = 0.0f;
    float _waterOffset = 0.0f;
};

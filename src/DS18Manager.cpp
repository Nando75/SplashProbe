#include "DS18Manager.h"

DS18Manager::DS18Manager()
    : _oneWire(PIN_DS18), _sensors(&_oneWire)
{
}

void DS18Manager::begin()
{
    _sensors.begin();
    //findSensors();

}

bool DS18Manager::isValidAddress(const DeviceAddress addr)
{
    for (int i = 0; i < 8; i++)
        if (addr[i] != 0x00 && addr[i] != 0xFF)
            return true;

    return false;
}

bool DS18Manager::findSensors()
{
    int count = _sensors.getDeviceCount();
    if (count < 2) return false;

    // Trova i primi due sensori validi
    int found = 0;
    DeviceAddress addr;

    for (int i = 0; i < count; i++) {
        if (_sensors.getAddress(addr, i)) {
            if (isValidAddress(addr)) {
                if (found == 0) memcpy(_airSensor, addr, 8);
                if (found == 1) memcpy(_waterSensor, addr, 8);
                found++;
            }
        }
        if (found == 2) break;
    }

    return (found == 2);
}

void DS18Manager::updateData()
{
    _sensors.requestTemperatures();
    // Lettura aria
    if (isValidAddress(_airSensor))
        _tempAir = _sensors.getTempC(_airSensor);
    else
        _tempAir = NAN;
    // Lettura acqua
    if (isValidAddress(_waterSensor))
        _tempWater = _sensors.getTempC(_waterSensor);
    else
        _tempWater = NAN;

}


float DS18Manager::getAirTemperature() const
{
    float t = (dataMode == Farfahrenheit) ? toF(_tempAir + _airOffset) : _tempAir + _airOffset;
    return t;
}

float DS18Manager::getWaterTemperature() const
{
    float t = (dataMode == Farfahrenheit) ? toF(_tempWater + _waterOffset) : _tempWater + _waterOffset;
    return t;
}


float DS18Manager::toF(float c) const {
    return c * 1.8f + 32.0f;
}


void DS18Manager::printAddresses() {
    if(!debug) return;
    Serial.println("DS18B20 found:");
    for (int i = 0; i < _sensors.getDeviceCount(); i++) {
        DeviceAddress addr;
        if (_sensors.getAddress(addr, i)) {
            Serial.print("Sensor ");
            Serial.print(i);
            Serial.print(": ");
            for (int j = 0; j < 8; j++) {
                Serial.print(addr[j], HEX);
                Serial.print(" ");
            }
            Serial.println();
        }
    }
}


void DS18Manager::setAirSensor(const DeviceAddress addr) {
    memcpy(_airSensor, addr, 8);
}

void DS18Manager::setWaterSensor(const DeviceAddress addr) {
    memcpy(_waterSensor, addr, 8);
}


void DS18Manager::setAirOffset(float off) {
    _airOffset = off;
}

void DS18Manager::setWaterOffset(float off) {
    _waterOffset = off;
}

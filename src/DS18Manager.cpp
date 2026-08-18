#include "DS18Manager.h"

DS18Manager::DS18Manager()
    : _oneWire(PIN_DS18), _sensors(&_oneWire)
{
}


void DS18Manager::begin()
{
    _sensors.begin();
    //_sensors.setWaitForConversion(false); 
    bool airPresent = _sensors.isConnected(_airSensor);
    bool waterPresent = _sensors.isConnected(_waterSensor);
    if (!airPresent || !waterPresent) {
        if(debug) Serial.println("Sensor addresses not found, starting search...");
        if(findSensors())
        {
            if(debug) printAddresses();
        }
        else
        {
            if(debug)
            {
                Serial.println(" ** No sensors detected. ** ");
                Serial.println("Check the DS18B20 circuit:");
                Serial.println("GND,3.3V,");
                Serial.println("and pullup resistor between the data line and 3.3V to pin: 'GPIO0'");
            } 
        }
    }


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
    int found = 0;
    DeviceAddress addr;
    for (int i = 0; i < count; i++) {
        if (_sensors.getAddress(addr, i)) {
            if (isValidAddress(addr)) {
                if (found == 0) { 
                    memcpy(_airSensor, addr, 8); 
                    if(debug) Serial.println("Found " + getAddress(addr) + " assigned as Air Sensor." );
                }
                if (found == 1) { 
                    memcpy(_waterSensor, addr, 8); 
                    if(debug) Serial.println("Found " + getAddress(addr) + " assigned as Water Sensor." );
                }
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

    if (isValidAddress(_airSensor) && _sensors.isConnected(_airSensor)) {
        float temp = _sensors.getTempC(_airSensor);
        _tempAir = (temp == DEVICE_DISCONNECTED_C) ? NAN : temp;
    } else {
        _tempAir = NAN;
    }

    if (isValidAddress(_waterSensor) && _sensors.isConnected(_waterSensor)) {
        float temp = _sensors.getTempC(_waterSensor);
        _tempWater = (temp == DEVICE_DISCONNECTED_C) ? NAN : temp;
    } else {
        _tempWater = NAN;
    }
}


float DS18Manager::getAirTemperature() const
{
    float t = (dataMode == Fahrenheit) ? toF(_tempAir + _airOffset) : _tempAir + _airOffset;
    return t;
}

float DS18Manager::getWaterTemperature() const
{
    float t = (dataMode == Fahrenheit) ? toF(_tempWater + _waterOffset) : _tempWater + _waterOffset;
    return t;
}


float DS18Manager::toF(float c) const {
    return c * 1.8f + 32.0f;
}

String DS18Manager::getAddress(DeviceAddress sensor)
{
    char buf[24]; // 8 byte x 2 words + 7 spaces + '\0' = 24 chars
    snprintf(buf, sizeof(buf), "%02X %02X %02X %02X %02X %02X %02X %02X",
             sensor[0], sensor[1], sensor[2], sensor[3],
             sensor[4], sensor[5], sensor[6], sensor[7]);
    return String(buf);
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
            Serial.print(getAddress(addr));
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

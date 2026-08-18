#include "Arduino.h"
#include "sys.h"
#include "DeepSleepManager.h"
#include "RgbLedManager.h"
#include "DisplayManager.h"
#include "DS18Manager.h"
#include "BatteryManager.h"

DeepSleepManager sleepManager;
RgbLedManager ledManager;
DisplayManager displayManager;
DS18Manager tempManager;
BatteryManager batteryManager;

bool firstLoop = true;
uint8_t sleepCycleCount = 0;

void printData(float tAir, float tWater, float vBatt, int pBatt)
{
    if (!debug) return;
    Serial.println("[INFO] I perform sensor reading and ePaper update:");
    Serial.print("Air Temp: ");
    Serial.println(tAir);
    Serial.print("Water Temp: ");
    Serial.println(tWater);
    Serial.println("");
    Serial.print("Battery Voltage: ");
    Serial.print(vBatt);
    Serial.print(" V | Percentage: ");
    Serial.print(pBatt);
    Serial.println(" %");
}


void setup() {
    // CubeCell init clock MCU:
    boardInitMcu();
    sleepManager.begin();
    sleepManager.powerOnPeripherals();
    if(debug) Serial.begin(115200);
    tempManager.begin();
    //If Necessary to tare sensors:
    //tempManager.setAirOffset(-5.2f); // calculated in degrees Celsius
    //tempManager.setWaterOffset(-5.2f); //calculated in degrees Celsius
    ledManager.begin();
}

void loop() {
    switch (currentState) {
        case STATE_READ_SENSORS: {
                if (sleepCycleCount == 0 || firstLoop) {
                    tempManager.updateData();
                    const float tAir = tempManager.getAirTemperature();
                    const float tWater = tempManager.getWaterTemperature();
                    uint8_t r=0; uint8_t g=255; uint8_t b=0;
                    if(tAir == NAN || tWater == NAN) { r=255; g=0; b=0; }
                    ledManager.setBoardColor(r, g, b);
                    ledManager.ringSpin(r, g, b, 160, 3);
                    //if (debug) tempManager.printAddresses();
                    BatteryData batt = batteryManager.readBattery();
                    if (debug) printData(tAir,tWater,batt.voltage,batt.percentage);
                    if(firstLoop) { displayManager.init(); firstLoop=false; }
                    displayManager.UpdateScreen(
                        tempManager.getAirTemperature(),
                        tempManager.getWaterTemperature(),
                        3.2, //batt.voltage,
                        10 //batt.percentage
                    );

                } else {
                    ledManager.setBoardColor(0, 0, 255);
                    ledManager.ringSpin(0, 0, 255, 160, 3);
                    if (debug) Serial.printf("[INFO] Intermediate wake-up cycle %d/%d. LED spin only.\n", sleepCycleCount, SENSORS_EVERY_N_CYCLES);
                }
                sleepCycleCount = (sleepCycleCount + 1) % SENSORS_EVERY_N_CYCLES;                
                currentState = STATE_SLEEP;
            break;
        }
        
        case STATE_SLEEP: {
            if(debug) Serial.printf("[SLEEP] I enter deep sleep for 30 minutes (%lu ms)...\n", SLEEP_DURATION_MS);
            ledManager.clearAll();
            sleepManager.sleepForMillis(SLEEP_DURATION_MS);
            if(debug) Serial.println("[ALARM] 30m timer expired!");
            currentState = STATE_READ_SENSORS;
            break;
        }
    }
}


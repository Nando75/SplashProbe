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
    Serial.println("[INFO] Eseguo lettura sensori e aggiornamento ePaper:");
    Serial.print("Temp Aria: ");
    Serial.print(tAir);
    Serial.print("Temp Acqua: ");
    Serial.print(tWater);
    Serial.println(" %");
    Serial.print("Tensione Batteria: ");
    Serial.print(vBatt);
    Serial.print(" V | Percentuale: ");
    Serial.print(pBatt);
    Serial.println(" %");
}


void setup() {
    // FONDAMENTALE per CubeCell: inizializza le periferiche interne e i clock MCU
    boardInitMcu();

    sleepManager.begin();
    sleepManager.powerOnPeripherals();
    if(debug) Serial.begin(115200);
    tempManager.begin();
    //If Necessary to tare sensors:
    //tempManager.setAirOffset(-5.2f); // calculated in degrees Celsius
    //tempManager.setWaterOffset(-5.2f); //calculated in degrees Celsius
    ledManager.begin();

    // Inizializza il BatteryManager
    //batteryManager.begin();

}

void loop() {
    switch (currentState) {
        case STATE_READ_SENSORS: {
                //ledManager.doubleFlash(0, 0, 255, 200, 200); //blue
                //ledManager.setBoardColor(0, 0, 255);
                // Nel frattempo fai un giro di ring verde
                //ledManager.ringSpin(0, 0, 255, 160, 3);
                if (sleepCycleCount == 0 || firstLoop) {
                    ledManager.setBoardColor(0, 255, 0);
                    ledManager.ringSpin(0, 0, 255, 160, 3);
                    if (debug) tempManager.printAddresses();
                    tempManager.updateData();
                    if(firstLoop) { displayManager.init(); firstLoop=false; }
                    const float tAir = tempManager.getAirTemperature();
                    const float tWater = tempManager.getWaterTemperature();
                    BatteryData batt = batteryManager.readBattery();
                    if (debug) printData(tAir,tWater,batt.voltage,batt.percentage);
                   //ledManager.doubleFlash(0, 255, 0, 200, 200); 
                    //ledManager.doubleFlash(0, 255, 0, 200, 200); 
                    
                    displayManager.UpdateScreen(
                        tempManager.getAirTemperature(),
                        tempManager.getWaterTemperature(),
                        batt.voltage,
                        batt.percentage
                    );

                } else {
                    ledManager.setBoardColor(0, 0, 255);
                    ledManager.ringSpin(0, 0, 255, 160, 3);

                   if (debug) Serial.printf("[INFO] Risveglio intermedio 5 min (%d/%d). Solo spin LED.\n", sleepCycleCount, SENSORS_EVERY_N_CYCLES);
                }
                                
                // Incrementa il contatore per il prossimo ciclo (modulo 6)
                sleepCycleCount = (sleepCycleCount + 1) % SENSORS_EVERY_N_CYCLES;                
                currentState = STATE_SLEEP;
            break;
        }
        
        case STATE_SLEEP: {
            if(debug) Serial.printf("[SLEEP] Entro in Deep Sleep per 30 minuti (%lu ms)...\n", SLEEP_DURATION_MS);
            ledManager.clearAll();
            sleepManager.sleepForMillis(SLEEP_DURATION_MS);
            if(debug) Serial.println("[SVEGLIA] Timer 30m scaduto !");
            currentState = STATE_READ_SENSORS;
            break;
        }
    }
}


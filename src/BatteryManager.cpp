#include "BatteryManager.h"

#define LITHIUM_VMIN 3.0f
#define LITHIUM_VMAX 4.1f

BatteryManager::BatteryManager() {
}

void BatteryManager::begin() {
    // Nessuna inizializzazione necessaria
}

BatteryData BatteryManager::readBattery() {
    BatteryData data;

    // 1. SPEGNE Vext (HIGH) temporaneamente per staccare i carichi parassiti
    //pinMode(Vext, OUTPUT);
    //digitalWrite(Vext, HIGH);

    // 2. Mette GPIO7 in alta impedenza (INPUT)
    //pinMode(GPIO7, INPUT);
    //delayMicroseconds(5);

    // 3. Lettura della tensione reale a vuoto
    data.voltage = getBatteryVoltage() / 1000.0f;

    // 4. RIACCENDE Vext (LOW) per ridare corrente al resto del circuito!
    //digitalWrite(Vext, LOW);


    // 3. Calcolo percentuale con la tua formula float precisa
    if (data.voltage <= LITHIUM_VMIN) {
        data.percentage = 0;
    } else if (data.voltage >= LITHIUM_VMAX) {
        data.percentage = 100;
    } else {
        float percentage = (data.voltage - LITHIUM_VMIN) / (LITHIUM_VMAX - LITHIUM_VMIN) * 100.0f;
        data.percentage = static_cast<int>(percentage);
    }

    return data;
}
#include "DeepSleepManager.h"

DeepSleepManager* DeepSleepManager::instance = nullptr;

DeepSleepManager::DeepSleepManager() : inSleepMode(false) {
    instance = this;
}

void DeepSleepManager::begin() {
    pinMode(Vext, OUTPUT);
    powerOnPeripherals(); // Attivo all'avvio
    TimerInit(&sleepTimer, DeepSleepManager::onSleepTimerEndCallback);
}

void DeepSleepManager::powerOnPeripherals() {
    digitalWrite(Vext, LOW); // LOW = Alimentazione attiva su Vext
    delay(10);               // Stabilizzazione alimentazione
}

void DeepSleepManager::powerOffPeripherals() {
    digitalWrite(Vext, HIGH); // HIGH = Periferiche spente
}

void DeepSleepManager::onSleepTimerEndCallback() {
    if (instance) {
        instance->inSleepMode = false;
    }
}

void DeepSleepManager::sleepForMillis(uint32_t durationMs) {
    // Spegne la seriale prima dello sleep
    Serial.flush();
    // Configura e avvia il timer RTC
    TimerSetValue(&sleepTimer, durationMs);
    TimerStart(&sleepTimer);
    inSleepMode = true;
    powerOffPeripherals();
    // Blocco in deep sleep finché il timer non scade
    while (inSleepMode) {
        lowPowerHandler(); // Entra nel sleep profondo di CubeCell
    }

    // Risveglio: riattiva l'alimentazione Vext
    powerOnPeripherals();
}

bool DeepSleepManager::isSleeping() const {
    return inSleepMode;
}
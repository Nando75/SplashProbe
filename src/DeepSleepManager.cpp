#include "DeepSleepManager.h"

DeepSleepManager* DeepSleepManager::instance = nullptr;

DeepSleepManager::DeepSleepManager() : inSleepMode(false) {
    instance = this;
}

void DeepSleepManager::begin() {
    pinMode(Vext, OUTPUT);
    powerOnPeripherals(); // Active at startup
    TimerInit(&sleepTimer, DeepSleepManager::onSleepTimerEndCallback);
}

void DeepSleepManager::powerOnPeripherals() {
    digitalWrite(Vext, LOW); // LOW = Power on Vext
    delay(10);               // Power Stabilization
}

void DeepSleepManager::powerOffPeripherals() {
    digitalWrite(Vext, HIGH); // HIGH = Peripherals off
}

void DeepSleepManager::onSleepTimerEndCallback() {
    if (instance) {
        instance->inSleepMode = false;
    }
}

void DeepSleepManager::sleepForMillis(uint32_t durationMs) {
    // Turn off the serial port before sleep
    Serial.flush();
    // Configure and start the RTC timer
    TimerSetValue(&sleepTimer, durationMs);
    TimerStart(&sleepTimer);
    inSleepMode = true;
    powerOffPeripherals();
    // Deep sleep block until the timer expires
    while (inSleepMode) {
        lowPowerHandler(); // Enter CubeCell's deep sleep
    }

    // Awakening: Reactivates Vext power
    powerOnPeripherals();
}

bool DeepSleepManager::isSleeping() const {
    return inSleepMode;
}
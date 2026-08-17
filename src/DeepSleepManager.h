#ifndef DEEP_SLEEP_MANAGER_H
#define DEEP_SLEEP_MANAGER_H

#include "Arduino.h"

class DeepSleepManager {
private:
    TimerEvent_t sleepTimer;
    volatile bool inSleepMode;

    static DeepSleepManager* instance;
    static void onSleepTimerEndCallback();

public:
    DeepSleepManager();
    void begin();
    void powerOnPeripherals();
    void powerOffPeripherals();
    void sleepForMillis(uint32_t durationMs);
    bool isSleeping() const;
};

#endif
#pragma once
#include <Arduino.h>
#include "config.h"

namespace Utils {
    void init();
    void startTone(int frequency, int duration, bool loopMode = false);
    void stopTone();
    void updateBuzzer();
}
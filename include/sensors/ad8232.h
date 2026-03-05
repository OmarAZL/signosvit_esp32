#pragma once
#include "config.h"
#include <Arduino.h>

namespace AD8232 {
    void init();
    int readECG();
    int readLOplus();
    int readLOminus();

    bool electrodesConnected();
}
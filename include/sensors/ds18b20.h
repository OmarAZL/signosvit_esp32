#pragma once
#include "config.h"
#include <OneWire.h>
#include <DallasTemperature.h>

namespace DS18B20 {
    void init();
    bool isConnected();
    float getTemperature();
}
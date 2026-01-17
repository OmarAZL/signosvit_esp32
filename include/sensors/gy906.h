#pragma once
#include "Wire.h"
#include "config.h"

namespace GY906 {
    void init();
    float getTempObject();
    float getTempAmbient();
}
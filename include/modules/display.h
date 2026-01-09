#pragma once
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "config.h"

extern Adafruit_SSD1306 display;

namespace Display {
    bool init();
    void clear();
    void updateDisplay();
    void showMessage(const String& message);
}
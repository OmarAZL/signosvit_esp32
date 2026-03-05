#pragma once

#include "EasyNextionLibrary.h"

namespace Nextion {
    void init();
    void start();
    void write(float &temp1, float &temp2, int &spo2, int &bpm, int &ecg, bool &electrodes);
}

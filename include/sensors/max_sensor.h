#pragma once
#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"

namespace MaxSensor {
    // Inicializa el hardware y la configuración de LEDs
    bool init();

    // Captura muestras y procesa BPM/SpO2
    void update();

    // Getters para los resultados
    int32_t getBPM();
    int32_t getSpO2();
    bool isDataValid();
    bool hasNewData();
    bool isReadingData();
}
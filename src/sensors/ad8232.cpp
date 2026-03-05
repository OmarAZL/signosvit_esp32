#include "sensors/ad8232.h"

namespace AD8232 {
    void init() {
        pinMode(AD8232_OUT_PIN, INPUT);
        pinMode(AD8232_LOPLUS_PIN, INPUT);
        pinMode(AD8232_LOMINUS_PIN, INPUT);

        // Configurar la atenuación del ADC para el pin de salida del ECG
        // ADC_11db es adecuado para un rango de 0-2.5V o 0-3.1V, que es común para el AD8232 [8, 33]
        analogSetAttenuation(ADC_11db); 
        // Opcional: Establecer la resolución del ADC (por defecto es 12 bits) [8, 33]
        analogReadResolution(12);
    }

    int readECG() {
        return analogRead(AD8232_OUT_PIN);
    }

    int readLOplus() {
        return digitalRead(AD8232_LOPLUS_PIN);
    }

    int readLOminus() {
        return digitalRead(AD8232_LOMINUS_PIN);
    }

    bool electrodesConnected() {
        // Si cualquiera de los pines LO+ o LO- está en HIGH, los electrodos están desconectados
        return !(readLOplus() == HIGH || readLOminus() == HIGH);
    }


}
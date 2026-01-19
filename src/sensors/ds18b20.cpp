#include "sensors/ds18b20.h"

OneWire oneWire(DS18B20_PIN);
DallasTemperature sensors(&oneWire);

namespace DS18B20 {
    void init() {
        sensors.begin();
        sensors.setResolution(DS18B20_RESOLUTION);
        // No bloquear esperando la conversión: solicitamos la conversión
        // y leemos la última temperatura disponible cuando se pida.
        sensors.setWaitForConversion(false);
        sensors.requestTemperatures(); // iniciar primera conversión
    }

    bool isConnected() {
        if (sensors.getDeviceCount() > 0) {
            return true;
        }
        return false; // No device found
    }

    float getTemperature() {
        // Solicita una conversión de forma no bloqueante y devuelve
        // la última temperatura convertida (si no hay nueva, devuelve la anterior).
        sensors.requestTemperatures();
        float temperature = sensors.getTempCByIndex(0);
        if (temperature == DEVICE_DISCONNECTED_C) {
            return NAN;
        }
        return temperature;
    }

}
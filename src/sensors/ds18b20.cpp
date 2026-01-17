#include "sensors/ds18b20.h"

OneWire oneWire(DS18B20_PIN);
DallasTemperature sensors(&oneWire);

namespace DS18B20 {
    void init() {
        sensors.begin();
        sensors.setResolution(DS18B20_RESOLUTION);
    }

    bool isConnected() {
        if (sensors.getDeviceCount() > 0) {
            return true;
        }
        return false; // No device found
    }

    float getTemperature() {
        sensors.requestTemperatures();
        float temperature = sensors.getTempCByIndex(0);
        if (temperature == DEVICE_DISCONNECTED_C) {
            return NAN;
        }
        return temperature;
    }

}
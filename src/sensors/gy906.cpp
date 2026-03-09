#include "sensors/gy906.h"
#include "Arduino.h"

namespace GY906 {
    void init() {
        Wire.beginTransmission(GY906_ADDRESS);
        Wire.write(0x01);
        Wire.write(0x00);
        uint8_t error = Wire.endTransmission();
        if (error != 0) {
            Serial.print("Error inicializando GY906 (MLX90614): ");
            Serial.println(error);
            return;
        }
        delay(100);
        Serial.println("GY906 inicializado correctamente.");
    }

    float getTemperature(uint8_t reg) {
        Wire.setClock(100000UL); // 100 kHz estándar
        Wire.beginTransmission(GY906_ADDRESS);
        Wire.write(reg);
        uint8_t error = Wire.endTransmission(false);
        if (error != 0) {
            Serial.print("Error en transmisión I2C GY906: ");
            Serial.println(error);
            return -127;
        }
        Wire.requestFrom(GY906_ADDRESS, 3);
        if (Wire.available() >= 3) {
            uint16_t temp = Wire.read();
            temp |= Wire.read() << 8;
            Wire.read();
            return (temp * 0.02) - 273.15;
        }
        Serial.println("Datos insuficientes de GY906.");
        return -127;
    }

    float getTempObject() {
        return getTemperature(0x07);
    }

    float getTempAmbient() {
        return getTemperature(0x06);
    }
}
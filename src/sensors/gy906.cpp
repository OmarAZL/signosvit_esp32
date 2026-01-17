#include "sensors/gy906.h"

namespace GY906 {
    void init() {
        Wire.beginTransmission(GY906_ADDRESS);
        Wire.write(0x01);
        Wire.write(0x00);
        Wire.endTransmission();
        delay(100);
    }

    float getTemperature(uint8_t reg) {
        Wire.beginTransmission(GY906_ADDRESS);
        Wire.write(reg);
        Wire.endTransmission(false);
        Wire.requestFrom(GY906_ADDRESS, (uint8_t)3);
        if (Wire.available() >= 3) {
            uint16_t temp = Wire.read();
            temp |= Wire.read() << 8;
            Wire.read();
            return (temp * 0.02) - 273.15;
        }
        return NAN;
    }

    float getTempObject() {
        return getTemperature(0x07);
    }

    float getTempAmbient() {
        return getTemperature(0x06);
    }
}
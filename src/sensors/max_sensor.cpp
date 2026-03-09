#include "sensors/max_sensor.h"

namespace MaxSensor {
    MAX30105 sensor;

    const int32_t bufferLength = 100; 
    uint32_t irBuffer[bufferLength];
    uint32_t redBuffer[bufferLength];
    int32_t bufferPointer = 0; // Índice actual del buffer

    int32_t spo2 = 0;
    int8_t validSPO2 = 0;
    int32_t heartRate = 0;
    int8_t validHeartRate = 0;
    bool newDataAvailable = false;
    bool readingData = false;

    bool init() {
        if (!sensor.begin(Wire, I2C_SPEED_STANDARD)) {
            Serial.println("Error inicializando MAX30105: dispositivo no encontrado.");
            return false;
        }

        // Configuración específica para oximetría
        byte ledBrightness = 60; 
        byte sampleAverage = 4; 
        byte ledMode = 2; // Red + IR
        int sampleRate = 100; 
        int pulseWidth = 411; 
        int adcRange = 4096; 

        sensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
        Serial.println("MAX30105 inicializado correctamente.");
        return true;
    }

    void update() {
        newDataAvailable = false;
        sensor.check(); // Revisa si el sensor tiene datos listos

        while (sensor.available()) {
            if (!readingData) {
                readingData = true;
                Serial.println("Leyendo datos del sensor MAX30105...");
            }
            redBuffer[bufferPointer] = sensor.getRed();
            irBuffer[bufferPointer] = sensor.getIR();
            sensor.nextSample();
            bufferPointer++;

            // Cuando el buffer se llena, calculamos
            if (bufferPointer >= bufferLength) {
                Serial.println("Datos del MAX30105 procesados. BPM y SpO2 calculados.");
                maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
                bufferPointer = 0; // Reiniciamos para la siguiente ráfaga
                newDataAvailable = true;
                readingData = false;
            }
        }
    }

    int32_t getBPM() { return heartRate; }
    int32_t getSpO2() { return spo2; }
    bool isDataValid() { return (validSPO2 == 1 && validHeartRate == 1); }
    bool hasNewData() { return newDataAvailable; }
    bool isReadingData() { return readingData; }
}


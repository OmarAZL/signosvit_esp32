#include <Arduino.h>
#include "modules/mqtt.h"
#include "modules/displayNextion.h"
#include "modules/utils.h"

#include "modules/display.h"

#include "sensors/ad8232.h"
#include "sensors/ds18b20.h"
#include "sensors/gy906.h"
#include "sensors/max_sensor.h"

const bool enabledBuzzer = true;

// Función para escanear dispositivos I2C conectados
void scanI2C() {
    Serial.println("Escaneando dispositivos I2C...");
    int nDevices = 0;
    for (byte address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        byte error = Wire.endTransmission();
        if (error == 0) {
            Serial.print("Dispositivo I2C encontrado en 0x");
            if (address < 16) Serial.print("0");
            Serial.println(address, HEX);
            nDevices++;
        } else if (error == 4) {
            Serial.print("Error desconocido en 0x");
            if (address < 16) Serial.print("0");
            Serial.println(address, HEX);
        }
    }
    if (nDevices == 0) {
        Serial.println("No se encontraron dispositivos I2C.");
    } else {
        Serial.println("Escaneo completado.");
    }
}


bool isTimerExpired(unsigned long &lastUpdate, uint32_t interval) {
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate >= interval) {
    lastUpdate = currentMillis; 
    return true;
  }
  return false;
}

void setup() {
    Serial.begin(115200);
    // --- inicializar bus I2C antes de usar cualquier sensor ---
    Wire.begin(); // pines por defecto (SDA=21, SCL=22 en ESP32)
    Wire.setClock(I2C_SPEED_STANDARD); // empieza a 100 kHz (compatible con MLX90614)
    scanI2C(); // Escanea dispositivos I2C para verificar conexiones

    Nextion::init();
    MQTT::init();
    DS18B20::init();
    GY906::init();
    AD8232::init();
    Utils::init();
    MaxSensor::init();

    delay(1000);
    Nextion::start();
    Serial.println("\nEnviando mensajes.");
}

String payload;
float temp1;
float temp2;
int spo2;
int bpm;
int ecg;
bool electrodes = false;

// variables para el sonido de latido cardiaco (ECG-style)
int previousEcg = 0;                        // lectura anterior para detectar cruce
unsigned long lastEcgPeakTime = 0;          // tiempo del último cruce válido

// parámetros de detección (ajustar según calibración del AD8232)
const int ECG_THRESHOLD = 2000;             // valor analógico que indica un pico
const unsigned long ECG_REFRAC_MS = 300;    // tiempo mínimo entre picos, evita re-bounce

unsigned long updateSensorsMS = 0;
unsigned long updateMaxSensorsMS = 0;
unsigned long showSensorsMS = 0;
unsigned long reconnectMqttMS = 0;

bool maxSensorBuzzer = false;
bool tempSensorBuzzer = false;

/*
1. BPM (Frecuencia Cardíaca)
Malo por bajo (< 50 lpm): Si la persona no es un atleta de alto rendimiento y tiene pulso bajo, puede haber mareos o desmayos.

Malo por alto (> 120 lpm): Si está en reposo y su corazón late así, puede indicar deshidratación, estrés extremo, infección o problemas cardíacos.

2. SpO2 (Saturación de Oxígeno)
92% a 94%: Se considera "preocupante" o nivel bajo (necesita monitoreo).

Menos de 90%: Es una emergencia médica. El cuerpo no está oxigenando los órganos vitales correctamente.

3. Temperatura
Fiebre (> 38°C): Indica que el cuerpo está luchando contra una infección.

Hipotermia (< 35°C): Es peligrosa porque los órganos empiezan a fallar por falta de calor.
SDA 21 SCL 22
*/ 

void loop() {
    Utils::updateBuzzer();

    // detectar latido a partir de la señal ECG y emitir beep inmediato
    if (enabledBuzzer && electrodes) {
        if (ecg > ECG_THRESHOLD && previousEcg <= ECG_THRESHOLD) {
            unsigned long now = millis();
            if (now - lastEcgPeakTime >= ECG_REFRAC_MS) {
                Utils::pulseTone(1000, 50);
                lastEcgPeakTime = now;
            }
        }
        previousEcg = ecg;
    }

    if (isTimerExpired(updateMaxSensorsMS, 20)) {
        MaxSensor::update();
        if (MaxSensor::isDataValid()) {
            bpm = MaxSensor::getBPM();
            spo2 = MaxSensor::getSpO2();
            
            Serial.printf("%d bpm, %d spo2\n", bpm, spo2);
            // si se desea calcular bpm a partir de ECG podría hacerse aquí
            // (por ahora seguimos usando el valor proveniente del MAX30102)

            if(enabledBuzzer) {
                if((bpm < 50 || (bpm > 120 && bpm <= 1000) || spo2 < 90 ) && !tempSensorBuzzer) {
                    Utils::startTone(1000, 400, true);
                    maxSensorBuzzer = true;
                } else if(maxSensorBuzzer) {
                    Utils::stopTone();
                    maxSensorBuzzer = false;
                }
            }
        }
    }

    if (isTimerExpired(updateSensorsMS, 100)) {
        temp1 = DS18B20::getTemperature();
        temp2 = GY906::getTempObject();

        if(enabledBuzzer) {
            if((temp1 > 38 || temp2 > 38) && !maxSensorBuzzer) {
                Utils::startTone(1000, 400, true);
                tempSensorBuzzer = true;
            } else if(tempSensorBuzzer) {
                Utils::stopTone();
                tempSensorBuzzer = false;
            }
        }        

        electrodes = AD8232::electrodesConnected();
        ecg = AD8232::readECG();
    }

    if (isTimerExpired(showSensorsMS, 400)) {
        Nextion::write(temp1, temp2, spo2, bpm, ecg, electrodes);
        bool mqttConnected = mqttClient.connected();
        
        if(mqttConnected) {
            MQTT::sendData(temp1, temp2, spo2, bpm, ecg, electrodes);
        }
        else if(WiFi.isConnected() && isTimerExpired(reconnectMqttMS, 10000)) {
            MQTT::connectMQTT();
        }
    }
}
#include <Arduino.h>
#include "modules/mqtt.h"
#include "modules/displayNextion.h"
#include "modules/utils.h"

#include "sensors/ad8232.h"
#include "sensors/ds18b20.h"
#include "sensors/gy906.h"
#include "sensors/max_sensor.h"

// Aun no le encuentro un lugar a esta funcion, es para verificar que un dispositivo i2c este conectado
bool isConnected(uint8_t address) {
    Wire.beginTransmission(address);
    return (Wire.endTransmission() == 0);
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

unsigned long updateSensorsMS = 0;
unsigned long updateMaxSensorsMS = 0;
unsigned long showSensorsMS = 0;
unsigned long reconnectMqttMS = 0;

void loop() {
    Utils::updateBuzzer();

    if (isTimerExpired(updateMaxSensorsMS, 100)) {
        MaxSensor::update();
        if (MaxSensor::isDataValid()) {
            bpm = MaxSensor::getBPM();
            spo2 = MaxSensor::getSpO2();
        }
    }

    if (isTimerExpired(updateSensorsMS, 200)) {
        //temp1 = NAN;
        //temp1 = DS18B20::getTemperature();
        temp2 = GY906::getTempObject();

        electrodes = AD8232::electrodesConnected();
        ecg = AD8232::readECG();
    }

    if (isTimerExpired(showSensorsMS, 800)) {
        //Nextion::write(temp1, temp2, spo2, bpm, ecg, electrodes);
        bool mqttConnected = mqttClient.connected();
        
        if(mqttConnected) {
            MQTT::sendData(temp1, temp2, spo2, bpm, ecg, electrodes);
        }
        else if(WiFi.isConnected() && isTimerExpired(reconnectMqttMS, 10000)) {
            MQTT::connectMQTT();
        }
    }
}

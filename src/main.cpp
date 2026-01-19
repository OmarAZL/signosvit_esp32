#include <Arduino.h>
#include "modules/mqtt.h"
#include "modules/display.h"
#include "modules/utils.h"

#include "sensors/ds18b20.h"
#include "sensors/gy906.h"

/*
    Debo comentar más mi codigo :b
{
    "data": {
        "temp1": <temp1>,
        "temp2": <temp2>,
        "ecg": <ecg>
    }
}

*/

// Aun no le encuentro un lugar a esta funcion, es para verificar que un dispositivo i2c este conectado
bool isConnected(uint8_t address) {
    Wire.beginTransmission(address);
    return (Wire.endTransmission() == 0);
}

void setup() {
    Serial.begin(115200);
    Display::init();
    MQTT::init();
    DS18B20::init();
    GY906::init();
    Utils::init();

    Display::showMessage("Iniciando...");
    delay(1000);
    Display::clear();
    
    Serial.println("\nEnviando mensajes.");
}

String payload;
float temp1;
float temp2;

unsigned long now = 0;
unsigned long lastReport = 0;

void loop() {
    now = millis();

    if ((millis() - lastReport) > 1500) {
        mqttClient.loop();
        lastReport = now;

        temp1 = DS18B20::getTemperature();
        temp2 = GY906::getTempObject();
        bool mqttConnected = mqttClient.connected();
        if(mqttConnected) {   
            MQTT::sendData(temp1, temp2, now);
        }
        else if(WiFi.isConnected()) {
            MQTT::connectMQTT();
        }
        
        Display::showData(temp1, temp2, now, mqttConnected);
    }
}

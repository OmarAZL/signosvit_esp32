#include <Arduino.h>
#include "modules/mqtt.h"
#include "modules/display.h"

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
    MQTT::init();
    Display::init();
    DS18B20::init();
    GY906::init();

    Display::showMessage("Pantalla iniciada");
    delay(1000);
    
    Serial.println("\nEnviando mensajes.");
}

String payload;
float temp1;
float temp2;

unsigned long now = 0;

void loop() {
    mqttClient.loop();
    now = millis();

    temp1 = DS18B20::getTemperature();
    temp2 = GY906::getTempObject();

    if(mqttClient.connected()) {   
        MQTT::sendData(temp1, temp2, now);
    }
    
    Display::clear();
    display.setTextSize(1); // Normal size
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Temperatura 1: " + String(temp1, 2));
    display.println("Temperatura 2: " + String(temp2, 2));
    display.println("Timestamp: " + String(now));
    String mqttStatus = mqttClient.connected() ? "Activado" : "Desactivado";
    display.println("MQTT: " + mqttStatus);
    Display::updateDisplay();
    
    delay(1600);
}

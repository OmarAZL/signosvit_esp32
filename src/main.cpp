#include <Arduino.h>
#include "modules/mqtt.h"
#include "modules/display.h"

// Aun no le encuentro un lugar a esta funcion, es para verificar que un dispositivo i2c este conectado
bool isConnected(uint8_t address) {
    Wire.beginTransmission(address);
    return (Wire.endTransmission() == 0);
}

void setup() {
    Serial.begin(115200);
    MQTT::init();
    if(!Display::init()) {
        while(true) {
            Serial.println("No se ha podido iniciar la pantalla.");
            delay(2000);
        }
    }
    Display::showMessage("Pantalla iniciada");
    delay(1000);
    
    Serial.println("\nEnviando mensajes.");
}

String payload;
void loop() {
    mqttClient.loop();

    payload = "";
    payload = String(millis());

    if(mqttClient.connected()) {   
        Serial.println(payload);
        mqttClient.publish("hello/world", (char*)payload.c_str(), true);
    }
    Display::showMessage(payload);
    
    delay(600);
}

#include <Arduino.h>
#include "modules/mqtt.h"

void setup() {
    Serial.begin(115200);
    MQTT::init();
    Serial.println("\nEnviando mensajes.");
}

String payload;
void loop() {
    mqttClient.loop();

    if(mqttClient.connected()) {
        payload = "";
        payload = String(millis());
        Serial.println(payload);
        mqttClient.publish("hello/world", (char*)payload.c_str(), true);
    }
    
    delay(600);
}

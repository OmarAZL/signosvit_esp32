#pragma once
#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

extern PubSubClient mqttClient;

namespace MQTT {
    bool connectMQTT();
    void init();
    void sendData(float temp1, float temp2, unsigned long timestamp);
}
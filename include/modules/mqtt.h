#pragma once
#include <WiFi.h>
#include <PubSubClient.h>

extern PubSubClient mqttClient;

namespace MQTT {
    void init();
}
#pragma once
#include <PubSubClient.h>

extern PubSubClient mqttClient;

namespace MQTT {
    void init();
}
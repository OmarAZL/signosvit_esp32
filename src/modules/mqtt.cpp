#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

namespace MQTT {

    const char* ssid = env::ssid;
    const char* passwd = env::passwd;

    const char* server = "192.168.1.112";
    const uint16_t port = 8883;
    const char* mqttUser = "";
    const char* mqttPassword = "";

    void initWifi() {
        WiFi.begin(ssid, passwd);
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            Serial.println("Conectando al WiFi");
        }
        Serial.println("Conectado al WiFi");
    }

    void init() {
        initWifi();
        String mac = WiFi.macAddress();
        mqttClient.setServer(server, port);
        while (!mqttClient.connected())
        {
            Serial.println("Connecting to MQTT...");
            if (mqttClient.connect(mac.c_str())) {
                Serial.println("connected");
            }
            else
            {   
                Serial.print("failed with state ");
                Serial.println(mqttClient.state());
                delay(2000);
            }
        }
    }   
}
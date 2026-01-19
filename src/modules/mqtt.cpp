#include "modules/mqtt.h"
#include "env.h"

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
JsonDocument jsonDoc;

// [Variables]
const char* ssid = env::ssid;
const char* passwd = env::passwd;

const char* server = "192.168.1.103";
const uint16_t port = 1883;
const char* mqttUser = "omar";
const char* mqttPassword = "omar";
String macAddress = WiFi.macAddress();

namespace MQTT { 

    String content = ""; // Esta función se puede llamar varias veces, asi que para evitar crear una variable en cada llamada, se crea desde un inicio.
    void OnMqttReceived(char* topic, byte* payload, unsigned int length) {
        Serial.print("Recibido en ");
        Serial.print(topic);
        Serial.print(": ");

        content = ""; 
        for (size_t i = 0; i < length; i++) {
            content.concat((char)payload[i]);
        }
        Serial.print(content);
        Serial.println();
    }

    void initWifi() {
        WiFi.begin(ssid, passwd);
        while (!WiFi.isConnected()) {
            delay(1000);
            Serial.println("Conectando al WiFi");
        }
        Serial.println("Conectado al WiFi");
    }

    bool connectMQTT() {
        if(mqttClient.connected()) {
            return true;
        }

        if (mqttClient.connect(macAddress.c_str(), mqttUser, mqttPassword)) {
            String topic = "esp32/" + macAddress;
            mqttClient.subscribe(topic.c_str());
            Serial.println("Conectado a MQTT");
            Serial.println("Suscrito al tópico: " + topic);
            return true;
        }
         
        Serial.print("Fallo con el estado: ");
        Serial.println(mqttClient.state());
        return false;
    }

    void init() {
        initWifi();
        mqttClient.setServer(server, port);
        mqttClient.setCallback(OnMqttReceived);
        Serial.println("Conectando a MQTT...");
        connectMQTT();
    }   

    void sendData(float temp1, float temp2, unsigned long timestamp) {
        jsonDoc.clear();
        jsonDoc["mac"] = macAddress;
        jsonDoc["timestamp"] = timestamp;
        jsonDoc["data"]["temp1"] = temp1;
        jsonDoc["data"]["temp2"] = temp2;
        char buffer[256];
        size_t n = serializeJson(jsonDoc, buffer);
        String topic = "devices";
        mqttClient.publish(topic.c_str(), buffer, n);
    }

}
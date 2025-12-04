#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <WiFi.h>
#include <ArduinoMqttClient.h>

class NetworkManager {
  private:
    char ssid[50];
    char pass[50];
    const char* broker = "broker.hivemq.com"; 
    int port = 1883;
    String baseTopic = "portenta/bike/"; 

    WiFiClient wifiClient;
    MqttClient mqttClient;

  public:
    NetworkManager(const char* wifiSsid, const char* wifiPass) : mqttClient(wifiClient) {
      strcpy(ssid, wifiSsid);
      strcpy(pass, wifiPass);
    }

    void init() {
      Serial.print("WiFi: "); Serial.println(ssid);
      WiFi.begin(ssid, pass);
      int t = 0;
      while (WiFi.status() != WL_CONNECTED && t < 20) { delay(500); Serial.print("."); t++; }
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi OK!");
        connectMqtt();
      } else { Serial.println("\nWiFi Fail"); }
    }

    void connectMqtt() {
      if (!mqttClient.connect(broker, port)) Serial.println("MQTT Error");
      else Serial.println("MQTT Connected!");
    }

    void update() { if (WiFi.status() == WL_CONNECTED) mqttClient.poll(); }

    // Funzione MEGA-PACK che manda tutto
    void sendTelemetry(int bpm, float gForce, float slope, float temp) {
      if (WiFi.status() == WL_CONNECTED) {
        if (!mqttClient.connected()) connectMqtt();

        // Topic singoli per i Gauge dell'App
        mqttClient.beginMessage(baseTopic + "bpm"); mqttClient.print(bpm); mqttClient.endMessage();
        mqttClient.beginMessage(baseTopic + "gforce"); mqttClient.print(gForce); mqttClient.endMessage();
        mqttClient.beginMessage(baseTopic + "slope"); mqttClient.print(slope); mqttClient.endMessage();
        mqttClient.beginMessage(baseTopic + "temp"); mqttClient.print(temp); mqttClient.endMessage();

        // JSON Completo per il log
        String json = "{";
        json += "\"bpm\":" + String(bpm) + ",";
        json += "\"gf\":" + String(gForce) + ",";
        json += "\"slp\":" + String(slope) + ",";
        json += "\"tmp\":" + String(temp);
        json += "}";

        mqttClient.beginMessage(baseTopic + "json");
        mqttClient.print(json);
        mqttClient.endMessage();

        Serial.print(">> JSON: "); Serial.println(json);
      }
    }
};
#endif
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
      WiFi.begin(ssid, pass);
      int t=0; while(WiFi.status()!=WL_CONNECTED && t<20){ delay(500); Serial.print("."); t++; }
      if(WiFi.status()==WL_CONNECTED) connectMqtt();
    }
    void connectMqtt() { mqttClient.connect(broker, port); }
    void update() { if(WiFi.status()==WL_CONNECTED) mqttClient.poll(); }

    void sendTelemetry(int bpm, float gf, float slope, float lean, float vib, bool crash, float temp) {
      if (WiFi.status() == WL_CONNECTED) {
        if (!mqttClient.connected()) connectMqtt();

        // Topic singoli per App dashboard veloce
        mqttClient.beginMessage(baseTopic + "lean"); mqttClient.print(lean); mqttClient.endMessage();
        mqttClient.beginMessage(baseTopic + "vib"); mqttClient.print(vib); mqttClient.endMessage();
        
        if(crash) { // Manda allarme solo se cadi
             mqttClient.beginMessage(baseTopic + "alert"); mqttClient.print("CRASH!"); mqttClient.endMessage();
        }

        // JSON COMPLETO (Per il Log/Database)
        String json = "{";
        json += "\"bpm\":" + String(bpm) + ",";
        json += "\"gf\":" + String(gf) + ",";
        json += "\"slp\":" + String(slope) + ",";
        json += "\"lean\":" + String(lean) + ",";
        json += "\"vib\":" + String(vib) + ",";
        json += "\"crash\":" + String(crash) + ",";
        json += "\"tmp\":" + String(temp);
        json += "}";

        mqttClient.beginMessage(baseTopic + "json");
        mqttClient.print(json);
        mqttClient.endMessage();
        Serial.println(">> SENT: " + json);
      }
    }
};
#endif
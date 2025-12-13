#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <WiFi.h>
#include <ArduinoMqttClient.h>


class NetworkManager {
  private:
    char ssid[50];
    char pass[50];
    const char* broker = "broker.hivemq.com"; // our cloud server
    int port = 1883; // this is our MQTT port
    String baseTopic = "portenta/bike/"; 
    WiFiClient wifiClient;
    MqttClient mqttClient;

  public:
    NetworkManager(const char* wifiSsid, const char* wifiPass) : mqttClient(wifiClient) {
      strcpy(ssid, wifiSsid);
      strcpy(pass, wifiPass);
    }

    void init() {
      Serial.print("Connecting WiFi: "); Serial.println(ssid);
      WiFi.begin(ssid, pass);
      
      //we try connecting for 10 seconds 
      int t = 0;
      while(WiFi.status() != WL_CONNECTED && t<20) { 
        delay(500); 
        Serial.print("."); 
        t++; 
      }
      
      if(WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi OK and connected!");
        connectMqtt(); //we connect to Cloud if WiFi works
      } 
      else { 
        Serial.println("\nWiFi Failed."); 
      }
    }

    void connectMqtt() {
      //we connect to HiveMQ broker
      if (!mqttClient.connect(broker, port)) {
        Serial.println("MQTT Error");
      }
      else {
        Serial.println("MQTT Connected!");
      }    
    }

    // Keep the connection alive
    void update() { 
      if(WiFi.status()==WL_CONNECTED) {
        mqttClient.poll(); 
      } 
    }
 
    //THE MAIN SENDING FUNCTION: it takes ALL metrics and sends them to the cloud
    void sendTelemetry(int bpm, float gf, float slope, float lean, float vib, bool crash, 
                       float temp, double lat, double lon, double alt, double spd, double odo, double avg) {
      
      if (WiFi.status() == WL_CONNECTED) {
        //tries to reconnect if connection dropped
        if (!mqttClient.connected()) {
          connectMqtt();
        }
          
        //it sends SINGLE TOPICS (for real time Dashboard Gauges)
        mqttClient.beginMessage(baseTopic + "bpm"); mqttClient.print(bpm); mqttClient.endMessage();
        mqttClient.beginMessage(baseTopic + "gf");  mqttClient.print(gf);  mqttClient.endMessage();
        mqttClient.beginMessage(baseTopic + "slp"); mqttClient.print(slope); mqttClient.endMessage();
        mqttClient.beginMessage(baseTopic + "lean"); mqttClient.print(lean); mqttClient.endMessage();
        mqttClient.beginMessage(baseTopic + "crash"); mqttClient.print(crash); mqttClient.endMessage();
        mqttClient.beginMessage(baseTopic + "vib"); mqttClient.print(vib); mqttClient.endMessage();
        mqttClient.beginMessage(baseTopic + "temp"); mqttClient.print(temp); mqttClient.endMessage();
        mqttClient.beginMessage(baseTopic + "lat"); mqttClient.print(lat, 6); mqttClient.endMessage();
        mqttClient.beginMessage(baseTopic + "lon"); mqttClient.print(lon, 6); mqttClient.endMessage();
        mqttClient.beginMessage(baseTopic + "alt"); mqttClient.print(alt); mqttClient.endMessage();
        mqttClient.beginMessage(baseTopic + "spd"); mqttClient.print(spd); mqttClient.endMessage();
        mqttClient.beginMessage(baseTopic + "odo"); mqttClient.print(odo); mqttClient.endMessage();
        mqttClient.beginMessage(baseTopic + "avg"); mqttClient.print(avg); mqttClient.endMessage();

        // We send a FULL JSON for Data Logging. you can also think about it as our database
        // We build a single text string with all data
        String json = "{";
        json += "\"bpm\":" + String(bpm) + ",";
        json += "\"gf\":" + String(gf) + ",";
        json += "\"slp\":" + String(slope) + ",";
        json += "\"lean\":" + String(lean) + ",";
        json += "\"vib\":" + String(vib) + ",";
        json += "\"crash\":" + String(crash) + ",";
        json += "\"tmp\":" + String(temp) + ",";
        json += "\"lat\":" + String(lat, 6) + ",";
        json += "\"lon\":" + String(lon, 6) + ",";
        json += "\"alt\":" + String(alt) + ",";
        json += "\"spd\":" + String(spd) + ",";
        json += "\"odo\":" + String(odo) + ",";
        json += "\"avg\":" + String(avg);
        json += "}";

        // Send JSON on separate topic
        mqttClient.beginMessage(baseTopic + "json");
        mqttClient.print(json);
        mqttClient.endMessage();
        
        Serial.println(">> SENT: " + json);
      }
    }
};
#endif
#include "BleManager.h"
#include "ImuManager.h"
#include "DisplayManager.h"
#include "NetworkManager.h"

// --- I TUOI DATI ---
const char* WIFI_SSID = "AndroidAP4055"; // Esempio
const char* WIFI_PASS = "cerve110Ho!1";

// Istanziamo TUTTI i manager
BleManager ble;
ImuManager imu;
DisplayManager display;
NetworkManager net(WIFI_SSID, WIFI_PASS);

unsigned long lastSend = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  display.init();
  imu.init();
  net.init();
  ble.init(); // Ora c'è anche lui!
}

void loop() {
  // 1. Aggiorna Connessioni e Sensori
  net.update();
  ble.update();
  imu.update();

  // 2. Invio e Grafica (5Hz)
  if (millis() - lastSend > 200) {
    lastSend = millis();
    
    // Raccogli tutti i dati
    int bpm = ble.getBpm();
    float gf = imu.getGForce();
    float sl = imu.getSlope();
    float tm = imu.getTemp();

    // Aggiorna Display Fisico
    display.drawTelemetry(bpm, gf, sl, tm, (WiFi.status() == WL_CONNECTED), ble.isConnected());

    // Manda all'App
    net.sendTelemetry(bpm, gf, sl, tm);
  }
}
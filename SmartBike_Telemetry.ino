#include "BleManager.h"
#include "ImuManager.h"
#include "DisplayManager.h"
#include "NetworkManager.h"

// --- DATI WIFI ---
const char* WIFI_SSID = "bici"; // Togli gli spazi dall'hotspot!
const char* WIFI_PASS = "password";

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
  ble.init();
}

void loop() {
  net.update();
  ble.update();
  imu.update();

  if (millis() - lastSend > 200) {
    lastSend = millis();
    
    // Raccogli i dati
    int bpm = ble.getBpm();
    float gf = imu.getGForce();
    float sl = imu.getSlope();
    float ln = imu.getLean(); // Piega
    float vb = imu.getVibration(); // Vibrazione
    bool cr = imu.isCrash();
    float tm = imu.getTemp();

    display.drawTelemetry(bpm, sl, ln, vb, cr);
    net.sendTelemetry(bpm, gf, sl, ln, vb, cr, tm);
  }
}
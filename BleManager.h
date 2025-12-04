#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <ArduinoBLE.h>

class BleManager {
  private:
    BLEDevice peripheral;
    BLECharacteristic hrChar;
    bool connected = false;
    int currentBpm = 0;
    const char* serviceUUID = "180d";
    const char* charUUID = "2a37";

  public:
    void init() {
      if (!BLE.begin()) { Serial.println("BLE Fail"); return; }
      Serial.println("BLE Scanning...");
      BLE.scanForUuid(serviceUUID);
    }

    void update() {
      if (connected) {
        if (!peripheral.connected()) {
          connected = false;
          currentBpm = 0;
          Serial.println("BLE Lost -> Scan");
          BLE.scanForUuid(serviceUUID);
          return;
        }
        if (hrChar.valueUpdated()) {
          const uint8_t* data = hrChar.value();
          currentBpm = data[1]; 
        }
      } else {
        BLEDevice device = BLE.available();
        if (device) {
          Serial.print("BLE Found: "); Serial.println(device.localName());
          if (device.connect()) {
            BLE.stopScan();
            peripheral = device;
            if (peripheral.discoverAttributes()) {
              hrChar = peripheral.characteristic(charUUID);
              if (hrChar) {
                hrChar.subscribe();
                connected = true;
                Serial.println("BLE Linked!");
              }
            } else { peripheral.disconnect(); }
          }
        }
      }
    }
    int getBpm() { return currentBpm; }
    bool isConnected() { return connected; }
};
#endif
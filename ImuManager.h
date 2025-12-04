#ifndef IMU_MANAGER_H
#define IMU_MANAGER_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

class ImuManager {
  private:
    Adafruit_MPU6050 mpu;
    float gForce = 0.0;
    float slope = 0.0;
    float leanAngle = 0.0; // NUOVO: Piega
    float vibration = 0.0; // NUOVO: Ruvidità asfalto
    bool crashDetected = false; // NUOVO: Allarme
    float tempC = 0.0;

  public:
    void init() {
      if (!mpu.begin()) { Serial.println("IMU Error"); return; }
      mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
      mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    }

    void update() {
      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);

      // 1. G-FORCE TOTALE
      float rawG = sqrt(sq(a.acceleration.x) + sq(a.acceleration.y) + sq(a.acceleration.z));
      gForce = rawG / 9.81;

      // 2. SLOPE (Salita/Discesa - Asse Y)
      slope = atan2(a.acceleration.y, a.acceleration.z) * 57.29; 

      // 3. LEAN ANGLE (Piega Laterale - Asse X) [COMPUTED METRIC]
      leanAngle = atan2(a.acceleration.x, a.acceleration.z) * 57.29;

      // 4. VIBRATION INDEX [COMPUTED METRIC]
      // Calcoliamo quanto l'asse Z si discosta da 1G (gravità fissa).
      // Valori alti = Sterrato/Pavè. Valori bassi = Asfalto liscio.
      float zG = abs(a.acceleration.z / 9.81);
      vibration = abs(zG - 1.0) * 100; // Moltiplico x100 per avere un numero leggibile (0-50)

      // 5. CRASH DETECTION
      if (gForce > 3.5) crashDetected = true; // Botta forte
      else crashDetected = false;

      tempC = temp.temperature;
    }

    float getGForce() { return gForce; }
    float getSlope() { return slope; }
    float getLean() { return leanAngle; }
    float getVibration() { return vibration; }
    bool isCrash() { return crashDetected; }
    float getTemp() { return tempC; }
};
#endif
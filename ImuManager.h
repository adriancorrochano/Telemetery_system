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
    float tempC = 0.0; // Nuovo dato!

  public:
    void init() {
      if (!mpu.begin()) {
        Serial.println("IMU ERROR");
      } else {
        Serial.println("IMU OK");
        mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
        mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
      }
    }

    void update() {
      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp); // Legge anche la temperatura ora

      float rawG = sqrt(sq(a.acceleration.x) + sq(a.acceleration.y) + sq(a.acceleration.z));
      gForce = rawG / 9.81;

      // Slope in gradi
      slope = atan2(a.acceleration.y, a.acceleration.z) * 57.29; 

      // Temperatura
      tempC = temp.temperature;
    }

    float getGForce() { return gForce; }
    float getSlope() { return slope; }
    float getTemp() { return tempC; } // Getter per la temperatura
};
#endif
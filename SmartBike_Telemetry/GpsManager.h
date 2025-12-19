#ifndef GPS_MANAGER_H
#define GPS_MANAGER_H

#include <TinyGPS++.h>

class GpsManager {
  private:
    TinyGPSPlus gps; // The library that parses GPS text (NMEA)
    double totalDistance = 0.0; //our total distnace in km
    double lastLat = 0.0, lastLon = 0.0; //last position
    unsigned long startTime = 0; //exact time when we started moving
    bool hasFix = false; //a boolean that we set to true if we have satellite signal
    bool firstPoint = false; // Fllag for the very first coordinate

  public:
    void init() {
      // Start serial communication with GPS module
      //we use the portenta Serial1 specifically Pin 13/14
      Serial1.begin(9600); 
    }

    void update() {
      //we read all the data coming from the GPS wire
      while (Serial1.available() > 0) {
        gps.encode(Serial1.read());
      }

      // If we have a valid location signal...
      if (gps.location.isValid()) {
        if (!hasFix) {  
          hasFix = true;
          startTime = millis(); // Start the timer for avg speed
        }

        if (!firstPoint) {
          //we save the starting point
          lastLat = gps.location.lat();
          lastLon = gps.location.lng();
          firstPoint = true;//we set it to true
        } else {
          // CALCULATE DISTANCE (Odometer)
          // Calculate meters between current point and previous point, for this we use the library of the gps and the methods it provides
          double dist = gps.distanceBetween(gps.location.lat(), gps.location.lng(), lastLat, lastLon);
          
          //to filter noise we thought of only adding distance if we moved more than 2 meters
          if (dist > 2.0) {
            totalDistance += dist;
            lastLat = gps.location.lat();
            lastLon = gps.location.lng();
          }
        }
      }
    }

    void init() {
      Serial.begin(115200); // Comunicazione col PC veloce
      
      // Prova 9600 (Standard Waveshare)
      Serial1.begin(9600); 
      
      // Se dopo vedi caratteri strani, prova a cambiare 9600 con 115200 o 38400 qui sotto
      // Serial1.begin(115200); 
    }

    void loop() {
      if (Serial1.available()) {
        char c = Serial1.read();
        Serial.write(c); // Stampa ESATTAMENTE quello che riceve
      }
    }

    // Standard Getters for raw GPS data
    bool isConnected() { return gps.location.isValid(); }
    double getLat() { return gps.location.lat(); }
    double getLon() { return gps.location.lng(); }
    double getAlt() { return gps.altitude.meters(); }
    double getSpeed() { return gps.speed.kmph(); }
    int getSats() { return gps.satellites.value(); }
    
    //getter for the distance
    double getOdometer() { return totalDistance / 1000.0; } 
    
    //we calculate the average Speed (km/h)
    double getAvgSpeed() {
      if (totalDistance < 10 || !hasFix) 
        return 0.0;
      // Convert millis to hours
      double hours = (millis() - startTime) / 3600000.0;
      if (hours <= 0) 
        return 0.0;
      // Distance / Time
      return (totalDistance / 1000.0) / hours; 
    }

    // Helper to format time as HH:MM:SS
    String getTimeStr() {
      if (!gps.time.isValid()) return "--:--:--";
      char sz[10];
      sprintf(sz, "%02d:%02d:%02d", gps.time.hour(), gps.time.minute(), gps.time.second());
      return String(sz);
    }
};
#endif
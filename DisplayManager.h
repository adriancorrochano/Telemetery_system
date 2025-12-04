#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H
#include <U8g2lib.h>

class DisplayManager {
  private:
    U8G2_SSD1327_WS_128X128_F_4W_SW_SPI u8g2;
  public:
    DisplayManager() : u8g2(U8G2_R0, 9, 8, 6, 7, 5) {}
    void init() { u8g2.begin(); u8g2.enableUTF8Print(); u8g2.setContrast(255); }

    void drawTelemetry(int bpm, float slope, float lean, float vib, bool crash) {
      u8g2.clearBuffer();

      if (crash) {
        // SCHERMATA DI EMERGENZA
        u8g2.setFont(u8g2_font_ncenB18_tr);
        u8g2.drawStr(10, 60, "CRASH!");
        u8g2.drawStr(10, 90, "DETECTED");
      } else {
        // DASHBOARD NORMALE
        u8g2.setFont(u8g2_font_helvB08_tr);
        u8g2.drawStr(2, 10, "SMART BIKE");
        u8g2.drawHLine(0, 12, 128);

        // BPM
        u8g2.setFont(u8g2_font_ncenB18_tr);
        u8g2.setCursor(5, 45); (bpm>0)? u8g2.print(bpm) : u8g2.print("--");
        u8g2.setFont(u8g2_font_helvB08_tr); u8g2.print(" bpm");

        // PIEGA (LEAN)
        u8g2.setCursor(5, 70); u8g2.print("Lean: "); u8g2.print(lean, 0); u8g2.print(" deg");

        // VIBRAZIONE
        u8g2.setCursor(5, 90); u8g2.print("Vib: "); u8g2.print(vib, 0);

        // SLOPE
        u8g2.setCursor(5, 110); u8g2.print("Slope: "); u8g2.print(slope, 0); u8g2.print("%");
      }
      u8g2.drawFrame(0, 0, 128, 128);
      u8g2.sendBuffer();
    }
};
#endif
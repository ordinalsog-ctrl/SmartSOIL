#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"
#include "sensors.h"

class Display {
public:
    Adafruit_SSD1306 oled;
    bool active = true;
    unsigned long lastActivity = 0;

    Display() : oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1) {}

    bool begin() {
        if (!oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
            return false;
        }
        lastActivity = millis();
        showSplash();
        return true;
    }

    void showSplash() {
        oled.clearDisplay();
        oled.setTextColor(SSD1306_WHITE);
        oled.setTextSize(2);
        oled.setCursor(4, 8);
        oled.println("SmartSoil");
        oled.setTextSize(1);
        oled.setCursor(28, 36);
        oled.println("Prototyp v1.0");
        oled.setCursor(20, 52);
        oled.println("Initialisiere...");
        oled.display();
    }

    void showMain(const SensorData& data) {
        if (!active) return;
        oled.clearDisplay();
        oled.setTextColor(SSD1306_WHITE);

        // Header: Titel + Batterie
        oled.setTextSize(1);
        oled.setCursor(0, 0);
        oled.print("SmartSoil");
        drawBattery(100, 0, data.batteryPercent);

        // Trennlinie
        oled.drawLine(0, 10, 127, 10, SSD1306_WHITE);

        // Feuchte mit Balken
        oled.setCursor(0, 14);
        oled.print("Feuchte:");
        oled.setCursor(80, 14);
        char buf[8];
        snprintf(buf, sizeof(buf), "%4.1f%%", data.moisturePercent);
        oled.print(buf);
        drawProgressBar(0, 24, 128, 6, data.moisturePercent);

        // pH-Wert mit Balken
        oled.setCursor(0, 34);
        oled.print("pH-Wert:");
        oled.setCursor(80, 34);
        snprintf(buf, sizeof(buf), "%4.1f", data.phValue);
        oled.print(buf);
        drawPHBar(0, 44, 128, 6, data.phValue);

        // Status-Zeile
        oled.setCursor(0, 55);
        oled.setTextSize(1);
        oled.print(getWaterIcon(data.moisturePercent));
        oled.print(" ");
        oled.print(getStatus(data));

        oled.display();
    }

    void showCalibrationPH(int step, float voltage) {
        oled.clearDisplay();
        oled.setTextColor(SSD1306_WHITE);
        oled.setTextSize(1);

        oled.setCursor(0, 0);
        oled.println("== pH KALIBRIERUNG ==");
        oled.println();

        if (step == 0) {
            oled.println("Sonde in pH 7.0");
            oled.println("Pufferloesung tauchen");
        } else {
            oled.println("Sonde in pH 4.0");
            oled.println("Pufferloesung tauchen");
        }

        oled.println();
        oled.print("Spannung: ");
        oled.print(voltage, 3);
        oled.println("V");
        oled.println();
        oled.println("Kurz druecken = OK");
        oled.println("Lang = Abbrechen");

        oled.display();
    }

    void showCalibrationMoisture(int step, int raw) {
        oled.clearDisplay();
        oled.setTextColor(SSD1306_WHITE);
        oled.setTextSize(1);

        oled.setCursor(0, 0);
        oled.println("= FEUCHTE KALIBR. =");
        oled.println();

        if (step == 0) {
            oled.println("Sensor in TROCKENE");
            oled.println("Erde stecken");
        } else {
            oled.println("Sensor in NASSE");
            oled.println("Erde stecken");
        }

        oled.println();
        oled.print("ADC-Wert: ");
        oled.println(raw);
        oled.println();
        oled.println("Kurz druecken = OK");
        oled.println("Lang = Abbrechen");

        oled.display();
    }

    void showMessage(const char* line1, const char* line2 = nullptr) {
        oled.clearDisplay();
        oled.setTextColor(SSD1306_WHITE);
        oled.setTextSize(1);
        oled.setCursor(0, 20);
        oled.println(line1);
        if (line2) {
            oled.setCursor(0, 36);
            oled.println(line2);
        }
        oled.display();
    }

    void sleep() {
        oled.clearDisplay();
        oled.display();
        active = false;
    }

    void wake() {
        active = true;
        lastActivity = millis();
    }

    bool shouldSleep() {
        return active && (millis() - lastActivity > DISPLAY_TIMEOUT_MS);
    }

private:
    void drawProgressBar(int x, int y, int w, int h, float percent) {
        oled.drawRect(x, y, w, h, SSD1306_WHITE);
        int fill = (int)(percent / 100.0 * (w - 2));
        if (fill > 0) {
            oled.fillRect(x + 1, y + 1, fill, h - 2, SSD1306_WHITE);
        }
    }

    void drawPHBar(int x, int y, int w, int h, float ph) {
        // pH 0-14 Balken, Markierung bei 6-7 (optimal)
        oled.drawRect(x, y, w, h, SSD1306_WHITE);

        // Optimaler Bereich markieren
        int optStart = (int)(PH_OPTIMAL_LOW / 14.0 * (w - 2));
        int optEnd = (int)(PH_OPTIMAL_HIGH / 14.0 * (w - 2));
        oled.drawRect(x + 1 + optStart, y, optEnd - optStart, h, SSD1306_WHITE);

        // Aktueller Wert als gefülltes Dreieck
        int pos = (int)(ph / 14.0 * (w - 2));
        pos = constrain(pos, 0, w - 3);
        oled.fillTriangle(x + 1 + pos - 2, y + h + 2,
                          x + 1 + pos + 2, y + h + 2,
                          x + 1 + pos, y + h, SSD1306_WHITE);
    }

    void drawBattery(int x, int y, int percent) {
        // Batterie-Icon 24x8
        oled.drawRect(x, y, 22, 8, SSD1306_WHITE);
        oled.fillRect(x + 22, y + 2, 3, 4, SSD1306_WHITE);
        int bars = map(percent, 0, 100, 0, 4);
        for (int i = 0; i < bars; i++) {
            oled.fillRect(x + 2 + i * 5, y + 2, 4, 4, SSD1306_WHITE);
        }
    }

    const char* getWaterIcon(float moisture) {
        if (moisture < MOISTURE_THRESHOLD_LOW) return "[!]";
        if (moisture > MOISTURE_THRESHOLD_HIGH) return "[~]";
        return "[OK]";
    }

    const char* getStatus(const SensorData& data) {
        if (data.batteryPercent < 10) return "Akku fast leer!";
        if (data.moisturePercent < MOISTURE_THRESHOLD_LOW) {
            if (data.phValue < PH_WARN_LOW) return "Giessen + kalken!";
            return "Giessen!";
        }
        if (data.moisturePercent > MOISTURE_THRESHOLD_HIGH) return "Zu nass!";
        if (data.phValue < PH_WARN_LOW) return "pH zu sauer-kalken";
        if (data.phValue > PH_WARN_HIGH) return "pH zu basisch";
        return "Alles optimal!";
    }
};

#endif

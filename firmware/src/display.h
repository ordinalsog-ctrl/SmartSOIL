/**
 * SmartSoil v2 - Display-Modul
 * OLED 0.96" SSD1306 - Zeigt alle Pflanzen im Wechsel
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"
#include "sensors.h"

class DisplayManager {
public:
    Adafruit_SSD1306 oled;
    bool active = true;
    unsigned long lastActivity = 0;
    int currentPlant = 0;
    unsigned long lastCycle = 0;

    DisplayManager() : oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1) {}

    bool begin() {
        Wire.begin(PIN_SDA, PIN_SCL);
        if (!oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) return false;
        lastActivity = millis();
        showSplash();
        return true;
    }

    // Haupt-Update: wird im Loop aufgerufen
    void update(SensorManager& sensors, const char* plantNames[]) {
        if (!active) return;

        // Bei mehreren Sensoren: automatisch durchschalten
        if (NUM_SENSORS > 1 && millis() - lastCycle > DISPLAY_CYCLE_MS) {
            lastCycle = millis();
            currentPlant = (currentPlant + 1) % NUM_SENSORS;
        }

        showPlant(sensors.plants[currentPlant], plantNames[currentPlant], sensors);
    }

    void nextPlant() {
        currentPlant = (currentPlant + 1) % NUM_SENSORS;
        lastCycle = millis();
        wake();
    }

    void showSplash() {
        oled.clearDisplay();
        oled.setTextColor(SSD1306_WHITE);
        oled.setTextSize(2);
        oled.setCursor(4, 10);
        oled.print("SmartSoil");
        oled.setTextSize(1);
        oled.setCursor(20, 38);
        oled.print("v2.0  Multi-Sensor");
        oled.setCursor(22, 52);
        oled.print("Starte...");
        oled.display();
    }

    void showCalibration(int sensorIdx, int step, int rawValue, const char* plantName) {
        oled.clearDisplay();
        oled.setTextColor(SSD1306_WHITE);
        oled.setTextSize(1);

        oled.setCursor(0, 0);
        oled.print("KALIBRIERUNG");
        oled.setCursor(70, 0);
        oled.print(plantName);

        oled.drawLine(0, 10, 127, 10, SSD1306_WHITE);

        oled.setCursor(0, 14);
        if (step == 0) {
            oled.println("Sensor in TROCKENE");
            oled.println("Erde/Luft halten");
        } else {
            oled.println("Sensor in WASSER");
            oled.println("tauchen (bis Linie)");
        }

        oled.println();
        oled.print("ADC: ");
        oled.println(rawValue);
        oled.println();
        oled.println("Kurz=OK  Lang=Abbruch");
        oled.display();
    }

    void showMessage(const char* line1, const char* line2 = nullptr) {
        oled.clearDisplay();
        oled.setTextColor(SSD1306_WHITE);
        oled.setTextSize(1);
        oled.setCursor(0, 22);
        oled.println(line1);
        if (line2) {
            oled.setCursor(0, 38);
            oled.println(line2);
        }
        oled.display();
    }

    void wake() {
        active = true;
        lastActivity = millis();
    }

    void sleep() {
        oled.clearDisplay();
        oled.display();
        active = false;
    }

    bool shouldSleep() {
        return active && (millis() - lastActivity > DISPLAY_TIMEOUT_MS);
    }

private:
    void showPlant(const PlantReading& p, const char* name, SensorManager& sensors) {
        oled.clearDisplay();
        oled.setTextColor(SSD1306_WHITE);

        // ── Header ──────────────────────────────────────────
        oled.setTextSize(1);
        oled.setCursor(0, 0);

        // Pflanzenname (max 14 Zeichen)
        char nameBuf[15];
        strncpy(nameBuf, name, 14);
        nameBuf[14] = '\0';
        oled.print(nameBuf);

        // Sensor-Index und Paginierung rechts (z.B. "2/4")
        if (NUM_SENSORS > 1) {
            char pageBuf[6];
            snprintf(pageBuf, sizeof(pageBuf), "%d/%d", p.sensorIndex + 1, NUM_SENSORS);
            oled.setCursor(128 - strlen(pageBuf) * 6, 0);
            oled.print(pageBuf);
        }

        oled.drawLine(0, 10, 127, 10, SSD1306_WHITE);

        if (!p.valid) {
            oled.setCursor(10, 28);
            oled.println("Kein Sensor?");
            oled.setCursor(10, 42);
            oled.println("Verbindung pruefen");
            oled.display();
            return;
        }

        // ── Großer Prozentwert ───────────────────────────────
        oled.setTextSize(3);
        char pctBuf[6];
        snprintf(pctBuf, sizeof(pctBuf), "%3d%%", (int)p.moisturePercent);
        oled.setCursor(0, 14);
        oled.print(pctBuf);

        // Trend-Symbol rechts neben dem Prozentwert
        oled.setTextSize(1);
        oled.setCursor(96, 20);
        oled.print(sensors.trendSymbol(p.trend));

        // ── Fortschrittsbalken ───────────────────────────────
        drawMoistureBar(0, 42, 128, 8, p.moisturePercent, p.status);

        // ── Status-Zeile ─────────────────────────────────────
        oled.setTextSize(1);
        oled.setCursor(0, 54);
        oled.print(sensors.statusText(p.status));

        // Warndreieck bei kritisch
        if (p.status == STATUS_CRITICAL_DRY) {
            oled.setCursor(100, 54);
            oled.print("[!!]");
        }

        oled.display();
    }

    void drawMoistureBar(int x, int y, int w, int h, float pct, MoistureStatus status) {
        oled.drawRect(x, y, w, h, SSD1306_WHITE);

        // Optimalen Bereich markieren (leichte Markierung)
        int okStart = (int)(THRESH_DRY / 100.0f * (w - 2));
        int okEnd   = (int)(THRESH_OK_HIGH / 100.0f * (w - 2));
        oled.drawLine(x + 1 + okStart, y, x + 1 + okStart, y + h - 1, SSD1306_WHITE);
        oled.drawLine(x + 1 + okEnd,   y, x + 1 + okEnd,   y + h - 1, SSD1306_WHITE);

        // Füllstand
        int fill = (int)(pct / 100.0f * (w - 2));
        if (fill > 0) {
            // Blinken bei kritisch trocken
            if (status == STATUS_CRITICAL_DRY && (millis() / 500) % 2 == 0) return;
            oled.fillRect(x + 1, y + 1, fill, h - 2, SSD1306_WHITE);
        }
    }
};

#endif // DISPLAY_H

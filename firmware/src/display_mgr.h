/**
 * SmartSoil – OLED Display Manager
 * SSD1306 128x64
 */

#ifndef DISPLAY_MGR_H
#define DISPLAY_MGR_H

#include <Adafruit_SSD1306.h>
#include "plant.h"
#include "config.h"

// ── Messwert-Anzeige (Modus 1) ────────────────────────────────
void showReadingScreen(Adafruit_SSD1306& d, const char* name, float moisture,
                       const PlantPersonality& p, int level, int streak) {
    d.clearDisplay();
    d.setTextColor(SSD1306_WHITE);

    // Zeile 1: Pflanzenname + Level
    d.setTextSize(1);
    d.setCursor(0, 0);
    d.print(name);
    char lvlBuf[8];
    snprintf(lvlBuf, sizeof(lvlBuf), "Lv.%d", level);
    d.setCursor(128 - strlen(lvlBuf) * 6, 0);
    d.print(lvlBuf);

    // Trennlinie
    d.drawLine(0, 10, 128, 10, SSD1306_WHITE);

    // Zeile 2: Gesicht + Prozentzahl
    d.setTextSize(1);
    d.setCursor(0, 14);
    d.print(p.face);

    d.setTextSize(2);
    char pctBuf[8];
    snprintf(pctBuf, sizeof(pctBuf), "%.0f%%", moisture);
    int pctX = 128 - strlen(pctBuf) * 12;
    d.setCursor(pctX, 13);
    d.print(pctBuf);

    // Feuchtigkeitsbalken
    d.drawRect(0, 34, 128, 10, SSD1306_WHITE);
    int barW = (int)constrain(moisture * 124.0f / 100.0f, 0, 124);
    if (barW > 0) d.fillRect(2, 36, barW, 6, SSD1306_WHITE);

    // Zeile 4: Mood + Streak
    d.setTextSize(1);
    d.setCursor(0, 47);
    d.print(p.mood);
    if (streak > 0) {
        char stBuf[12];
        snprintf(stBuf, sizeof(stBuf), "%dd ok", streak);
        d.setCursor(128 - strlen(stBuf) * 6, 47);
        d.print(stBuf);
    }

    // Zeile 5: Kurznachricht
    d.setCursor(0, 57);
    d.print(p.message);

    d.display();
}

// ── Gieß-Erkennungs-Anzeige ───────────────────────────────────
void showWateringScreen(Adafruit_SSD1306& d, float from, float to) {
    d.clearDisplay();
    d.setTextColor(SSD1306_WHITE);

    d.setTextSize(2);
    d.setCursor(10, 4);
    d.print("Gegossen!");

    d.drawLine(0, 24, 128, 24, SSD1306_WHITE);

    d.setTextSize(1);
    d.setCursor(0, 30);
    char buf[24];
    snprintf(buf, sizeof(buf), "%.0f%% -> %.0f%%", from, to);
    d.print(buf);

    d.setCursor(0, 44);
    d.print("Gut gemacht! :)");

    d.setCursor(0, 56);
    char tsBuf[24];
    snprintf(tsBuf, sizeof(tsBuf), "Gespeichert.");
    d.print(tsBuf);

    d.display();
}

// ── WiFi verbindet ────────────────────────────────────────────
void showConnectingScreen(Adafruit_SSD1306& d) {
    d.clearDisplay();
    d.setTextColor(SSD1306_WHITE);
    d.setTextSize(1);
    d.setCursor(0, 10);
    d.print("WiFi verbindet...");
    d.setCursor(0, 25);
    d.print(WIFI_SSID);
    d.setCursor(0, 45);
    d.print("Bitte warten");
    d.display();
}

// ── WiFi aktiv ────────────────────────────────────────────────
void showWiFiActiveScreen(Adafruit_SSD1306& d, const String& ip, float moisture) {
    d.clearDisplay();
    d.setTextColor(SSD1306_WHITE);

    d.setTextSize(1);
    d.setCursor(0, 0);
    d.print("SmartSoil Online");
    d.drawLine(0, 10, 128, 10, SSD1306_WHITE);

    d.setCursor(0, 14);
    d.print("Browser offnen:");

    d.setTextSize(1);
    d.setCursor(0, 26);
    d.print("http://");
    d.print(ip);

    d.drawLine(0, 38, 128, 38, SSD1306_WHITE);

    d.setCursor(0, 42);
    char buf[20];
    snprintf(buf, sizeof(buf), "Aktuell: %.0f%%", moisture);
    d.print(buf);

    d.setCursor(0, 54);
    d.print("Knopf = schlafen");

    d.display();
}

// ── WiFi Fehler ───────────────────────────────────────────────
void showWiFiErrorScreen(Adafruit_SSD1306& d) {
    d.clearDisplay();
    d.setTextColor(SSD1306_WHITE);
    d.setTextSize(1);
    d.setCursor(0, 15);
    d.print("WiFi fehlgeschlagen");
    d.setCursor(0, 30);
    d.print("SSID: " WIFI_SSID);
    d.setCursor(0, 50);
    d.print("Schlafe weiter...");
    d.display();
}

#endif // DISPLAY_MGR_H

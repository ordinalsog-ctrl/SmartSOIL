#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "sensors.h"
#include "display.h"
#include "webserver.h"

// Module
Sensors sensors;
Display ui;
SmartSoilWeb web;

// Zustand
enum Mode { MODE_MAIN, MODE_CAL_PH, MODE_CAL_MOISTURE };
Mode currentMode = MODE_MAIN;
int calStep = 0;

SensorData currentData;
unsigned long lastMeasure = 0;

// Taster-Handling
bool buttonPressed = false;
unsigned long buttonDownTime = 0;

// --- Taster ---

void onShortPress() {
    ui.wake();
    ui.lastActivity = millis();

    if (currentMode == MODE_CAL_PH) {
        float v = sensors.readPHVoltage();
        if (calStep == 0) {
            sensors.calibratePH7(v);
            Serial.printf("pH 7.0 kalibriert: %.3fV\n", v);
            calStep = 1;
        } else {
            sensors.calibratePH4(v);
            Serial.printf("pH 4.0 kalibriert: %.3fV\n", v);
            sensors.saveCalibration();
            ui.showMessage("pH Kalibrierung", "gespeichert!");
            delay(2000);
            currentMode = MODE_CAL_MOISTURE;
            calStep = 0;
        }
    } else if (currentMode == MODE_CAL_MOISTURE) {
        int raw = sensors.readMoistureRaw();
        if (calStep == 0) {
            sensors.calibrateMoistureDry(raw);
            Serial.printf("Trocken kalibriert: %d\n", raw);
            calStep = 1;
        } else {
            sensors.calibrateMoistureWet(raw);
            Serial.printf("Nass kalibriert: %d\n", raw);
            sensors.saveCalibration();
            ui.showMessage("Feuchte Kalibrierung", "gespeichert!");
            delay(2000);
            currentMode = MODE_MAIN;
            calStep = 0;
        }
    }
}

void onLongPress() {
    if (currentMode == MODE_MAIN) {
        // Kalibrierung starten
        currentMode = MODE_CAL_PH;
        calStep = 0;
        Serial.println("Kalibrierungsmodus gestartet");
    } else {
        // Kalibrierung abbrechen
        currentMode = MODE_MAIN;
        calStep = 0;
        ui.showMessage("Kalibrierung", "abgebrochen");
        delay(1500);
    }
}

void handleButton() {
    bool pressed = (digitalRead(PIN_BUTTON) == LOW);

    if (pressed && !buttonPressed) {
        buttonPressed = true;
        buttonDownTime = millis();
    }

    if (!pressed && buttonPressed) {
        buttonPressed = false;
        unsigned long duration = millis() - buttonDownTime;

        if (duration > DEBOUNCE_MS && duration < LONG_PRESS_MS) {
            onShortPress();
        } else if (duration >= LONG_PRESS_MS) {
            onLongPress();
        }
    }

    // LED blinken bei langem Druck als Feedback
    if (buttonPressed && (millis() - buttonDownTime > LONG_PRESS_MS)) {
        digitalWrite(PIN_LED, (millis() / 200) % 2);
    }
}

// --- Setup ---

void setup() {
    Serial.begin(115200);
    Serial.println();
    Serial.println("=========================");
    Serial.println("  SmartSoil Prototyp v1  ");
    Serial.println("=========================");

    pinMode(PIN_BUTTON, INPUT_PULLUP);
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    // Sensoren initialisieren
    sensors.begin();
    Serial.println("[OK] Sensoren");

    // Display starten
    Wire.begin(PIN_SDA, PIN_SCL);
    if (!ui.begin()) {
        Serial.println("[FEHLER] OLED nicht gefunden!");
        while (true) {
            digitalWrite(PIN_LED, !digitalRead(PIN_LED));
            delay(500);
        }
    }
    Serial.println("[OK] Display");

    // WiFi Access Point + Webserver
    web.begin();
    Serial.println("[OK] WiFi AP + Webserver");

    Serial.println();
    Serial.printf("SSID: %s / Passwort: %s\n", WIFI_AP_SSID, WIFI_AP_PASS);
    Serial.printf("Webinterface: http://%s\n", WiFi.softAPIP().toString().c_str());
    Serial.println();
    Serial.println("Taster: kurz = Aktion | lang (3s) = Kalibrierung");
    Serial.println("-------------------------------------------------");

    delay(2000);
}

// --- Loop ---

void loop() {
    handleButton();

    unsigned long now = millis();

    // Messung
    if (now - lastMeasure >= MEASURE_INTERVAL_MS) {
        lastMeasure = now;
        currentData = sensors.read();

        // Serial-Ausgabe
        Serial.printf("Feuchte: %5.1f%% (raw: %.0f) | pH: %4.1f (%.3fV) | Akku: %d%% (%.2fV)\n",
                       currentData.moisturePercent, currentData.moistureRaw,
                       currentData.phValue, currentData.phVoltage,
                       currentData.batteryPercent, currentData.batteryVoltage);

        // Display aktualisieren
        if (currentMode == MODE_MAIN) {
            ui.showMain(currentData);
        } else if (currentMode == MODE_CAL_PH) {
            ui.showCalibrationPH(calStep, sensors.readPHVoltage());
        } else if (currentMode == MODE_CAL_MOISTURE) {
            ui.showCalibrationMoisture(calStep, sensors.readMoistureRaw());
        }

        // Webserver aktualisieren
        web.update(currentData);

        // Batterie-Warnung
        if (currentData.batteryVoltage < BATTERY_LOW_WARN && currentData.batteryVoltage > 1.0) {
            digitalWrite(PIN_LED, (millis() / 1000) % 2);
        }
    }

    // Webserver-Requests bearbeiten
    if (web.running) web.server.handleClient();

    // Display-Timeout
    if (ui.shouldSleep()) {
        ui.sleep();
        Serial.println("Display aus (Timeout). Taste druecken zum Aufwecken.");
    }

    delay(10);
}

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Messwerte
float moisturePercent = 0.0;
float phValue = 0.0;
unsigned long lastMeasure = 0;

float readMoisture() {
    int raw = analogRead(PIN_MOISTURE);

    // ADC-Wert auf 0-100% umrechnen (invertiert: hoher Wert = trocken)
    float percent = map(raw, MOISTURE_DRY, MOISTURE_WET, 0, 100);
    return constrain(percent, 0, 100);
}

float readPH() {
    // Mehrfachmessung für stabilen Wert
    long sum = 0;
    for (int i = 0; i < 10; i++) {
        sum += analogRead(PIN_PH);
        delay(10);
    }
    float avgRaw = sum / 10.0;

    // Spannung berechnen (ESP32: 3.3V, 12-bit ADC)
    float voltage = avgRaw * (3.3 / 4095.0);

    // pH berechnen (lineare Umrechnung)
    float ph = 7.0 + ((2.5 - voltage) / (PH_SLOPE / 1000.0)) + PH_OFFSET;
    return constrain(ph, 0, 14);
}

const char* getWaterStatus(float moisture) {
    if (moisture < MOISTURE_THRESHOLD_LOW) return "Giessen!";
    if (moisture > MOISTURE_THRESHOLD_HIGH) return "Zu nass!";
    return "Optimal";
}

const char* getFertilizerHint(float ph) {
    if (ph < 5.5) return "Boden zu sauer - kalken";
    if (ph > 7.5) return "Boden zu basisch";
    if (ph >= 6.0 && ph <= 7.0) return "pH optimal";
    return "pH akzeptabel";
}

void updateDisplay() {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    // Titel
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("--- SmartSoil ---");

    // Feuchtigkeit
    display.setCursor(0, 16);
    display.print("Feuchte: ");
    display.print(moisturePercent, 1);
    display.println("%");

    // pH-Wert
    display.setCursor(0, 28);
    display.print("pH-Wert: ");
    display.println(phValue, 1);

    // Status-Linie
    display.setCursor(0, 42);
    display.setTextSize(1);
    display.println(getWaterStatus(moisturePercent));

    // Dünge-Hinweis
    display.setCursor(0, 54);
    display.println(getFertilizerHint(phValue));

    display.display();
}

void setup() {
    Serial.begin(115200);
    Serial.println("SmartSoil startet...");

    // ADC konfigurieren
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);

    // OLED starten
    Wire.begin(PIN_SDA, PIN_SCL);
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("OLED nicht gefunden!");
        while (true) delay(1000);
    }

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 20);
    display.println("SmartSoil");
    display.display();
    delay(2000);
}

void loop() {
    unsigned long now = millis();

    if (now - lastMeasure >= MEASURE_INTERVAL_MS) {
        lastMeasure = now;

        moisturePercent = readMoisture();
        phValue = readPH();

        // Serial-Ausgabe für Debugging
        Serial.printf("Feuchte: %.1f%% | pH: %.1f | %s\n",
                       moisturePercent, phValue, getWaterStatus(moisturePercent));

        updateDisplay();
    }
}

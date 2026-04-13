/**
 * SmartSoil v2 - Sensor-Modul
 * Kapazitive Feuchtesensoren mit Kalibrierung und Trend-Erkennung
 */

#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <EEPROM.h>
#include "config.h"

// Feuchte-Status Kategorien
enum MoistureStatus {
    STATUS_CRITICAL_DRY,   // < 15% - Pflanze in Not!
    STATUS_DRY,            // 15-30% - Giessen!
    STATUS_OK,             // 30-65% - Perfekt
    STATUS_WET,            // 65-80% - Etwas viel
    STATUS_FLOODED         // > 80% - Zu nass
};

// Trend über die letzten Messungen
enum MoistureTrend {
    TREND_FALLING_FAST,   // Trocknet schnell
    TREND_FALLING,        // Trocknet langsam
    TREND_STABLE,         // Stabil
    TREND_RISING,         // Wird feuchter
};

struct PlantReading {
    uint8_t  sensorIndex;
    float    moisturePercent;      // 0-100%
    int      rawADC;               // Rohwert für Debugging
    MoistureStatus status;
    MoistureTrend  trend;
    unsigned long  timestamp;      // millis() der letzten Messung
    float    history[HISTORY_SIZE]; // Verlaufsdaten
    uint8_t  historyCount;         // Wie viele Werte gespeichert
    uint8_t  historyIndex;         // Ringpuffer-Zeiger
    bool     valid;                // Sensor angeschlossen und misst sinnvoll?
};

class SensorManager {
public:
    PlantReading plants[5];   // Max 5 Sensoren

    // Kalibrierungswerte pro Sensor
    int calDry[5];
    int calWet[5];

    void begin() {
        analogReadResolution(12);       // 12-bit ADC: 0-4095
        analogSetAttenuation(ADC_11db); // Messbereich 0-3.3V

        // Kalibrierung laden oder Defaults setzen
        EEPROM.begin(EEPROM_SIZE);
        if (EEPROM.read(EEPROM_ADDR_MAGIC) == EEPROM_MAGIC_BYTE) {
            loadCalibration();
            Serial.println("[Sensoren] Kalibrierung aus EEPROM geladen");
        } else {
            for (int i = 0; i < 5; i++) {
                calDry[i] = CAL_DRY_DEFAULT;
                calWet[i] = CAL_WET_DEFAULT;
            }
            Serial.println("[Sensoren] Standard-Kalibrierung aktiv");
        }

        // PlantReading initialisieren
        for (int i = 0; i < NUM_SENSORS; i++) {
            plants[i].sensorIndex = i;
            plants[i].moisturePercent = 0;
            plants[i].rawADC = 0;
            plants[i].status = STATUS_DRY;
            plants[i].trend = TREND_STABLE;
            plants[i].timestamp = 0;
            plants[i].historyCount = 0;
            plants[i].historyIndex = 0;
            plants[i].valid = true;
            for (int j = 0; j < HISTORY_SIZE; j++) plants[i].history[j] = 0;
        }
    }

    // Alle Sensoren lesen
    void readAll() {
        for (int i = 0; i < NUM_SENSORS; i++) {
            readSensor(i);
        }
    }

    // Einen Sensor lesen
    void readSensor(int idx) {
        if (idx >= NUM_SENSORS) return;

        int raw = readMedian(SENSOR_PINS[idx], SAMPLES_PER_READING);
        float pct = rawToPercent(raw, calDry[idx], calWet[idx]);

        // Plausibilitätsprüfung
        // Wenn ADC = 0 oder 4095 → Sensor vermutlich nicht angeschlossen
        plants[idx].valid = (raw > 50 && raw < 4050);
        plants[idx].rawADC = raw;
        plants[idx].moisturePercent = pct;
        plants[idx].status = calcStatus(pct);
        plants[idx].timestamp = millis();

        // Verlaufspuffer aktualisieren (Ringpuffer)
        plants[idx].history[plants[idx].historyIndex] = pct;
        plants[idx].historyIndex = (plants[idx].historyIndex + 1) % HISTORY_SIZE;
        if (plants[idx].historyCount < HISTORY_SIZE) plants[idx].historyCount++;

        // Trend berechnen
        plants[idx].trend = calcTrend(idx);
    }

    // Kalibrierung: aktuellen Wert als "trocken" speichern
    void calibrateDry(int idx) {
        calDry[idx] = readMedian(SENSOR_PINS[idx], SAMPLES_PER_READING);
        Serial.printf("[Cal] Sensor %d trocken: %d\n", idx, calDry[idx]);
    }

    // Kalibrierung: aktuellen Wert als "nass" speichern
    void calibrateWet(int idx) {
        calWet[idx] = readMedian(SENSOR_PINS[idx], SAMPLES_PER_READING);
        Serial.printf("[Cal] Sensor %d nass: %d\n", idx, calWet[idx]);
    }

    void saveCalibration() {
        EEPROM.write(EEPROM_ADDR_MAGIC, EEPROM_MAGIC_BYTE);
        for (int i = 0; i < 5; i++) {
            int offset = EEPROM_CAL_BASE + i * 8;
            EEPROM.put(offset, calDry[i]);
            EEPROM.put(offset + 4, calWet[i]);
        }
        EEPROM.commit();
        Serial.println("[Cal] Kalibrierung gespeichert");
    }

    const char* statusText(MoistureStatus s) {
        switch(s) {
            case STATUS_CRITICAL_DRY: return "KRITISCH TROCKEN";
            case STATUS_DRY:          return "Giessen!";
            case STATUS_OK:           return "Optimal";
            case STATUS_WET:          return "Feucht";
            case STATUS_FLOODED:      return "Zu nass!";
        }
        return "?";
    }

    const char* trendSymbol(MoistureTrend t) {
        switch(t) {
            case TREND_FALLING_FAST: return "↓↓";
            case TREND_FALLING:      return "↓";
            case TREND_STABLE:       return "→";
            case TREND_RISING:       return "↑";
        }
        return "~";
    }

private:
    void loadCalibration() {
        for (int i = 0; i < 5; i++) {
            int offset = EEPROM_CAL_BASE + i * 8;
            EEPROM.get(offset, calDry[i]);
            EEPROM.get(offset + 4, calWet[i]);
            // Sanity check
            if (calDry[i] < 1000 || calDry[i] > 4095) calDry[i] = CAL_DRY_DEFAULT;
            if (calWet[i] < 100  || calWet[i] > 3000)  calWet[i] = CAL_WET_DEFAULT;
        }
    }

    // Median-Filter: sortiert N Messungen, nimmt den mittleren Wert
    // Ignoriert Ausreißer deutlich besser als Mittelwert
    int readMedian(uint8_t pin, int n) {
        int* vals = new int[n];
        for (int i = 0; i < n; i++) {
            vals[i] = analogRead(pin);
            delayMicroseconds(500);
        }
        // Insertion-Sort (für kleine Arrays effizient)
        for (int i = 1; i < n; i++) {
            int key = vals[i];
            int j = i - 1;
            while (j >= 0 && vals[j] > key) {
                vals[j + 1] = vals[j];
                j--;
            }
            vals[j + 1] = key;
        }
        int result = vals[n / 2];
        delete[] vals;
        return result;
    }

    float rawToPercent(int raw, int dry, int wet) {
        // Invertiert: hoher ADC = trocken, niedriger ADC = nass
        float pct = (float)(dry - raw) / (float)(dry - wet) * 100.0f;
        return constrain(pct, 0.0f, 100.0f);
    }

    MoistureStatus calcStatus(float pct) {
        if (pct < THRESH_CRITICAL_DRY) return STATUS_CRITICAL_DRY;
        if (pct < THRESH_DRY)          return STATUS_DRY;
        if (pct < THRESH_OK_HIGH)      return STATUS_OK;
        if (pct < THRESH_WET)          return STATUS_WET;
        return STATUS_FLOODED;
    }

    MoistureTrend calcTrend(int idx) {
        int count = plants[idx].historyCount;
        if (count < 3) return TREND_STABLE;

        // Letzten 6 Werte (oder weniger wenn nicht genug da) vergleichen
        int lookback = min(count, 6);
        // Rückwärts durch Ringpuffer lesen
        int latestIdx  = ((int)plants[idx].historyIndex - 1 + HISTORY_SIZE) % HISTORY_SIZE;
        int oldestIdx  = ((int)plants[idx].historyIndex - lookback + HISTORY_SIZE) % HISTORY_SIZE;

        float latest = plants[idx].history[latestIdx];
        float oldest = plants[idx].history[oldestIdx];
        float delta  = latest - oldest;  // Positiv = wird feuchter, Negativ = trocknet

        if (delta < -8.0f)  return TREND_FALLING_FAST;
        if (delta < -2.0f)  return TREND_FALLING;
        if (delta >  2.0f)  return TREND_RISING;
        return TREND_STABLE;
    }
};

#endif // SENSORS_H

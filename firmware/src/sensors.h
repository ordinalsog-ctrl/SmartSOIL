#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <EEPROM.h>
#include "config.h"

struct SensorData {
    float moisturePercent;
    float moistureRaw;
    float phValue;
    float phVoltage;
    float batteryVoltage;
    int   batteryPercent;
};

class Sensors {
public:
    // Kalibrierungswerte (im RAM, aus EEPROM geladen)
    float phCal4V = PH_CAL_DEFAULT_4;
    float phCal7V = PH_CAL_DEFAULT_7;
    int   moistDry = MOISTURE_DRY;
    int   moistWet = MOISTURE_WET;

    void begin() {
        analogReadResolution(12);
        analogSetAttenuation(ADC_11db);
        loadCalibration();
    }

    SensorData read() {
        SensorData data;

        // Feuchte
        data.moistureRaw = readMedian(PIN_MOISTURE, MOISTURE_SAMPLES);
        data.moisturePercent = mapFloat(data.moistureRaw, moistDry, moistWet, 0.0, 100.0);
        data.moisturePercent = constrain(data.moisturePercent, 0.0f, 100.0f);

        // pH
        float phRaw = readMedian(PIN_PH, PH_SAMPLES);
        data.phVoltage = phRaw * (3.3 / 4095.0);
        // Lineare Interpolation zwischen den 2 Kalibrierpunkten
        float slope = (7.0 - 4.0) / (phCal7V - phCal4V);
        data.phValue = 7.0 + slope * (data.phVoltage - phCal7V);
        data.phValue = constrain(data.phValue, 0.0f, 14.0f);

        // Batterie
        float batRaw = readMedian(PIN_BATTERY, 10);
        data.batteryVoltage = (batRaw * 3.3 / 4095.0) * BATTERY_DIVIDER;
        data.batteryPercent = (int)mapFloat(data.batteryVoltage,
                                            BATTERY_EMPTY, BATTERY_FULL, 0.0, 100.0);
        data.batteryPercent = constrain(data.batteryPercent, 0, 100);

        return data;
    }

    // Kalibrierung pH: aktuelle Spannung für Puffer speichern
    void calibratePH4(float voltage) { phCal4V = voltage; }
    void calibratePH7(float voltage) { phCal7V = voltage; }

    // Kalibrierung Feuchte
    void calibrateMoistureDry(int raw) { moistDry = raw; }
    void calibrateMoistureWet(int raw) { moistWet = raw; }

    float readPHVoltage() {
        float raw = readMedian(PIN_PH, PH_SAMPLES);
        return raw * (3.3 / 4095.0);
    }

    int readMoistureRaw() {
        return (int)readMedian(PIN_MOISTURE, MOISTURE_SAMPLES);
    }

    void saveCalibration() {
        EEPROM.write(EEPROM_ADDR_MAGIC, EEPROM_MAGIC);
        EEPROM.put(EEPROM_ADDR_PH_4V, phCal4V);
        EEPROM.put(EEPROM_ADDR_PH_7V, phCal7V);
        EEPROM.put(EEPROM_ADDR_MOIST_D, moistDry);
        EEPROM.put(EEPROM_ADDR_MOIST_W, moistWet);
        EEPROM.commit();
    }

    void loadCalibration() {
        EEPROM.begin(EEPROM_SIZE);
        if (EEPROM.read(EEPROM_ADDR_MAGIC) == EEPROM_MAGIC) {
            EEPROM.get(EEPROM_ADDR_PH_4V, phCal4V);
            EEPROM.get(EEPROM_ADDR_PH_7V, phCal7V);
            EEPROM.get(EEPROM_ADDR_MOIST_D, moistDry);
            EEPROM.get(EEPROM_ADDR_MOIST_W, moistWet);
            Serial.println("Kalibrierung aus EEPROM geladen.");
        } else {
            Serial.println("Keine Kalibrierung gefunden, Defaults aktiv.");
        }
    }

private:
    // Median-Filter: sortiert N Messungen und nimmt den mittleren Wert
    // Robuster als Mittelwert (Ausreißer werden ignoriert)
    float readMedian(int pin, int samples) {
        int values[samples];
        for (int i = 0; i < samples; i++) {
            values[i] = analogRead(pin);
            delay(2);
        }
        // Einfacher Bubblesort (kleine Arrays)
        for (int i = 0; i < samples - 1; i++) {
            for (int j = i + 1; j < samples; j++) {
                if (values[j] < values[i]) {
                    int tmp = values[i];
                    values[i] = values[j];
                    values[j] = tmp;
                }
            }
        }
        return values[samples / 2];
    }

    float mapFloat(float x, float inMin, float inMax, float outMin, float outMax) {
        return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    }
};

#endif

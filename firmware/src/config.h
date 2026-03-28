#ifndef CONFIG_H
#define CONFIG_H

// --- Pin-Belegung ---
#define PIN_MOISTURE    34   // Kapazitiver Feuchtesensor (ADC1)
#define PIN_PH          35   // pH-Sensor (ADC1)
#define PIN_SDA         21   // OLED I2C Data
#define PIN_SCL         22   // OLED I2C Clock

// --- OLED Display ---
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define OLED_ADDR       0x3C

// --- Kalibrierung Feuchtesensor ---
// Werte anpassen nach Kalibrierung mit trockenem/nassem Boden
#define MOISTURE_DRY    3500  // ADC-Wert bei trockener Erde
#define MOISTURE_WET    1500  // ADC-Wert bei nasser Erde

// --- Kalibrierung pH-Sensor ---
// 2-Punkt-Kalibrierung mit Pufferlösungen
#define PH_OFFSET       0.0   // Offset nach Kalibrierung
#define PH_SLOPE        -5.70 // Steigung (mV pro pH)

// --- Schwellenwerte ---
#define MOISTURE_THRESHOLD_LOW   30  // Unter 30% -> "Gießen!"
#define MOISTURE_THRESHOLD_HIGH  70  // Über 70% -> "Zu nass!"

// --- Messintervall ---
#define MEASURE_INTERVAL_MS  5000  // Alle 5 Sekunden messen

#endif

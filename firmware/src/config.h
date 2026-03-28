#ifndef CONFIG_H
#define CONFIG_H

// --- Pin-Belegung ---
#define PIN_MOISTURE        34    // Kapazitiver Feuchtesensor (ADC1_CH6)
#define PIN_PH              35    // pH-Sensor (ADC1_CH7)
#define PIN_BATTERY         32    // Batteriespannung über Spannungsteiler (ADC1_CH4)
#define PIN_SDA             21    // OLED I2C Data
#define PIN_SCL             22    // OLED I2C Clock
#define PIN_BUTTON          27    // Taster für Kalibrierung/Modus
#define PIN_LED             2     // Onboard LED (Status)

// --- OLED Display ---
#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT       64
#define OLED_ADDR           0x3C

// --- Kalibrierung Feuchtesensor ---
#define MOISTURE_DRY        3500  // ADC-Wert bei trockener Erde
#define MOISTURE_WET        1500  // ADC-Wert bei nasser Erde
#define MOISTURE_SAMPLES    20    // Anzahl Messungen für Mittelwert

// --- Kalibrierung pH-Sensor ---
#define PH_SAMPLES          20    // Anzahl Messungen für Mittelwert
#define PH_CAL_DEFAULT_4    1.98  // Erwartete Spannung bei pH 4.0 Puffer
#define PH_CAL_DEFAULT_7    2.50  // Erwartete Spannung bei pH 7.0 Puffer

// --- Schwellenwerte Feuchte ---
#define MOISTURE_THRESHOLD_LOW    30   // Unter 30% -> "Gießen!"
#define MOISTURE_THRESHOLD_HIGH   70   // Über 70% -> "Zu nass!"

// --- Schwellenwerte pH ---
#define PH_OPTIMAL_LOW      6.0
#define PH_OPTIMAL_HIGH     7.0
#define PH_WARN_LOW         5.5
#define PH_WARN_HIGH        7.5

// --- Batterie ---
// Spannungsteiler: 100kΩ + 100kΩ -> Faktor 2
#define BATTERY_DIVIDER     2.0
#define BATTERY_FULL        4.2   // LiPo voll
#define BATTERY_EMPTY       3.3   // LiPo leer
#define BATTERY_LOW_WARN    3.5   // Warnung ab hier

// --- Timing ---
#define MEASURE_INTERVAL_MS     5000    // Alle 5 Sekunden messen
#define DISPLAY_TIMEOUT_MS      30000   // Display aus nach 30s Inaktivität
#define LONG_PRESS_MS           3000    // Langer Tastendruck für Kalibrierung
#define DEBOUNCE_MS             50      // Taster-Entprellung

// --- WiFi ---
#define WIFI_AP_SSID        "SmartSoil"
#define WIFI_AP_PASS        "smartsoil123"
#define WEBSERVER_PORT      80

// --- EEPROM ---
#define EEPROM_SIZE         64
#define EEPROM_MAGIC        0x55    // Prüfbyte ob kalibriert
#define EEPROM_ADDR_MAGIC   0
#define EEPROM_ADDR_PH_4V   4       // float: Spannung bei pH 4
#define EEPROM_ADDR_PH_7V   8       // float: Spannung bei pH 7
#define EEPROM_ADDR_MOIST_D 12      // int: ADC trocken
#define EEPROM_ADDR_MOIST_W 16      // int: ADC nass

#endif

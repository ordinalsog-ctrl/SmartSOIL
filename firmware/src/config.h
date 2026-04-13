/**
 * SmartSoil – Konfiguration
 * WaveShare ESP32-S3 Zero
 */

#ifndef CONFIG_H
#define CONFIG_H

// ── Gerät ────────────────────────────────────────────────────
#define DEVICE_ID               "sensor_01"
#define PLANT_NAME              "Monstera"       // Anzeigename auf OLED + Dashboard

// ── Pins ──────────────────────────────────────────────────────
#ifdef FREENOVE_BOARD
  // Freenove ESP32-Wrover (Prototyp)
  #define SENSOR_PIN            34       // ADC1 – GPIO34
  #define PIN_SDA               21       // I2C SDA
  #define PIN_SCL               22       // I2C SCL
  #define PIN_LED_BUILTIN        2       // Onboard LED (kein WS2812)
  #define HAS_NEOPIXEL           0
#else
  // WaveShare ESP32-S3 Zero (Produkt)
  #define SENSOR_PIN             4       // ADC1 – GPIO4
  #define PIN_SDA                8       // I2C SDA
  #define PIN_SCL                9       // I2C SCL
  #define PIN_LED_WS2812        21       // Onboard WS2812 RGB LED
  #define HAS_NEOPIXEL           1
#endif
#define PIN_BUTTON               0       // BOOT-Taste (GPIO0) – beide Boards

// ── ADC Attenuation ───────────────────────────────────────────
// ADC_ATTENDB_MAX = höchste Dämpfung (11dB / 3.3V Bereich)
// funktioniert auf ESP32 classic und ESP32-S3
#define MY_ADC_ATTEN  ADC_ATTENDB_MAX

// ── Sensor / Kalibrierung ─────────────────────────────────────
#define SAMPLES_PER_READING     30
#define CAL_DRY_DEFAULT         3200
#define CAL_WET_DEFAULT         1100

// ── Schwellenwerte (%) ────────────────────────────────────────
#define THRESH_CRITICAL_DRY     15
#define THRESH_DRY              30
#define THRESH_OK_HIGH          65
#define THRESH_WET              80

// ── Gieß-Erkennung ────────────────────────────────────────────
#define WATERING_DELTA          12.0f    // % Anstieg = Gießen erkannt

// ── Deep Sleep ────────────────────────────────────────────────
#define SLEEP_MINUTES           30
#define OLED_ON_SECONDS         4        // Anzeigedauer nach Timer-Wake

// ── WiFi ──────────────────────────────────────────────────────
// WIFI_SSID und WIFI_PASS in firmware/src/config_secrets.h eintragen
// (Vorlage: config_secrets.h.example)
#include "config_secrets.h"
#define WIFI_TIMEOUT_SEC        15
#define WIFI_ACTIVE_TIMEOUT_MIN 15       // Auto-Sleep nach Inaktivität

// ── OLED ──────────────────────────────────────────────────────
#define OLED_ADDR               0x3C
#define OLED_WIDTH              128
#define OLED_HEIGHT             64

// ── EEPROM (Kalibrierung) ─────────────────────────────────────
#define EEPROM_SIZE             64
#define EEPROM_MAGIC_BYTE       0xA7
#define EEPROM_ADDR_MAGIC       0
#define EEPROM_CAL_BASE         4

// ── Gamification ──────────────────────────────────────────────
#define LEVEL_UP_STREAK_DAYS    7        // Streak-Tage für Level-Up

#endif // CONFIG_H

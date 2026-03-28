# Stückliste SmartSoil

| # | Bauteil | Beschreibung | ca. Preis |
|---|---------|-------------|-----------|
| 1 | ESP32 DevKit v1 | Mikrocontroller mit WiFi/BLE | 6-8€ |
| 2 | Kapazitiver Bodenfeuchtesensor v1.2 | Korrosionsfrei, analog | 2-3€ |
| 3 | pH-Sensor Modul (SEN0161) | Analog, mit BNC-Sonde | 25-35€ |
| 4 | OLED Display 0.96" SSD1306 | 128x64, I2C | 3-5€ |
| 5 | 3.7V LiPo Akku 1000mAh | Stromversorgung | 5-8€ |
| 6 | TP4056 Lademodul | USB-C Laden mit Schutzschaltung | 1-2€ |
| 7 | Breadboard + Jumper-Kabel | Für Prototyp | 3-5€ |
| 8 | Gehäuse (3D-Druck) | Wetterfest, Erdspieß | - |

**Gesamtkosten Prototyp: ca. 45-65€**

## Verkabelung

```
ESP32 Pin 34 (ADC) <--- Feuchtesensor Signal
ESP32 Pin 35 (ADC) <--- pH-Sensor Signal
ESP32 Pin 21 (SDA) <--- OLED SDA
ESP32 Pin 22 (SCL) <--- OLED SCL
ESP32 3.3V         <--- Sensorversorgung
ESP32 GND          <--- Gemeinsame Masse
```

# Aufbauanleitung SmartSoil Prototyp v1

## Benötigte Teile

| # | Bauteil | Anzahl |
|---|---------|--------|
| 1 | ESP32 DevKit v1 | 1 |
| 2 | Kapazitiver Bodenfeuchtesensor v1.2 | 1 |
| 3 | pH-Sensor Modul (SEN0161 / E-201-C) | 1 |
| 4 | OLED Display 0.96" SSD1306 (I2C) | 1 |
| 5 | Taster (Momentary Push Button) | 1 |
| 6 | 10kΩ Widerstand (Pull-up für Taster, optional) | 1 |
| 7 | 2x 100kΩ Widerstände (Spannungsteiler Batterie) | 2 |
| 8 | 3.7V LiPo Akku 1000mAh | 1 |
| 9 | TP4056 Lademodul mit Schutzschaltung | 1 |
| 10 | Breadboard 830 Punkte | 1 |
| 11 | Jumper-Kabel (M-M, M-F) | ~20 |

## Verkabelung

```
                    ┌──────────────────┐
                    │    ESP32 DevKit  │
                    │                  │
   Feuchtesensor ──►│ GPIO 34 (ADC1)   │
   (Signal)         │                  │
                    │                  │
   pH-Sensor ──────►│ GPIO 35 (ADC1)   │
   (Signal/Po)      │                  │
                    │                  │
   Batterie ───┬───►│ GPIO 32 (ADC1)   │
   (über       │    │                  │
   Spannungs-  │    │                  │
   teiler)     │    │ GPIO 21 (SDA) ◄──┼──── OLED SDA
               │    │ GPIO 22 (SCL) ◄──┼──── OLED SCL
               │    │                  │
               │    │ GPIO 27 ◄────────┼──── Taster ──► GND
               │    │                  │
               │    │ 3.3V ────────────┼──►  Sensoren VCC
               │    │ GND  ────────────┼──►  Gemeinsame Masse
               │    └──────────────────┘
               │
               ├── 100kΩ ── GPIO 32
               └── 100kΩ ── GND
```

### Schritt 1: ESP32 auf Breadboard

Den ESP32 DevKit mittig auf das Breadboard stecken, sodass auf beiden Seiten Platz für Verbindungen bleibt.

### Schritt 2: OLED Display

| OLED Pin | ESP32 Pin |
|----------|-----------|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

### Schritt 3: Kapazitiver Feuchtesensor

| Sensor Pin | ESP32 Pin |
|------------|-----------|
| VCC | 3.3V |
| GND | GND |
| AOUT | GPIO 34 |

**Wichtig:** Nur den analogen Ausgang (AOUT) verwenden, nicht den digitalen (DOUT).

### Schritt 4: pH-Sensor

| pH-Modul Pin | ESP32 Pin |
|-------------|-----------|
| V+ | 3.3V |
| GND | GND |
| Po (Analog) | GPIO 35 |

**Wichtig:** Die BNC-Sonde in das pH-Modul schrauben. Vor der ersten Messung die Schutzkappe mit KCl-Lösung von der Sonde entfernen.

### Schritt 5: Batteriemessung (Spannungsteiler)

```
LiPo (+) ──── 100kΩ ──┬── 100kΩ ──── GND
                       │
                       └──► GPIO 32
```

Teilt die 4.2V des LiPo auf max. 2.1V herunter (sicher für den ESP32 ADC).

### Schritt 6: Taster

```
GPIO 27 ──── Taster ──── GND
```

Der interne Pull-up-Widerstand des ESP32 wird per Software aktiviert (`INPUT_PULLUP`).

### Schritt 7: Stromversorgung

```
LiPo (+) ──► TP4056 BAT+ ──► ESP32 VIN
LiPo (-) ──► TP4056 BAT- ──► ESP32 GND
```

USB-C am TP4056 zum Laden. Der ESP32 läuft über VIN mit 3.7-4.2V.

---

## Software flashen

### Voraussetzung: PlatformIO installieren

```bash
# Option A: VS Code Extension
# -> VS Code öffnen -> Extensions -> "PlatformIO IDE" installieren

# Option B: CLI
pip install platformio
```

### Kompilieren & Hochladen

```bash
cd firmware

# Kompilieren
pio run

# Auf ESP32 flashen (USB-Kabel anschließen)
pio run -t upload

# Serial Monitor öffnen (Messwerte beobachten)
pio device monitor
```

---

## Kalibrierung

### pH-Sensor kalibrieren

1. **Taster 3 Sekunden lang drücken** → Kalibrierungsmodus startet
2. pH-Sonde in **pH 7.0 Pufferlösung** tauchen
3. Warten bis Spannung stabil (ca. 30 Sekunden)
4. **Taster kurz drücken** → Wert wird übernommen
5. pH-Sonde in **pH 4.0 Pufferlösung** tauchen
6. Warten bis Spannung stabil
7. **Taster kurz drücken** → Kalibrierung gespeichert

### Feuchtesensor kalibrieren

Nach der pH-Kalibrierung folgt automatisch die Feuchte-Kalibrierung:

1. Sensor in **trockene Erde** stecken
2. **Taster kurz drücken** → Trockenwert gespeichert
3. Sensor in **komplett nasse Erde** stecken
4. **Taster kurz drücken** → Nasswert gespeichert

Die Kalibrierungswerte werden im EEPROM gespeichert und bleiben nach Neustart erhalten.

---

## WiFi Webinterface

Nach dem Start erstellt der ESP32 einen eigenen WLAN-Hotspot:

- **SSID:** SmartSoil
- **Passwort:** smartsoil123
- **Webinterface:** http://192.168.4.1

Das Webinterface zeigt die aktuellen Messwerte in Echtzeit im Browser an.

---

## Fehlerbehebung

| Problem | Lösung |
|---------|--------|
| OLED zeigt nichts | I2C-Adresse prüfen (0x3C oder 0x3D). SDA/SCL vertauscht? |
| Feuchte immer 0% oder 100% | Kalibrierung durchführen. Sensoranschluss prüfen (AOUT, nicht DOUT) |
| pH-Wert springt stark | Sonde 30s einweichen lassen. Mehrere Messzyklen abwarten |
| WiFi nicht sichtbar | Ausreichend Strom? USB-Kabel mit Datenleitung verwenden |
| Serial Monitor leer | Baudrate auf 115200 stellen. Reset-Taste am ESP32 drücken |
| LED blinkt dauerhaft | Akku laden! Blinken = Batterie-Warnung |

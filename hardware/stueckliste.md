# Stückliste SmartSoil

## Custom PCB v1.0 (JLCPCB, bestellt 2026-04-13)

> Vollständige BOM-Datei: `hardware/pcb/BOM_SmartSoil_v1.csv`
> Lieferung erwartet: ~2026-04-21

| # | Designator | Bauteil | Wert | LCSC | Hersteller |
|---|-----------|---------|------|------|------------|
| 1 | C1 | Kondensator | 100nF / 0402 | C1525 | SAMSUNG |
| 2 | C2 | Kondensator | 10uF / 0603 | C19702 | SAMSUNG |
| 3 | CN1 | JST Connector 2-Pin | B2B-PH-K-S-GW | C5251182 | JST |
| 4 | LED1 | RGB LED | WS2812B-B/T | C2761795 | Worldsemi |
| 5 | R1, R2 | Widerstand | 5.1kΩ / 0603 | C269716 | Tyohm |
| 6 | SW1 | Taster | SKRKAEE020 | C115357 | ALPSALPINE |
| 7 | U1 | ESP32-S3 Modul | ESP32-S3-WROOM-1-N4 | C2913197 | ESPRESSIF |
| 8 | U2 | LiPo Ladecontroller | TP4056-42-ESOP8 | C16581 | TOPPOWER |
| 9 | U3 | LDO Regler 3.3V | AMS1117-3.3 | C6186 | AMS |
| 10 | U4 | JST Connector 4-Pin | B4B-PH-K-S-GW | C566011 | JST |
| 11 | U5 | JST Connector 3-Pin | B3B-PH-KL(LF)(SN) | C158004 | JST |
| 12 | USBC1 | USB-C Buchse | TYPE-C-31-M-12 | C165948 | KH |

**PCB Spezifikationen:**
- Board-Größe: 55 × 42 mm
- Layer: 2
- Dicke: 1.6mm
- Farbe: Grün, HASL (with lead)
- Menge: 2× PCBA (+ 3× bare PCB)
- Fertigungsart: Standard PCBA (Top Side)

---

## Externe Komponenten (manuell anschließen)

| # | Bauteil | Modell | Connector | GPIO | Quelle |
|---|---------|--------|-----------|------|--------|
| 1 | Feuchtigkeitssensor | AZ-Delivery Capacitive v1.2 | U5 (3-Pin JST) | GPIO4 (ADC) | bereits vorhanden |
| 2 | OLED Display | 0.96" SSD1306 128x64 I2C | U4 (4-Pin JST) | GPIO8 (SDA), GPIO9 (SCL) | zu bestellen |
| 3 | LiPo Akku | 3.7V 2000mAh, JST-PH 2mm | CN1 (2-Pin JST) | — | zu bestellen |

---

## Pin-Belegung Custom PCB

| GPIO | Funktion | Bauteil |
|------|----------|---------|
| GPIO 0 | Button (BOOT) | SW1 |
| GPIO 4 | ADC – Feuchtigkeitssensor | U5 Pin 3 |
| GPIO 8 | I2C SDA – OLED | U4 Pin 2 |
| GPIO 9 | I2C SCL – OLED | U4 Pin 3 |
| GPIO 21 | WS2812B RGB LED | LED1 |

---

## Gesamtkosten

| Position | Kosten |
|----------|--------|
| JLCPCB PCB + PCBA (2 Boards) | $125.31 (~116€) |
| AZ-Delivery Feuchtigkeitssensor v1.2 | bereits vorhanden |
| OLED Display 0.96" SSD1306 | ~5€ |
| LiPo 2000mAh (JST-PH 2mm) | ~8€ |
| **Gesamt Prototyp (2 Module)** | **~129€** |
| **Kosten pro Modul** | **~65€** |

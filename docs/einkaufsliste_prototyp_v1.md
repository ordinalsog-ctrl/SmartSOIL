# Einkaufsliste SmartSoil Prototyp v1

> Komplett-Liste für den Aufbau von Null. Alles über Amazon.de bestellbar.

---

## Empfehlung: Starter Kit + Einzelteile

Am günstigsten und anfängerfreundlichsten ist ein **ESP32 Starter Kit**, das Breadboard, Kabel, Taster, Widerstände und den ESP32 selbst enthält. Dazu nur noch die projektspezifischen Sensoren einzeln bestellen.

---

## Teil A: Starter Kit (enthält Grundausstattung)

### FREENOVE Super Starter Kit für ESP32-WROVER
- **Inhalt:** ESP32-Board, Breadboard, Jumper-Kabel, Taster, Widerstände (auch 100kΩ), LEDs, USB-Kabel, 516-Seiten Tutorial mit 81 Projekten
- **Preis:** ca. 35-50€
- **Amazon.de:** Suche nach "FREENOVE Super Starter Kit ESP32"
- **Damit abgedeckt:** ESP32, Breadboard, Kabel, Taster, Widerstände, USB-Kabel

> **Warum dieses Kit?** Enthält fast alles für den Grundaufbau + exzellente Dokumentation für Anfänger. Die 81 Beispielprojekte helfen beim Einstieg.

---

## Teil B: Projektspezifische Bauteile (einzeln bestellen)

### 1. Kapazitiver Bodenfeuchtesensor v1.2
- **Produkt:** AZDelivery Bodenfeuchtesensor Hygrometer Modul V1.2 kapazitiv
- **Warum kapazitiv:** Korrosionsfrei, hält Jahre in nasser Erde (anders als billige resistive Sensoren mit offenen Kontakten)
- **Preis:** ca. 4-8€
- **Amazon.de:** Suche nach "AZDelivery Bodenfeuchtesensor kapazitiv v1.2"
- **Tipp:** Schnittkanten mit Klarlack oder Epoxy versiegeln für längere Lebensdauer

### 2. pH-Sensor Modul mit BNC-Sonde
- **Produkt:** DFRobot Analog pH Sensor Kit V2 (SEN0161-V2)
- **Messbereich:** pH 0-14, Genauigkeit ±0.1 pH
- **Anschluss:** Analogausgang, BNC-Sonde zum Eintauchen
- **Preis:** ca. 30-46€
- **Amazon.de:** Suche nach "DFRobot SEN0161 pH Sensor Arduino"
- **Hinweis:** Teuerste Einzelkomponente, aber entscheidend für das Projekt

### 3. OLED Display 0.96" SSD1306
- **Produkt:** AZDelivery 0.96 Zoll OLED Display I2C 128x64
- **Anschluss:** I2C (nur 4 Kabel: VCC, GND, SDA, SCL)
- **Preis:** ca. 5-7€ (Einzeln), ca. 12-15€ (3er-Pack)
- **Amazon.de:** Suche nach "AZDelivery OLED Display 0.96 I2C"
- **Hinweis:** Kann im FREENOVE Kit enthalten sein – vorher prüfen!

### 4. LiPo Akku 3.7V 1000mAh
- **Anforderung:** 3.7V, min. 1000mAh, JST PH2.0 Stecker, mit Schutzschaltung
- **Preis:** ca. 5-10€
- **Amazon.de:** Suche nach "LiPo Akku 3.7V 1000mAh JST"
- **Wichtig:** Nur Akkus MIT eingebauter Schutzschaltung (PCM/BMS) kaufen!

### 5. TP4056 Lademodul mit Schutzschaltung
- **Produkt:** TP4056 Lademodul mit DW01 Schutz-IC
- **Funktion:** Lädt den LiPo via USB, schützt vor Überladung/Tiefentladung/Kurzschluss
- **Preis:** ca. 3-5€ (oft im 5er-Pack)
- **Amazon.de:** Suche nach "TP4056 Lademodul Micro USB Schutzschaltung"
- **Tipp:** Micro-USB-Version ist günstiger und häufiger verfügbar als USB-C

### 6. pH-Pufferlösungen zur Kalibrierung
- **Produkt:** pH Kalibrierlösung Set (pH 4.0 + pH 7.0)
- **Menge:** Je 100-150ml reicht für viele Kalibrierungen
- **Preis:** ca. 8-12€
- **Amazon.de:** Suche nach "pH Kalibrierlösung 4.0 7.0 Set"

---

## Teil C: Werkzeug (einmalige Anschaffung)

### 7. Lötkolben Starter Set
- **Inhalt:** Lötkolben (regelbar 180-520°C), Lötzinn, Entlötpumpe, Pinzette, Ablage mit Schwamm
- **Preis:** ca. 18-25€
- **Amazon.de:** Suche nach "Lötkolben Set Anfänger"
- **Wofür:** Header-Pins auf Platinen löten, Kabelverbindungen für finale Version
- **Hinweis:** Für den Breadboard-Prototyp ist Löten noch NICHT nötig – nur für spätere feste Verbindungen

---

## Zusammenfassung Bestellliste

| # | Bauteil | ca. Preis | Priorität |
|---|---------|-----------|-----------|
| A | FREENOVE Super Starter Kit (ESP32 + Zubehör) | 35-50€ | Muss |
| B1 | Kapazitiver Bodenfeuchtesensor v1.2 | 4-8€ | Muss |
| B2 | DFRobot pH-Sensor SEN0161 | 30-46€ | Muss |
| B3 | OLED Display 0.96" SSD1306 I2C | 5-7€ | Muss (falls nicht im Kit) |
| B4 | LiPo Akku 3.7V 1000mAh | 5-10€ | Kann warten* |
| B5 | TP4056 Lademodul | 3-5€ | Kann warten* |
| B6 | pH-Pufferlösungen Set | 8-12€ | Muss |
| C1 | Lötkolben Set | 18-25€ | Kann warten** |
| | **Gesamt** | **~110-165€** | |

*Für den Prototyp reicht erstmal USB-Stromversorgung – Akku kann später ergänzt werden.

**Zum Breadboard-Prototyp steckt man alles ohne Löten zusammen.

### Spar-Tipp: Erst ohne pH starten (~60-80€)

Wenn das Budget knapp ist, zuerst nur mit Feuchtesensor starten und den pH-Sensor (30-46€) später nachkaufen. Die Firmware unterstützt beides unabhängig.

---

## Aufbaureihenfolge nach Erhalt

### Tag 1: Grundaufbau (nur Starter Kit + Feuchtesensor)
1. ESP32 auf Breadboard stecken
2. OLED Display anschließen (4 Kabel)
3. Firmware flashen (PlatformIO installieren, USB anschließen)
4. Prüfen ob Display "SmartSoil" zeigt
5. Feuchtesensor anschließen (3 Kabel)
6. In Erde stecken, Messwerte beobachten

### Tag 2: pH-Sensor ergänzen
7. pH-Sensor-Modul anschließen (3 Kabel)
8. BNC-Sonde einschrauben
9. Kalibrierung mit Pufferlösungen durchführen
10. Messwerte im Serial Monitor und Display prüfen

### Tag 3: WiFi testen
11. Mit Handy zum WLAN "SmartSoil" verbinden
12. Browser öffnen: http://192.168.4.1
13. Dashboard prüfen

### Später: Akku-Betrieb
14. Spannungsteiler für Batteriemessung aufbauen (2x 100kΩ)
15. TP4056 + LiPo anschließen
16. Kabellosen Betrieb testen

---

## Häufige Anfänger-Fehler

| Fehler | Vermeidung |
|--------|-----------|
| Falsches USB-Kabel | Viele Kabel sind "nur Laden" ohne Datenleitung. Wenn der ESP32 nicht erkannt wird: anderes Kabel testen |
| ESP32 zu breit für Breadboard | Den ESP32 so stecken, dass er über den Mittelsteg ragt – eine Seite hat dann nur 1 freie Reihe. Normal! |
| Sensoren an falsche Pins | Unbedingt die Pin-Nummern prüfen (GPIO 34, 35, 21, 22). Die Nummern auf dem Board können anders beschriftet sein |
| pH-Sonde austrocknen lassen | Die Sonde IMMER in der Schutzkappe mit KCl-Lösung lagern wenn nicht in Gebrauch |
| 5V an ESP32 ADC-Pin | Die ADC-Pins vertragen maximal 3.3V! Den pH-Sensor an 3.3V betreiben, NICHT an 5V |
| Breadboard-Wackelkontakte | Kabel fest einstecken. Bei sporadischen Messwerten: Verbindungen prüfen |

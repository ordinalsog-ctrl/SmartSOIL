# Marktanalyse SmartSoil

Stand: März 2026

## Zusammenfassung

Kein einziges Produkt auf dem Markt kombiniert hochgenaue pH-Messung + smarte Feuchtigkeitsmessung + App-Anbindung. Der Markt ist gespalten in:

- **Professionelle pH-Pens** – genau, aber offline und ohne Feuchtemessung
- **Smarte Pflanzensensoren** – Feuchte + App, aber kein oder ungenauer pH

**SmartSoil füllt genau diese Lücke.**

---

## Marktpositionierung

```
                    pH-Messung
                    ↑
     Bluelab/Apera  │          ★ SmartSoil
     (teuer, dumm)  │          (Marktlücke!)
                    │
                    │   FYTA
    Gain Express    │   EarthOne   Gardena
    (billig, dumm)  │   Xiaomi     Ecowitt
                    └──────────────────────→ Smart/App
```

---

## Tier 1: Professionelle pH-Messgeräte

### Bluelab Soil pH Pen
- **Misst:** pH + Temperatur (direkt im Boden)
- **Konnektivität:** Keine
- **Preis:** ~195-225€
- **Genauigkeit:** ±0.1 pH, ±1°C
- **Haltbarkeit:** Vollständig wasserdicht, 30+ Jahre Markenreputation, Sonde austauschbar
- **Vorteile:** Laborgenauigkeit, direkte Bodenmessung (kein Aufschlämmverfahren nötig), Standardgerät bei kommerziellen Züchtern
- **Nachteile:** Nur pH (keine Feuchte), keine App/Smart-Features, teuer, regelmäßige Kalibrierung nötig

### Apera GroStar GS2
- **Misst:** pH + Temperatur (direkt im Boden)
- **Konnektivität:** Keine
- **Preis:** ~257€
- **Genauigkeit:** ±0.1 pH
- **Haltbarkeit:** IP67 wasserdicht, stoßfeste Speerspitze (10x stärker als konventionelle), Sonde austauschbar, 2 Jahre Garantie
- **Vorteile:** Laborgenauigkeit, austauschbare Swiss LabSen-Sonde, PTFE Anti-Verstopfungsring, TruRead-Modus (Mehrfachmessung), volles Kalibrierungskit inklusive
- **Nachteile:** Nur pH, keine Feuchte, keine Smart-Features, teuerstes Gerät im Vergleich

**Fazit Tier 1:** Goldstandard für pH-Genauigkeit. Beide liefern ±0.1 pH Präzision. Der Apera GS2 hat den Vorteil der austauschbaren Sonde. Keines misst Feuchtigkeit – ein zweites Gerät ist nötig.

---

## Tier 2: Smarte Sensoren mit App

### FYTA Beam (deutsches Unternehmen, Berlin)
- **Misst:** Feuchte, Temperatur, Salzgehalt, Licht (pH nur mit separatem Testkit)
- **Konnektivität:** Bluetooth (BLE), Homey-Integration
- **Preis:** ~40€ (Einzelsensor), ~110€ (3er-Pack mit pH-Kit)
- **Haltbarkeit:** Solarbetrieben + CR2032 Backup (bis 2 Jahre), wechselbare Stablängen (3-20 cm)
- **Vorteile:** Umfassendste Messwerte, 3.400+ Pflanzendatenbank mit Pflegehinweisen, solarbetrieben, deutsche Entwicklung, Substratkalibrierung für verschiedene Bodenarten
- **Nachteile:** pH NICHT eingebaut (separates Kit nötig), Salzgehalt/EC algorithmisch berechnet (kein direkter Sensor), einige Nutzer melden ungenaue Nährstoffwerte, nur Bluetooth (kein WiFi)

### EarthOne Plant Monitor
- **Misst:** Feuchte, Licht, Temperatur, Luftfeuchtigkeit, VPD, Wachstumsgradtage
- **Konnektivität:** WiFi, Alexa, Google Assistant, Apple HomeKit, Siri, IFTTT (900+ Integrationen)
- **Preis:** ~30€
- **Haltbarkeit:** Wasser-/schmutzresistent, USB-C aufladbar, ~4 Monate Akkulaufzeit
- **Vorteile:** Beste Smart-Home-Integration, Erntezeitvorhersage (ML-basiert), kein Abo nötig, pH-Sensor-Port geplant, hervorragender Preis
- **Nachteile:** Noch kein pH (Port existiert für zukünftigen Sensor), relativ kurze Akkulaufzeit, einige Probleme mit Android-App in Frühphase

### Ecowitt WH51
- **Misst:** Nur Bodenfeuchte
- **Konnektivität:** WiFi (über Ecowitt Gateway), bis 8 Sensoren pro Gateway
- **Preis:** ~18€ pro Sensor, Gateway ~25-55€, Starterkit ~55€
- **Haltbarkeit:** IP66 wasserdicht, AA-Batterie, sehr erweiterbar
- **Vorteile:** Sehr günstig pro Zone, Multi-Zonen-Monitoring, Tiefenvariante verfügbar (WH51L mit 1m Kabel), Integration in Ecowitt Wetterstationsökosystem, MQTT-Support für Home Assistant
- **Nachteile:** Nur Feuchte (kein pH, keine Temperatur bei Basismodell), Gateway erforderlich

### Gardena Smart Sensor (19040-20)
- **Misst:** Bodenfeuchte, Bodentemperatur, Lichtintensität
- **Konnektivität:** Gardena Smart Gateway (proprietär)
- **Preis:** ~67-80€ (Sensor), Gateway separat ~100€+
- **Haltbarkeit:** Frostfest bis -20°C, flaches Profil (20mm über Boden, mähsicher), AA-Batterien ~1 Jahr
- **Vorteile:** Hervorragende Verarbeitung (Husqvarna-Gruppe), Integration mit Gardena Smart Irrigation für automatische Bewässerung, ganzjährige Outdoor-Nutzung
- **Nachteile:** Kein pH, teures Gateway-Ökosystem nötig, ein Sensor pro Bewässerungszone, Gesamtkosten des Systems sehr hoch (170€+)

### Xiaomi Mi Flora / Flower Care
- **Misst:** Feuchte, Temperatur, Leitfähigkeit (Fruchtbarkeit), Licht
- **Konnektivität:** Bluetooth Low Energy (BLE)
- **Preis:** ~10-15€
- **Haltbarkeit:** IP55, CR2032 Batterie, kompakt
- **Vorteile:** Günstigster smarter Sensor, gute Home Assistant Integration, misst Leitfähigkeit
- **Nachteile:** Nur Bluetooth (sehr begrenzte Reichweite), Verbindungsabbrüche häufig, kein pH, kein WiFi, Smartphone muss in der Nähe sein

### RainPoint WiFi Soil Sensor
- **Misst:** Feuchte, Temperatur (einige Modelle + Licht)
- **Konnektivität:** WiFi (über RainPoint Hub)
- **Preis:** ~40€ (Sensor), Hub/Timer separat
- **Haltbarkeit:** IPX5 wasserdicht, 304 Edelstahl-Sonden
- **Vorteile:** Integration mit RainPoint Smart Irrigation, 10-Tage-Verlauf, 5-Minuten-Aktualisierung
- **Nachteile:** Kein pH, Genauigkeit nur ±5-10%, Hub erforderlich, Ökosystem-Lock-in

---

## Tier 3: Analoge/Manuelle Messgeräte

### Gain Express ZD-06
- **Misst:** pH + Feuchte (analog, eine Sonde)
- **Konnektivität:** Keine
- **Preis:** ~20-30€
- **Haltbarkeit:** 29cm Sonde, keine Batterien nötig, als haltbarstes Analoggerät in Tests bewertet
- **Vorteile:** Genauestes und konsistentestes Analogmessgerät in Vergleichstests, tiefe Messung (20cm+), keine Batterien
- **Nachteile:** Analoganzeige, keine Datenaufzeichnung, keine App, begrenzter pH-Bereich (3-8)

### SUSTEE Aquameter (Japan)
- **Misst:** Nur Bodenfeuchte (Farbwechsel)
- **Konnektivität:** Keine (passive Anzeige)
- **Preis:** ~10-20€ (5er-Set)
- **Haltbarkeit:** Poröser Keramikkern, Kernwechsel alle 6-9 Monate
- **Vorteile:** Preisgekröntes Design (Red Dot Award), wartungsfrei, keine Batterie/App/Laden nötig, Entwicklung mit Universität Tokio für Agrarwissenschaft
- **Nachteile:** Nur Feuchte, binäre Anzeige (nass/trocken, keine Prozent), kein pH

---

## Vergleichsmatrix

| Anforderung | Bestes Produkt | Preis |
|---|---|---|
| Genauester pH | Apera GroStar GS2 | ~257€ |
| Bestes pH Preis-Leistung | Bluelab Soil pH Pen | ~195€ |
| Bester All-in-One Smart | FYTA Beam | 40-110€ |
| Beste Smart-Home-Integration | EarthOne | ~30€ |
| Bestes Multi-Zonen-Monitoring | Ecowitt WH51 System | 55€+ |
| Beste Auto-Bewässerung | Gardena Smart Sensor | 170€+ |
| Günstigster Smart-Sensor | Xiaomi Mi Flora | 10-15€ |
| Bester analoger pH+Feuchte | Gain Express ZD-06 | 20-30€ |

---

## Chancen für SmartSoil

### Die Marktlücke
Kein Produkt auf dem Markt bietet:
- ✅ Genaue pH-Messung (direkt im Boden)
- ✅ Kontinuierliche Feuchtigkeitsmessung
- ✅ Smart-Anbindung (WiFi/BLE + App)
- ✅ Bezahlbarer Preis (<100€)

### Wettbewerbsvorteile SmartSoil
1. **Einziges Gerät mit pH + Feuchte + App** – kein Konkurrenzprodukt vereint alle drei
2. **Preisvorteil** – Zielpreis ~50-70€ vs. Bluelab + FYTA Kombi (~300€+)
3. **Gieß- und Düngeempfehlung** in einem Gerät statt zwei
4. **Open Source möglich** – Differenzierung gegenüber proprietären Ökosystemen (Gardena, Ecowitt)
5. **Kein Gateway nötig** – ESP32 hat WiFi eingebaut, direkte Verbindung

### Herausforderungen
1. **pH-Sensor-Haltbarkeit** – Profisonden (Bluelab, Apera) kosten einzeln 50-100€ und müssen regelmäßig getauscht werden
2. **Kalibrierung** – pH-Sensoren müssen regelmäßig kalibriert werden (Pufferlösungen nötig)
3. **Wasserdichtigkeit** – Elektronik muss dauerhaft im Erdreich überleben
4. **Zertifizierungen** – CE-Kennzeichnung für europäischen Vertrieb

---

## Quellen
- Bob Vila – Best pH Meters
- Greenwashing Index – Soil pH Tester Tests
- Gadzetto – Best Smart Plant Monitors 2025
- Homey – Smart Soil Sensors Guide
- Ecowitt, FYTA, Gardena, Apera, Bluelab – Herstellerseiten

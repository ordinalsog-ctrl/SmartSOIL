# SmartSoil

Smarter Bodensensor für Pflanzen - misst Wassergehalt und pH-Wert der Erde.

## Features

- **Bodenfeuchte messen** - kapazitiver Sensor, Anzeige in %
- **pH-Wert messen** - für optimale Düngung und Timing
- **Gieß-Empfehlung** - automatische Benachrichtigung wenn gegossen werden muss
- **OLED-Display** - Echtzeit-Anzeige der Messwerte
- **App-Anbindung** (geplant) - via WiFi/BLE

## Hardware

- ESP32 Mikrocontroller
- Kapazitiver Bodenfeuchtesensor v1.2
- Analoger pH-Sensor (SEN0161 o.ä.)
- 0.96" OLED Display (SSD1306, I2C)
- 3.7V LiPo Akku + Lademodul

## Projektstruktur

```
SmartSoil/
├── firmware/       # ESP32 Firmware (PlatformIO)
│   ├── src/        # Quellcode
│   ├── lib/        # Bibliotheken
│   └── include/    # Header
├── hardware/       # Schaltpläne, Stückliste
├── docs/           # Dokumentation
└── app/            # Mobile App (geplant)
```

## Quickstart

1. [PlatformIO](https://platformio.org/) installieren
2. `cd firmware && pio run` zum Kompilieren
3. `pio run -t upload` zum Flashen auf den ESP32

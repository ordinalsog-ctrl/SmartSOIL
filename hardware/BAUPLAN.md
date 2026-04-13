# SmartSoil - Bauplan für absolute Anfänger

> Diese Anleitung setzt KEINERLEI Vorwissen voraus.
> Jedes Bauteil wird erklärt, jeder Handgriff beschrieben.
> Lies die Anleitung EINMAL KOMPLETT durch, bevor du anfängst.

---

## TEIL A: GRUNDWISSEN (Lies das zuerst!)

### Was ist eigentlich ein Mikrocontroller?

Ein Mikrocontroller ist ein winziger Computer auf einer kleinen Platine.
Unser ESP32 ist so ein Minicomputer. Er kann:
- Sensoren auslesen (wie feucht ist die Erde? welcher pH-Wert?)
- Berechnungen machen (soll gegossen werden?)
- Ein Display ansteuern (die Werte anzeigen)
- WiFi bereitstellen (damit dein Handy sich verbinden kann)

Er braucht dafür ein Programm (die "Firmware"), das wir per USB
auf ihn laden. Das Programm ist schon fertig geschrieben!

### Was ist ein Breadboard?

```
Ein Breadboard (Steckbrett) ist eine Plastikplatte mit vielen
kleinen Löchern. In diese Löcher steckst du Kabel und Bauteile.

WICHTIG zu verstehen - so sind die Löcher INTERN verbunden:

┌──────────────────────────────────────────────────────────────┐
│                                                              │
│  + + + + + + + + + + + + + + + + + + + + + + + + + + + + +  │
│  Diese ganze Reihe ist EIN Draht! (= "Stromschiene")        │
│  - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  │
│  Diese ganze Reihe ist EIN Draht! (= "Masseschiene")        │
│                                                              │
│  1:  a · b · c · d · e │ Lücke │ f · g · h · i · j          │
│  2:  a · b · c · d · e │       │ f · g · h · i · j          │
│  3:  a · b · c · d · e │       │ f · g · h · i · j          │
│  ...                    │       │                            │
│                                                              │
│  In jeder ZEILE sind a-b-c-d-e verbunden (= ein Draht)      │
│  In jeder ZEILE sind f-g-h-i-j verbunden (= ein Draht)      │
│  ABER: links (a-e) und rechts (f-j) sind NICHT verbunden!   │
│  Die Lücke in der Mitte trennt die beiden Seiten.            │
│                                                              │
│  - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  │
│  + + + + + + + + + + + + + + + + + + + + + + + + + + + + +  │
│                                                              │
└──────────────────────────────────────────────────────────────┘

Beispiel: Wenn du einen Draht in Loch 5a steckst und einen
anderen in Loch 5c, sind die beiden AUTOMATISCH verbunden,
weil sie in der gleichen Zeile (5) auf der gleichen Seite (a-e)
sitzen.
```

### Was sind Jumper-Kabel?

```
Jumper-Kabel sind kurze Kabel mit Steckern an beiden Enden.
Es gibt 3 Typen:

  ┌──┐────────┌──┐
  │MM│  Pin   │MM│    Male-Male (M-M): Stift an beiden Enden
  └──┘────────└──┘    → Zum Verbinden von Breadboard zu Breadboard

  ┌──┐────────[==]
  │MM│  Pin   │FF│    Male-Female (M-F): Stift + Buchse
  └──┘────────[==]    → Zum Verbinden von Breadboard zu Sensor/Display

  [==]────────[==]
  │FF│  Pin   │FF│    Female-Female (F-F): Buchse an beiden Enden
  [==]────────[==]    → Zum Verbinden von Sensor zu Sensor

"Male" (M) = Stift (passt IN ein Loch)
"Female" (F) = Buchse (da passt ein Stift HINEIN)

Für den Prototyp brauchst du hauptsächlich M-M Kabel
(von Breadboard zu Breadboard).
```

### Was bedeuten die Pin-Bezeichnungen am ESP32?

```
Der ESP32 hat viele Pins (die Metallstifte an den Seiten).
Jeder Pin hat einen Namen und eine Funktion:

GPIO = "General Purpose Input/Output"
       = universeller Ein-/Ausgangs-Pin
       Der kann entweder ein Signal LESEN (Input)
       oder ein Signal SENDEN (Output)

Wichtige Pins für uns:
─────────────────────────────────────────────────
3.3V   = Gibt immer 3.3 Volt Strom aus
         (wie der "+"-Pol einer Batterie)

VIN    = Gibt 5 Volt aus (wenn USB angeschlossen)
         (manche Sensoren brauchen mehr Power)

GND    = "Ground" = Masse = Minus-Pol
         (wie der "-"-Pol einer Batterie)
         JEDES Bauteil muss mit GND verbunden sein!

GPIO34 = Kann analoge Werte lesen (0-4095)
         → Hier kommt der Feuchtesensor dran

GPIO35 = Kann analoge Werte lesen (0-4095)
         → Hier kommt der pH-Sensor dran

GPIO21 = Standard-Pin für I2C Daten (SDA)
         → Hier kommt das Display dran

GPIO22 = Standard-Pin für I2C Takt (SCL)
         → Hier kommt das Display dran

GPIO27 = Normaler Ein-/Ausgangs-Pin
         → Hier kommt der Taster dran
```

### Was ist I2C?

```
I2C ist ein "Protokoll" - eine Art wie Bauteile miteinander
reden. Statt für jede Information einen eigenen Draht zu
brauchen, teilen sich alle I2C-Geräte nur 2 Drähte:

SDA = "Serial Data"  = Datenleitung (hier fließen die Infos)
SCL = "Serial Clock"  = Taktleitung (gibt den Rhythmus vor)

Unser OLED-Display spricht I2C. Es braucht also nur
4 Kabel: VCC (Strom), GND (Masse), SDA (Daten), SCL (Takt).
```

### Was ist ein ADC?

```
ADC = "Analog to Digital Converter"
    = Analog-Digital-Wandler

Unsere Sensoren geben eine SPANNUNG aus (z.B. 1.8 Volt).
Der ESP32 kann aber nur mit ZAHLEN rechnen.
Der ADC wandelt die Spannung in eine Zahl um:

  0.0V  →  0       (Minimum)
  3.3V  →  4095    (Maximum, weil 12-bit: 2^12 = 4096)

  Feuchtesensor in trockener Erde: ~2.8V → ~3500
  Feuchtesensor in nasser Erde:    ~1.2V → ~1500
```

### Spannung, Strom, Masse - ganz einfach erklärt

```
Stell dir Wasser in einem Schlauch vor:

SPANNUNG (Volt) = Wasserdruck
  → 3.3V = niedriger Druck (reicht für OLED, Feuchte-Sensor)
  → 5V   = höherer Druck (braucht der pH-Sensor)

STROM (Ampere) = Wassermenge die durchfließt
  → Unsere Bauteile brauchen sehr wenig (Milliampere = mA)
  → Der ESP32 liefert genug für alles

MASSE (GND) = Wo das Wasser hinabfließt
  → JEDES Bauteil muss mit GND verbunden sein
  → Ohne GND fließt kein Strom = nichts funktioniert
  → Es gibt mehrere GND-Pins am ESP32 - alle sind identisch

KURZSCHLUSS = + und - direkt verbinden (OHNE Bauteil dazwischen)
  → Passiert wenn z.B. ein loses Kabel VCC und GND berührt
  → Kann den ESP32 ZERSTÖREN!
  → Deshalb: immer alles prüfen bevor USB anschließen!
```

---

## TEIL B: DEINE BAUTEILE KENNENLERNEN

Wenn das Paket kommt, leg erstmal alles auf den Tisch und
identifiziere jedes Teil. Hier ist was du hast:

### Bauteil 1: ESP32 DevKit (aus dem Freenove Kit)

```
Aussehen:
┌─────────────────────────────────┐
│  [Micro-USB Buchse]             │
│                                 │
│  ┌───────────────────────────┐  │
│  │    ESP32-WROOM-32         │  │  ← Silberne Metallabdeckung
│  │    (Metallkappe mit       │  │     Das ist das "Gehirn"
│  │     Antenne)              │  │
│  └───────────────────────────┘  │
│                                 │
│  [BOOT]              [EN/RST]  │  ← 2 kleine Taster auf der Platine
│                                 │
│  Pin Pin Pin Pin ... Pin Pin   │  ← Metallstifte links
│  Pin Pin Pin Pin ... Pin Pin   │  ← Metallstifte rechts
└─────────────────────────────────┘

Die Beschriftung der Pins steht auf der Platine
(winzig klein - manchmal braucht man eine Lupe!)
Die Pins sind von oben nach unten nummeriert.

Wichtige Tasten auf dem Board:
- BOOT = Zum Programmieren (manchmal gedrückt halten beim Upload)
- EN/RST = Reset (startet den ESP32 neu)
```

### Bauteil 2: OLED Display (AZDelivery 0.96")

```
Aussehen:
┌─────────────────────┐
│                     │
│   ┌─────────────┐   │
│   │  Schwarzer  │   │  ← Das Display selbst (0.96 Zoll)
│   │  Bildschirm │   │     Zeigt weiße Pixel auf schwarz
│   └─────────────┘   │
│                     │
│  GND VCC SCL SDA    │  ← 4 Pins unten (Beschriftung auf Platine!)
│   │   │   │   │     │
└───┘───┘───┘───┘─────┘

ACHTUNG: Die Reihenfolge der Pins kann je nach Hersteller
unterschiedlich sein! Lies die Beschriftung auf DEINEM Display!
Manche haben: VCC GND SCL SDA
Andere haben: GND VCC SDA SCL
→ IMMER nach Beschriftung gehen, NICHT nach Position!
```

### Bauteil 3: Kapazitiver Feuchtesensor (AZDelivery V1.2)

```
Aussehen:
┌──────┐
│Platine│──────────────────────────────┐
│ mit  │  Langer flacher Stab         │
│3 Pins│  (wird in die Erde gesteckt) │
└──┬───┘──────────────────────────────┘
   │
   │  3 Pins oder Kabel:
   │  VCC  = Strom (3.3V)
   │  GND  = Masse
   │  AOUT = Analoges Signal (Messwert)

Wie funktioniert er?
Der Sensor misst, wie gut die Erde elektrische Felder leitet.
Nasse Erde leitet besser → niedrigerer Wert
Trockene Erde leitet schlechter → höherer Wert

WICHTIG: "Kapazitiv" bedeutet, dass der Sensor NICHT rostet!
Billige Sensoren mit 2 freiliegenden Metallstiften rosten
nach wenigen Wochen weg. Deiner hält viel länger.

Markierung auf dem Sensor: Nicht tiefer als die Linie einstecken!
(Die Elektronik oben darf NICHT nass werden)
```

### Bauteil 4: DFRobot pH-Sensor Kit V2

```
Das Kit besteht aus 3 Teilen:

Teil A: pH-Sonde (Glaselektrode)
┌─────┐
│     │  Langer Stab mit Glaskugel an der Spitze
│     │  ⚠️ SEHR EMPFINDLICH! Nicht fallen lassen!
│     │  ⚠️ Glaskugel NICHT mit Fingern berühren!
│ BNC │  Am oberen Ende: BNC-Stecker (Schraubstecker)
└──┬──┘
   │   Kommt mit einer Schutzkappe voller Aufbewahrungslösung
   │   → KAPPE NICHT WEGWERFEN! Sonde nach Gebrauch wieder
   │     reinstellen, sonst trocknet sie aus und ist kaputt!

Teil B: Verstärkerplatine (Signal Board)
┌──────────────────┐
│  [BNC-Buchse]    │  ← Hier schraubst du die Sonde rein
│                  │
│  Kleine Platine  │  Das Signal der pH-Sonde ist extrem
│  mit Chip        │  schwach. Diese Platine verstärkt es,
│                  │  damit der ESP32 es lesen kann.
│  [3-Pin-Stecker] │  ← V+ (rot), GND (schwarz), Signal (blau)
└──────────────────┘
   ⚠️ V+ braucht 5V (NICHT 3.3V)!
   Das Signal kommt trotzdem als 0-3.3V raus (sicher für ESP32)

Teil C: BNC-Kabel
   Verbindet Sonde mit Verstärkerplatine
   (manchmal ist die Sonde schon direkt dran)

Zusammenbau:
1. BNC-Stecker der Sonde in die BNC-Buchse der Platine stecken
2. Leicht im Uhrzeigersinn drehen bis es klickt/einrastet
3. 3-Pin-Kabel an die Platine anschließen
```

### Bauteil 5: Taster (Push-Button)

```
Ein kleiner Knopf aus dem Freenove Kit.
Hat 4 Beinchen (Pins):

    ┌─────┐
  1─┤     ├─2       Pins 1+2 sind IMMER verbunden
    │     │         Pins 3+4 sind IMMER verbunden
  3─┤     ├─4       Wenn du drückst, werden 1+3 verbunden
    └─────┘

Für uns reicht es, 2 diagonal gegenüberliegende Pins zu nutzen.
Einen verbinden wir mit GPIO 27, den anderen mit GND.

Im Normalzustand: keine Verbindung (GPIO 27 = HIGH dank Pull-Up)
Beim Drücken: Verbindung hergestellt (GPIO 27 = LOW = 0)
```

### Bauteil 6: LiPo-Akku + TP4056 Laderegler

```
⚠️ DIESE TEILE BRAUCHST DU ERST IN PHASE 2!
⚠️ Für den Prototyp reicht USB-Strom vom Computer!
⚠️ Leg sie erstmal beiseite.

Der LiPo-Akku ist ein flacher Beutel mit 2 Drähten (rot + schwarz).
⚠️ NIEMALS die Drähte zusammen halten! (= Kurzschluss = Feuer!)
⚠️ In der Verpackung lassen bis Phase 2.
```

---

## TEIL C: SOFTWARE VORBEREITEN

> Mach das VOR dem Hardware-Aufbau, damit du sofort testen
> kannst wenn alles verkabelt ist.

### Schritt 1: VS Code installieren

```
1. Gehe zu: https://code.visualstudio.com/
2. "Download for Mac" klicken
3. Installieren (in den Applications-Ordner ziehen)
4. VS Code öffnen
```

### Schritt 2: PlatformIO installieren

```
1. In VS Code: Links auf das Bauklötze-Symbol klicken (Extensions)
2. Suche: "PlatformIO IDE"
3. "Install" klicken
4. Warten! Die Installation dauert 2-5 Minuten
5. VS Code neu starten wenn aufgefordert
6. Unten links erscheint ein kleines Haus-Symbol (PIO Home)
```

### Schritt 3: SmartSoil-Projekt öffnen

```
1. VS Code: File → Open Folder
2. Navigiere zu: /Users/Jonasweiss/SmartSoil/firmware
3. "Open" klicken
4. PlatformIO erkennt die platformio.ini und lädt automatisch
   alle nötigen Bibliotheken herunter (dauert beim ersten Mal!)
```

### Schritt 4: Überprüfen ob alles da ist

```
In der Seitenleiste solltest du sehen:
firmware/
├── platformio.ini     ← Projekt-Konfiguration
└── src/
    ├── main.cpp       ← Hauptprogramm
    ├── config.h       ← Einstellungen (Pins, Schwellenwerte)
    ├── sensors.h      ← Sensor-Logik
    ├── sensors.cpp    ← (leer, alles in .h)
    ├── display.h      ← Display-Logik
    ├── display.cpp    ← (leer, alles in .h)
    └── webserver.h    ← WiFi Web-Oberfläche
```

---

## TEIL D: AUFBAU - Schritt für Schritt

> Arbeite JEDEN Schritt der Reihe nach ab.
> Schließe das USB-Kabel NOCH NICHT AN!
> Erst wenn alles verkabelt und geprüft ist.

### SCHRITT 1: ESP32 auf das Breadboard setzen

```
Was du brauchst:
- Breadboard (aus dem Freenove Kit)
- ESP32 DevKit Board

Was du tust:
1. Breadboard flach vor dich auf den Tisch legen
   (Die langen Schienen oben und unten, das Nummernraster
   in der Mitte)

2. ESP32 Board nehmen, USB-Buchse zeigt zu dir
   (oder von dir weg - egal, solange du weißt wo oben ist)

3. Die Pins des ESP32 in die Löcher stecken:
   - Die linke Pin-Reihe kommt in Spalte "e"
   - Die rechte Pin-Reihe kommt in Spalte "f"
   - Der ESP32 "reitet" also über die Mittellücke

   So sieht das aus:
   ─────────────────────────────────
   Spalte:  a  b  c  d  e │ f  g  h  i  j
                           │
   Zeile 1: ·  ·  ·  · [ESP32] ·  ·  ·  ·
   Zeile 2: ·  ·  ·  · [ESP32] ·  ·  ·  ·
   Zeile 3: ·  ·  ·  · [ESP32] ·  ·  ·  ·
   ...usw...

4. SANFT aber bestimmt nach unten drücken
   Wenn es sehr schwer geht: NICHT mit Gewalt!
   Prüfe ob die Pins gerade in den Löchern sitzen.

5. Jetzt kannst du auf der linken Seite (Spalte a-d)
   und auf der rechten Seite (Spalte g-j) Kabel
   anschließen, die automatisch mit dem jeweiligen
   ESP32-Pin verbunden sind.
```

### SCHRITT 2: Pin-Positionen am ESP32 finden

```
WICHTIG: Schau dir DEIN ESP32 Board genau an!

Die Pin-Beschriftungen stehen auf der Platine.
Such dir diese Pins und MARKIERE sie dir (z.B. mit einem
kleinen Aufkleber oder schreib dir die Zeilen-Nummern auf):

Linke Seite (Spalte e):          Rechte Seite (Spalte f):
─────────────────────            ─────────────────────
  3V3 (= 3.3 Volt)               VIN (= 5 Volt Input)
  GND                            GND
  GPIO 34                        GPIO 21
  GPIO 35                        GPIO 22
  GPIO 32                        GPIO 27
  GPIO 2 (Onboard LED)

Tipp: Im Freenove Kit ist oft ein Pinout-Diagramm dabei
(ein Zettel oder in der PDF-Anleitung). Das hilft enorm!

Wenn du unsicher bist, google: "ESP32 DevKit Pinout"
→ Dort siehst du genau welcher Pin wo sitzt.
```

### SCHRITT 3: Stromschienen vorbereiten

```
Was du brauchst:
- 2x rotes Jumper-Kabel (Male-Male)
- 2x schwarzes Jumper-Kabel (Male-Male)

Was du tust:

1. ROTES Kabel: ESP32 Pin "3V3" → Obere Stromschiene "+"
   (Steck ein Ende in ein Loch derselben Zeile wie der
    3V3-Pin, Spalte a-d, und das andere Ende in die
    "+"-Schiene oben)

2. SCHWARZES Kabel: ESP32 Pin "GND" → Obere Masseschiene "-"
   (Gleich wie oben, nur mit einem GND-Pin)

3. ROTES Kabel: Obere "+" → Untere "+"
   (Damit beide Seiten des Breadboards 3.3V haben)

4. SCHWARZES Kabel: Obere "-" → Untere "-"
   (Damit beide Seiten des Breadboards GND haben)

Ergebnis:
┌──────────────────────────────────────────┐
│  + + + + + + + + + + + + + + + + + + +  │ ← 3.3V überall
│  - - - - - - - - - - - - - - - - - - -  │ ← GND überall
│            [ESP32]                       │
│  - - - - - - - - - - - - - - - - - - -  │ ← GND überall
│  + + + + + + + + + + + + + + + + + + +  │ ← 3.3V überall
└──────────────────────────────────────────┘

TIPP: Verwende IMMER rot für Strom (+) und schwarz
für Masse (-). So behältst du den Überblick!
```

### SCHRITT 4: OLED Display anschließen (4 Kabel)

```
Was du brauchst:
- OLED Display
- 4x Jumper-Kabel (Male-Female, weil das Display
  wahrscheinlich Pin-Header hat)
  ODER Male-Male wenn du das Display ins Breadboard steckst

ZUERST: Lies die Beschriftung auf DEINEM Display!
Die Pin-Reihenfolge variiert je nach Hersteller!

Typische Varianten:
  Variante A: GND  VCC  SCL  SDA
  Variante B: VCC  GND  SCL  SDA
  Variante C: SDA  SCL  VCC  GND
→ Folge der BESCHRIFTUNG, nicht der Position!

Verbinde nach BESCHRIFTUNG:
──────────────────────────────────────────────────
Display-Pin    →  Wohin                 Kabelfarbe
──────────────────────────────────────────────────
VCC            →  "+" Stromschiene      ROT
               (3.3V - NICHT 5V!)

GND            →  "-" Masseschiene      SCHWARZ

SDA            →  ESP32 GPIO 21         BLAU
               (In ein freies Loch derselben
                Zeile wie GPIO 21, Spalte g-j)

SCL            →  ESP32 GPIO 22         GELB
               (In ein freies Loch derselben
                Zeile wie GPIO 22, Spalte g-j)
──────────────────────────────────────────────────

⚠️ WARNUNG: NIEMALS 5V an das OLED-Display!
   Der SSD1306 Chip verträgt nur 3.3V.
   5V kann ihn dauerhaft zerstören!

Wenn das Display 4 Pin-Header hat, kannst du es auch
direkt ins Breadboard stecken (eine freie Stelle suchen)
und dann mit Kabeln zum ESP32 verbinden.
```

### SCHRITT 5: Feuchtesensor anschließen (3 Kabel)

```
Was du brauchst:
- Kapazitiver Feuchtesensor V1.2
- 3x Jumper-Kabel

Der Sensor hat 3 Pins/Drähte:
──────────────────────────────────────────────────
Sensor-Pin     →  Wohin                 Kabelfarbe
──────────────────────────────────────────────────
VCC            →  "+" Stromschiene      ROT
                  (3.3V)

GND            →  "-" Masseschiene      SCHWARZ

AOUT           →  ESP32 GPIO 34         GRÜN
               (In ein freies Loch derselben
                Zeile wie GPIO 34)
──────────────────────────────────────────────────

Der Sensor-Stab kommt SPÄTER in die Erde.
Für den ersten Test: einfach in ein Glas Wasser tauchen
oder in der Luft lassen.

⚠️ WICHTIG: Der Sensor hat eine Markierung/Linie!
   Nicht tiefer als diese Linie einstecken!
   Die Elektronik oberhalb der Linie darf NIE nass werden!
```

### SCHRITT 6: pH-Sensor anschließen (3 Kabel + Sonde)

```
Was du brauchst:
- DFRobot pH Verstärkerplatine
- pH-Sonde (Glaselektrode)
- 3x Jumper-Kabel

ERST die Sonde an die Verstärkerplatine anschließen:
1. BNC-Stecker der Sonde nehmen
2. In die BNC-Buchse der Platine stecken
3. Leicht im Uhrzeigersinn drehen → klickt ein

DANN die Verstärkerplatine mit dem ESP32 verbinden:
──────────────────────────────────────────────────
Platinen-Pin   →  Wohin                 Kabelfarbe
──────────────────────────────────────────────────
V+  (rot)      →  ESP32 Pin "VIN"       ROT
                  ⚠️ Hier MUSS 5V hin!
                  VIN gibt 5V wenn USB angeschlossen

GND (schwarz)  →  "-" Masseschiene      SCHWARZ

Signal (blau)  →  ESP32 GPIO 35         WEISS
               (In ein freies Loch derselben
                Zeile wie GPIO 35)
──────────────────────────────────────────────────

⚠️ WARUM 5V? Der Verstärker-Chip auf der Platine braucht
   5V um korrekt zu arbeiten. Sein Ausgangssignal ist
   aber trotzdem nur 0-3V → sicher für den ESP32 ADC.

⚠️ pH-Sonde PFLEGE:
   - Schutzkappe mit Lösung AUFBEWAHREN
   - Sonde nach Gebrauch immer wieder reinstellen
   - Glaskugel NICHT mit Fingern anfassen
   - NICHT trocken lagern (trocknet aus = kaputt)
   - NICHT fallen lassen (Glas!)
```

### SCHRITT 7: Taster anschließen (2 Kabel)

```
Was du brauchst:
- 1x Push-Button (Taster) aus dem Freenove Kit
- 2x Jumper-Kabel (oder den Taster direkt ins Breadboard)

So sieht ein typischer Taster aus (von oben):
    ┌─────────┐
    │  1   2  │    Die 4 Beinchen gehen ins Breadboard.
    │  [BTN]  │    1-2 sind intern verbunden.
    │  3   4  │    3-4 sind intern verbunden.
    └─────────┘    Drücken verbindet 1→3 und 2→4.

Aufbau:
1. Taster in eine freie Stelle des Breadboards stecken
   → Er soll ÜBER der Mittellücke sitzen!
   → Pin 1+3 auf der linken Seite, Pin 2+4 auf der rechten

2. Ein Kabel von einem Pin des Tasters (z.B. Pin 1)
   nach GPIO 27 am ESP32

3. Ein Kabel vom gegenüberliegenden Pin (z.B. Pin 3)
   zur "-" Masseschiene (GND)

──────────────────────────────────────────────────
Taster-Pin     →  Wohin                 Kabelfarbe
──────────────────────────────────────────────────
Pin 1 (links)  →  ESP32 GPIO 27         ORANGE

Pin 3 (rechts) →  "-" Masseschiene      SCHWARZ
──────────────────────────────────────────────────

Kein Widerstand nötig! Der ESP32 hat einen eingebauten
"Pull-Up-Widerstand" der im Code aktiviert wird.

Bedienung:
- Kurz drücken (< 3 Sek.)  = Bestätigen
- Lang drücken (> 3 Sek.)  = Kalibrierung starten/stoppen
  (Die blaue LED auf dem ESP32 blinkt als Bestätigung)
```

---

## TEIL E: KONTROLLE VOR DEM EINSCHALTEN

> STOPP! Bevor du das USB-Kabel anschließt, geh diese
> Checkliste Punkt für Punkt durch!

```
CHECKLISTE (jeden Punkt einzeln abhaken):

□ 1. Liegt das Breadboard FLACH und STABIL auf dem Tisch?

□ 2. ESP32 sitzt fest im Breadboard?
     (nicht schief, keine Pins verbogen)

□ 3. OLED Display:
     □ VCC → 3.3V Stromschiene (NICHT 5V!)
     □ GND → Masseschiene
     □ SDA → GPIO 21
     □ SCL → GPIO 22

□ 4. Feuchtesensor:
     □ VCC → 3.3V Stromschiene
     □ GND → Masseschiene
     □ AOUT → GPIO 34

□ 5. pH-Sensor Verstärkerplatine:
     □ Sonde in BNC-Buchse eingeschraubt?
     □ V+ → VIN (5V)
     □ GND → Masseschiene
     □ Signal → GPIO 35
     □ pH-Sonde ist in Flüssigkeit! (Pufferlösung oder
       Schutzkappenflüssigkeit) → NIEMALS trocken einschalten!

□ 6. Taster:
     □ Ein Pin → GPIO 27
     □ Anderer Pin → GND Masseschiene

□ 7. KEINE losen Kabel die rumhängen und etwas berühren könnten

□ 8. KEINE Kabel die direkt "+" mit "-" verbinden
     (= Kurzschluss!)

□ 9. USB-Kabel bereitlegen (Micro-USB, muss ein Datenkabel sein,
     NICHT nur ein Ladekabel! Datenkabel sind meist etwas dicker)
```

---

## TEIL F: DER ERSTE START!

### F.1 USB anschließen und Code hochladen

```
1. USB-Kabel an den ESP32 anschließen und an deinen Mac

2. Prüfe: Leuchtet eine kleine LED auf dem ESP32?
   → JA: Super, er hat Strom!
   → NEIN: USB-Kabel tauschen (vielleicht nur ein Ladekabel)

3. Öffne VS Code mit dem SmartSoil Firmware-Projekt

4. Unten in der blauen Leiste siehst du:
   [→] (Upload)  [✓] (Build)  [🔌] (Monitor)

5. Klicke auf [✓] (Build / Häkchen)
   → PlatformIO kompiliert den Code
   → Warte bis unten steht: "SUCCESS"
   → Bei Fehlern: Screenshot machen und mir zeigen!

6. Klicke auf [→] (Upload / Pfeil)
   → Der Code wird auf den ESP32 geladen
   → Wenn "Connecting..." hängt:
     → Halte die BOOT-Taste am ESP32 gedrückt
     → Klicke nochmal Upload
     → Lasse BOOT los wenn "Uploading" erscheint

7. Klicke auf [🔌] (Serial Monitor)
   → Du siehst jetzt die Ausgabe des ESP32!
```

### F.2 Was du sehen solltest

```
Im Serial Monitor:

=========================
  SmartSoil Prototyp v1
=========================
[OK] Sensoren
[OK] Display
[OK] WiFi AP + Webserver

SSID: SmartSoil / Passwort: smartsoil123
Webinterface: http://192.168.4.1

Taster: kurz = Aktion | lang (3s) = Kalibrierung
-------------------------------------------------
Feuchte:  XX.X% (raw: XXXX) | pH:  X.X (X.XXXV) | Akku: XX% (X.XXV)
Feuchte:  XX.X% (raw: XXXX) | pH:  X.X (X.XXXV) | Akku: XX% (X.XXV)
...

Die Werte kommen alle 5 Sekunden.
Am Anfang sind sie ungenau (noch nicht kalibriert) - das ist normal!
```

### F.3 Was du auf dem OLED Display sehen solltest

```
┌──────────────────────┐
│ SmartSoil      [▓▓░░]│  ← Batterie-Icon (rechts oben)
│──────────────────────│
│ Feuchte:     XX.X%   │
│ [████████░░░░░░░░░░] │  ← Feuchte-Balken
│                      │
│ pH-Wert:       X.X   │
│ [░░░░░[██]░░░░░░░░░] │  ← pH-Balken (Bereich 6-7 markiert)
│                ▲      │  ← Zeiger für aktuellen pH
│                      │
│ [OK] Alles optimal!  │  ← oder "Giessen!" etc.
└──────────────────────┘
```

### F.4 WiFi Web-Interface testen

```
1. Nimm dein Handy (oder ein zweites Gerät)

2. Geh in die WiFi-Einstellungen

3. Suche das Netzwerk: "SmartSoil"
   → Wenn es NICHT auftaucht: ESP32 neustarten (Reset-Taste
     oder USB kurz rausziehen und wieder reinstecken)

4. Verbinde dich mit Passwort: smartsoil123
   → Dein Handy sagt vielleicht "Kein Internet" - NORMAL!
     Der ESP32 ist ja kein Internet-Router, sondern zeigt
     nur seine eigene Webseite.

5. Öffne den Browser und tippe ein: http://192.168.4.1

6. Du siehst eine dunkle Webseite mit:
   - Feuchte in % mit Balken (grün)
   - pH-Wert mit Balken (rot)
   - Status-Anzeige ("Alles optimal!" etc.)
   → Die Werte aktualisieren sich alle 5 Sekunden automatisch
```

---

## TEIL G: KALIBRIERUNG

> Die Sensoren messen jetzt, aber die Werte sind noch
> ungenau. Kalibrierung = den Sensoren beibringen, was
> "pH 7" und "pH 4" bzw. "trocken" und "nass" bedeutet.

### G.1 pH-Sensor kalibrieren

```
Was du brauchst:
- pH 7.0 Pufferlösung (bestellt)
- pH 4.0 Pufferlösung (bestellt)
- Ein kleines Glas/Becher
- Destilliertes Wasser zum Abspülen (zur Not Leitungswasser)
- Küchenpapier zum Abtupfen

Ablauf:

1. pH-Sonde aus der Schutzkappe nehmen
   → Schutzlösung in der Kappe BEHALTEN!

2. Sonde VORSICHTIG mit dest. Wasser abspülen
   → Mit Küchenpapier sanft abtupfen (nicht reiben!)

3. Etwas pH 7.0 Pufferlösung in ein Glas gießen
   → Genug damit die Glaskugel der Sonde komplett
     eingetaucht ist (ca. 2-3 cm)

4. Sonde in die pH 7.0 Lösung tauchen

5. Warte 1-2 MINUTEN
   → Beobachte den Spannungswert im Serial Monitor
   → Er pendelt sich langsam ein
   → Erst weitermachen wenn er STABIL ist
     (schwankt nur noch um ±0.005V)

6. TASTER am ESP32 LANG drücken (3 Sekunden halten)
   → Blaue LED blinkt
   → Display zeigt: "pH KALIBRIERUNG"
   → Display zeigt: "Sonde in pH 7.0 Pufferloesung tauchen"
   → Display zeigt die aktuelle Spannung

7. Wenn Spannung stabil → TASTER KURZ drücken
   → Serial Monitor zeigt: "pH 7.0 kalibriert: X.XXXV"
   → Display zeigt: "Jetzt pH 4.0"

8. Sonde aus pH 7 nehmen und abspülen

9. Etwas pH 4.0 Pufferlösung in ein ANDERES Glas gießen
   (oder das Glas ausspülen)

10. Sonde in pH 4.0 Lösung tauchen

11. Wieder 1-2 Minuten warten bis Wert stabil

12. TASTER KURZ drücken
    → Serial Monitor zeigt: "pH 4.0 kalibriert: X.XXXV"
    → Display zeigt: "pH Kalibrierung gespeichert!"

13. Jetzt kommt automatisch die Feuchtekalibrierung
    (nächster Abschnitt)

14. pH-Sonde abspülen und zurück in die Schutzkappe!
```

### G.2 Feuchtesensor kalibrieren

```
Direkt nach der pH-Kalibrierung zeigt das Display:
"FEUCHTE KALIBR."

Ablauf:

1. Display zeigt: "Sensor in TROCKENE Erde stecken"
   → Nimm den Feuchtesensor
   → Steck ihn in trockene Blumenerde
     (oder lass ihn in der Luft - auch OK für den Anfang)
   → Warte 10 Sekunden

2. TASTER KURZ drücken
   → Serial Monitor zeigt: "Trocken kalibriert: XXXX"

3. Display zeigt: "Sensor in NASSE Erde stecken"
   → Steck den Sensor in nasse Erde
     (oder halte ihn in ein Glas Wasser - nur bis zur Linie!)
   → Warte 10 Sekunden

4. TASTER KURZ drücken
   → Serial Monitor zeigt: "Nass kalibriert: XXXX"
   → Display zeigt: "Feuchte Kalibrierung gespeichert!"

5. Zurück zum Hauptbildschirm → Werte sollten jetzt
   realistisch aussehen!

Die Kalibrierung wird im EEPROM gespeichert.
= Sie bleibt auch nach einem Neustart erhalten!
= Du musst das nicht jedes Mal neu machen.
```

---

## TEIL H: TROUBLESHOOTING (Wenn etwas nicht funktioniert)

### Problem: ESP32 wird am Computer nicht erkannt

```
Symptom: Kein Port in PlatformIO sichtbar, Upload geht nicht

Lösungen (der Reihe nach probieren):
1. Anderes USB-Kabel verwenden!
   → Sehr häufigster Fehler!
   → Viele USB-Kabel sind NUR Ladekabel (keine Datenleitungen)
   → Teste mit dem Kabel aus dem Freenove Kit

2. Anderen USB-Port am Mac probieren

3. Mac neustarten

4. Treiber installieren:
   → Manche ESP32 brauchen den CP2102 oder CH340 Treiber
   → Google: "CP2102 driver Mac" oder "CH340 driver Mac"
```

### Problem: Upload bleibt bei "Connecting..." hängen

```
Lösung:
1. BOOT-Taste am ESP32 GEDRÜCKT halten
2. Kurz die EN/RST-Taste drücken (und loslassen)
3. BOOT-Taste weiter halten
4. Upload in PlatformIO starten
5. Wenn "Uploading..." erscheint: BOOT-Taste loslassen
```

### Problem: Display bleibt schwarz

```
Mögliche Ursachen:

1. VCC und GND vertauscht?
   → Sofort USB trennen! Kabel prüfen.

2. SDA und SCL vertauscht?
   → Tausche die beiden Kabel (Blau und Gelb)

3. Falsche I2C-Adresse?
   → Die meisten SSD1306 haben Adresse 0x3C
   → Manche haben 0x3D
   → Im Serial Monitor steht "[FEHLER] OLED nicht gefunden!"
   → In config.h ändern: #define OLED_ADDR 0x3D

4. Kabel sitzt nicht richtig im Breadboard?
   → Jedes Kabel rausziehen und nochmal fest reinstecken

5. Display defekt?
   → Sehr selten, aber möglich bei Billigdisplays
```

### Problem: Feuchtesensor zeigt immer 0% oder 100%

```
1. Kabel prüfen: AOUT wirklich an GPIO 34?
2. VCC wirklich an 3.3V (nicht 5V)?
3. Im Serial Monitor die Raw-Werte anschauen:
   → "raw: 0" = Kabelbruch oder falscher Pin
   → "raw: 4095" = Kurzschluss oder falscher Pin
   → "raw: 2000-3000" = Sensor funktioniert, Kalibrierung nötig
4. Sensor nochmal kalibrieren (Taster 3 Sek.)
```

### Problem: pH-Werte springen wild herum

```
1. GND-Verbindung prüfen!
   → Der pH-Sensor ist SEHR empfindlich
   → Eine schlechte Masseverbindung verursacht Rauschen

2. Kabel so kurz wie möglich halten
   → Lange Kabel fangen Störsignale ein

3. pH-Sonde wirklich in Flüssigkeit?
   → Trockene Messung = Unsinn-Werte

4. Sonde abgelaufen oder ausgetrocknet?
   → In Aufbewahrungslösung über Nacht einlegen
   → Dann neu kalibrieren
```

### Problem: WiFi "SmartSoil" taucht nicht auf

```
1. ESP32 neustarten (Reset-Taste oder USB rein/raus)
2. Serial Monitor prüfen:
   → Steht "[OK] WiFi AP + Webserver"?
   → Steht eine IP-Adresse da?
3. Mit dem Handy näher an den ESP32 gehen (< 5 Meter)
4. Handy WiFi aus und wieder an
```

---

## TEIL I: GESAMTÜBERSICHT VERKABELUNG

```
FINALE ÜBERSICHT: Jedes einzelne Kabel das du brauchst

Nr.  Von                  →  Nach              Farbe    Volt
───────────────────────────────────────────────────────────────
 1   ESP32 3V3            →  Strom (+) oben    ROT      3.3V
 2   ESP32 GND            →  Masse (-) oben    SCHWARZ  GND
 3   Strom (+) oben       →  Strom (+) unten   ROT      3.3V
 4   Masse (-) oben       →  Masse (-) unten   SCHWARZ  GND
 5   OLED VCC             →  Strom (+)         ROT      3.3V
 6   OLED GND             →  Masse (-)         SCHWARZ  GND
 7   OLED SDA             →  ESP32 GPIO 21     BLAU     Signal
 8   OLED SCL             →  ESP32 GPIO 22     GELB     Signal
 9   Feuchte VCC          →  Strom (+)         ROT      3.3V
10   Feuchte GND          →  Masse (-)         SCHWARZ  GND
11   Feuchte AOUT         →  ESP32 GPIO 34     GRÜN     Signal
12   pH V+ (rot)          →  ESP32 VIN         ROT      5V !!
13   pH GND (schwarz)     →  Masse (-)         SCHWARZ  GND
14   pH Signal (blau)     →  ESP32 GPIO 35     WEISS    Signal
15   Taster Pin 1         →  ESP32 GPIO 27     ORANGE   Signal
16   Taster Pin 3         →  Masse (-)         SCHWARZ  GND
───────────────────────────────────────────────────────────────
TOTAL: 16 Kabel

Merke:
- ROT    = Strom (3.3V oder 5V)
- SCHWARZ = Masse (GND)
- Andere Farben = Signal-Kabel (Daten)
```

---

## TEIL J: WAS KOMMT DANACH?

```
Wenn alles läuft und kalibriert ist:

Phase 2: Batteriebetrieb
   → TP4056 Laderegler + LiPo Akku anschließen
   → Deep Sleep aktivieren (5 Min schlafen, aufwachen, messen)
   → Dann hält der Akku Wochen!

Phase 3: Gehäuse
   → 3D-Druck oder wasserdichte Kunststoffbox
   → Nur Sensoren und USB-Buchse schauen raus

Phase 4: App
   → Mobile App für iOS/Android
   → Benachrichtigung: "Deine Monstera braucht Wasser!"
   → Verlaufsgraphen: pH und Feuchte über Wochen

Phase 5: Pflanzen-Datenbank
   → Optimale pH- und Feuchte-Bereiche pro Pflanze
   → "Monstera mag pH 5.5-6.5 und 40-60% Feuchte"
   → Automatische Empfehlung: "Düngen mit Eisendünger!"
```

---

## Anhang: Glossar

```
ADC        = Analog-Digital-Converter (wandelt Spannung in Zahlen)
BLE        = Bluetooth Low Energy (energiesparendes Bluetooth)
BNC        = Bayonet Neill-Concelman (Schraubstecker für pH-Sonde)
Breadboard = Steckbrett (zum Prototypen ohne Löten)
EEPROM     = Speicher der Daten behält wenn Strom weg ist
ESP32      = Unser Mikrocontroller (WiFi + Bluetooth eingebaut)
GND        = Ground = Masse = Minus-Pol
GPIO       = General Purpose Input/Output (universeller Pin)
I2C        = Inter-Integrated Circuit (Kommunikationsprotokoll)
Jumper     = Steckkabel für Breadboard
LiPo       = Lithium-Polymer (Akku-Typ, wiederaufladbar)
OLED       = Organic LED Display (selbstleuchtend, kein Backlight)
Pin        = Metallstift an einem Bauteil
Pull-Up    = Interner Widerstand der einen Pin auf HIGH zieht
SDA        = Serial Data (I2C Datenleitung)
SCL        = Serial Clock (I2C Taktleitung)
SSD1306    = Display-Chip auf dem OLED
TP4056     = LiPo-Lade-Chip auf der Laderegler-Platine
VCC/VIN    = Versorgungsspannung (Strom-Eingang)
```

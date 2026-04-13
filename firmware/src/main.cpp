/**
 * SmartSoil – Haupt-Firmware
 *
 * Modus 1 (Timer-Wake): Sensor lesen → OLED 4s → schlafen
 * Modus 2 (Button-Wake): WiFi an → Dashboard im Browser → Auto-Sleep nach 15min
 *
 * Board: WaveShare ESP32-S3 Zero
 */

#include <Arduino.h>
#include "config.h"    // ← muss zuerst kommen (defines HAS_NEOPIXEL, MY_ADC_ATTEN)

#include <WiFi.h>
#include <EEPROM.h>
#include <LittleFS.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WebServer.h>
#include <time.h>
#if HAS_NEOPIXEL
  #include <Adafruit_NeoPixel.h>
#endif
#include "plant.h"
#include "storage.h"
#include "display_mgr.h"
#include "dashboard.h"

// ── RTC Memory (überlebt Deep Sleep) ─────────────────────────
RTC_DATA_ATTR float    rtcLastMoisture  = -1.0f;  // -1 = noch keine Messung
RTC_DATA_ATTR uint32_t rtcWakeCount     = 0;
RTC_DATA_ATTR uint32_t rtcLastWaterTime = 0;
RTC_DATA_ATTR int      rtcStreakDays    = 0;
RTC_DATA_ATTR uint32_t rtcLastStreakDay = 0;
RTC_DATA_ATTR int      rtcPlantLevel   = 1;
RTC_DATA_ATTR uint32_t rtcApproxTime   = 0;       // Unix-Timestamp (via NTP gesetzt)

// ── Globale Instanzen ─────────────────────────────────────────
Adafruit_SSD1306  display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);
WebServer         server(80);
int               calDry = CAL_DRY_DEFAULT;
int               calWet = CAL_WET_DEFAULT;
#if HAS_NEOPIXEL
  Adafruit_NeoPixel pixel(1, PIN_LED_WS2812, NEO_GRB + NEO_KHZ800);
#endif

// ═════════════════════════════════════════════════════════════
// KALIBRIERUNG
// ═════════════════════════════════════════════════════════════
void loadCalibration() {
    EEPROM.begin(EEPROM_SIZE);
    if (EEPROM.read(EEPROM_ADDR_MAGIC) == EEPROM_MAGIC_BYTE) {
        EEPROM.get(EEPROM_CAL_BASE,     calDry);
        EEPROM.get(EEPROM_CAL_BASE + 4, calWet);
        if (calDry < 1000 || calDry > 4095) calDry = CAL_DRY_DEFAULT;
        if (calWet  < 100  || calWet  > 3000) calWet = CAL_WET_DEFAULT;
    }
    EEPROM.end();
}

// ═════════════════════════════════════════════════════════════
// SENSOR
// ═════════════════════════════════════════════════════════════
int readMedian() {
    const int n = SAMPLES_PER_READING;
    int vals[n];
    for (int i = 0; i < n; i++) {
        vals[i] = analogRead(SENSOR_PIN);
        delayMicroseconds(500);
    }
    // Insertion Sort
    for (int i = 1; i < n; i++) {
        int key = vals[i], j = i - 1;
        while (j >= 0 && vals[j] > key) { vals[j + 1] = vals[j]; j--; }
        vals[j + 1] = key;
    }
    return vals[n / 2];
}

float rawToPercent(int raw) {
    float pct = (float)(calDry - raw) / (float)(calDry - calWet) * 100.0f;
    return constrain(pct, 0.0f, 100.0f);
}

// ═════════════════════════════════════════════════════════════
// ZEIT
// ═════════════════════════════════════════════════════════════
uint32_t getApproxTime() {
    // Wenn NTP-Zeit bekannt: hochrechnen via Sleep-Zyklen
    if (rtcApproxTime > 0) {
        return rtcApproxTime + (rtcWakeCount * (uint32_t)SLEEP_MINUTES * 60);
    }
    return 0;
}

bool syncNTP() {
    configTime(3600, 3600, "pool.ntp.org", "time.nist.gov");  // UTC+1 + Sommerzeit
    struct tm ti;
    if (!getLocalTime(&ti, 6000)) return false;
    rtcApproxTime = mktime(&ti) - (rtcWakeCount * (uint32_t)SLEEP_MINUTES * 60);
    return true;
}

// ═════════════════════════════════════════════════════════════
// LED STATUS
// ═════════════════════════════════════════════════════════════
void pixelSet(uint8_t r, uint8_t g, uint8_t b) {
#if HAS_NEOPIXEL
    pixel.begin();
    pixel.setBrightness(40);
    pixel.setPixelColor(0, pixel.Color(r, g, b));
    pixel.show();
#else
    // Freenove: einfache LED — an wenn irgendeine Farbe
    pinMode(PIN_LED_BUILTIN, OUTPUT);
    digitalWrite(PIN_LED_BUILTIN, (r || g || b) ? HIGH : LOW);
#endif
}

void pixelOff() {
#if HAS_NEOPIXEL
    pixel.begin();
    pixel.setPixelColor(0, 0);
    pixel.show();
#else
    pinMode(PIN_LED_BUILTIN, OUTPUT);
    digitalWrite(PIN_LED_BUILTIN, LOW);
#endif
}

void pixelFromPersonality(const PlantPersonality& p) {
    pixelSet(p.r, p.g, p.b);
}

// ═════════════════════════════════════════════════════════════
// GAMIFICATION
// ═════════════════════════════════════════════════════════════
void updateGameState(float moisture) {
    uint32_t now = getApproxTime();
    if (now == 0) return;

    uint32_t todayDay = now / 86400;

    bool optimal = (moisture >= THRESH_CRITICAL_DRY && moisture < THRESH_WET);

    if (todayDay > rtcLastStreakDay) {
        rtcLastStreakDay = todayDay;
        if (optimal) {
            rtcStreakDays++;
        } else if (moisture < THRESH_CRITICAL_DRY) {
            rtcStreakDays = 0;  // Streak bricht nur bei kritisch trocken ab
        }
        // Level steigt alle LEVEL_UP_STREAK_DAYS Streak-Tage, max Level 10
        if (rtcStreakDays > 0 && rtcStreakDays % LEVEL_UP_STREAK_DAYS == 0) {
            if (rtcPlantLevel < 10) rtcPlantLevel++;
        }
    }
}

// ═════════════════════════════════════════════════════════════
// BATTERIE
// ═════════════════════════════════════════════════════════════
float readBatteryVolt() {
#if PIN_BAT_ADC < 0
    return -1.0f;  // Nicht verfügbar (Freenove)
#else
    analogReadResolution(12);
    analogSetAttenuation(MY_ADC_ATTEN);
    int raw = 0;
    for (int i = 0; i < 10; i++) {
        raw += analogRead(PIN_BAT_ADC);
        delayMicroseconds(200);
    }
    raw /= 10;
    float adcVolt  = (raw / 4095.0f) * BAT_ADC_VREF;
    return adcVolt * BAT_DIVIDER_RATIO;
#endif
}

int batteryPercent(float volt) {
    if (volt < 0) return -1;
    float pct = (volt - BAT_VOLT_EMPTY) / (BAT_VOLT_FULL - BAT_VOLT_EMPTY) * 100.0f;
    return (int)constrain(pct, 0.0f, 100.0f);
}

// ═════════════════════════════════════════════════════════════
// DISPLAY INIT
// ═════════════════════════════════════════════════════════════
bool initDisplay() {
    Wire.begin(PIN_SDA, PIN_SCL);
    return display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
}

// ═════════════════════════════════════════════════════════════
// MODUS 1: SLEEP-ZYKLUS
// ═════════════════════════════════════════════════════════════
void runSleepCycle() {
    rtcWakeCount++;

    // Sensor lesen
    loadCalibration();
    analogReadResolution(12);
    analogSetAttenuation(MY_ADC_ATTEN);
    int   raw      = readMedian();
    float moisture = rawToPercent(raw);

    // Batterie lesen
    float batVolt  = readBatteryVolt();
    int   batPct   = batteryPercent(batVolt);

    Serial.printf("[%s] Feuchte: %.1f%% (ADC:%d) | Akku: %.2fV (%d%%) | Wake#%lu\n",
                  DEVICE_ID, moisture, raw, batVolt, batPct, rtcWakeCount);

    // Gieß-Erkennung
    bool watered = false;
    float prevMoisture = rtcLastMoisture;

    if (rtcLastMoisture >= 0.0f) {
        float delta = moisture - rtcLastMoisture;
        if (delta >= WATERING_DELTA) {
            watered = true;
            rtcLastWaterTime = getApproxTime();
            Serial.printf("[Giessen] %.1f%% -> %.1f%%\n", prevMoisture, moisture);
        }
    }

    // Spielstand aktualisieren
    updateGameState(moisture);

    // NeoPixel Statusfarbe (Akku-Warnung überschreibt Pflanzenfarbe)
    PlantPersonality p = getPersonality(moisture);
    if (batPct >= 0 && batPct <= BAT_WARN_PERCENT) {
        // Akku kritisch: rot blinken
        for (int i = 0; i < 3; i++) {
            pixelSet(200, 0, 0); delay(200);
            pixelOff();          delay(200);
        }
    }
    pixelFromPersonality(p);

    // OLED
    if (initDisplay()) {
        if (watered) {
            showWateringScreen(display, prevMoisture, moisture);
            delay(2500);
        }
        showReadingScreen(display, PLANT_NAME, moisture, p, rtcPlantLevel, rtcStreakDays);
        // Batterieanzeige: letzte Zeile überschreiben wenn Warnung
        if (batPct >= 0) {
            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(0, 56);
            if (batPct <= BAT_WARN_PERCENT) {
                display.printf("! Akku: %d%%", batPct);
            } else {
                display.printf("Akku: %d%%", batPct);
            }
            display.display();
        }
        delay(OLED_ON_SECONDS * 1000);
        display.clearDisplay();
        display.display();
        display.ssd1306_command(SSD1306_DISPLAYOFF);  // Display aus = Strom sparen
    }

    // In LittleFS speichern
    if (LittleFS.begin(true)) {
        uint32_t now = getApproxTime();
        appendReading(now, moisture, raw, watered);
        if (watered) logWateringEvent(now, prevMoisture, moisture);
        trimReadingsIfNeeded();
        LittleFS.end();
    }

    // RTC Memory aktualisieren
    rtcLastMoisture = moisture;

    pixelOff();
    Serial.printf("[Sleep] %d Minuten\n", SLEEP_MINUTES);
    Serial.flush();

    // Sleep: Timer + Button als Wakeup-Quellen
    esp_sleep_enable_timer_wakeup((uint64_t)SLEEP_MINUTES * 60ULL * 1000000ULL);
    esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_BUTTON, LOW);
    esp_deep_sleep_start();
}

// ═════════════════════════════════════════════════════════════
// MODUS 2: WIFI AKTIV
// ═════════════════════════════════════════════════════════════
void runWiFiMode() {
    Serial.println("[WiFi] Modus gestartet");

    // OLED: Verbindet...
    if (initDisplay()) showConnectingScreen(display);

    // NeoPixel: Blau = verbindet
    pixelSet(0, 0, 100);

    // WiFi verbinden
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    bool connected = false;
    for (int i = 0; i < WIFI_TIMEOUT_SEC * 2; i++) {
        if (WiFi.status() == WL_CONNECTED) { connected = true; break; }
        delay(500);
    }

    if (!connected) {
        Serial.println("[WiFi] Verbindung fehlgeschlagen");
        if (display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
            showWiFiErrorScreen(display);
        }
        pixelSet(200, 0, 0);
        delay(3000);
        goto sleep;
    }

    {
        // NTP synchronisieren
        if (syncNTP()) {
            Serial.println("[NTP] Synchronisiert");
        }

        // Sensor lesen
        loadCalibration();
        analogReadResolution(12);
        analogSetAttenuation(MY_ADC_ATTEN);

        // Einmalige Messung für OLED + NeoPixel-Farbe beim Start
        int   raw      = readMedian();
        float moisture = rawToPercent(raw);
        PlantPersonality p = getPersonality(moisture);
        rtcLastMoisture = moisture;

        pixelFromPersonality(p);

        // OLED: IP-Adresse anzeigen
        String ip = WiFi.localIP().toString();
        if (display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
            showWiFiActiveScreen(display, ip, moisture);
        }

        Serial.printf("[WiFi] IP: %s\n", ip.c_str());

        // LittleFS für Web-Handler
        LittleFS.begin(true);

        // Web-Dashboard starten (Live-Sensor via Funktionszeiger)
        setupDashboardRoutes(server, readMedian, rawToPercent,
                             rtcLastWaterTime, rtcStreakDays, rtcPlantLevel);
        server.begin();

        // Event-Loop: läuft bis Timeout oder erneuter Knopfdruck
        uint32_t startMs  = millis();
        uint32_t timeoutMs = (uint32_t)WIFI_ACTIVE_TIMEOUT_MIN * 60UL * 1000UL;
        bool     btnWasHigh = true;

        while (millis() - startMs < timeoutMs) {
            server.handleClient();

            // Button: erneuter Druck → sofort schlafen
            bool btnNow = (digitalRead(PIN_BUTTON) == HIGH);
            if (btnWasHigh && !btnNow) {
                delay(50);
                if (digitalRead(PIN_BUTTON) == LOW) {
                    Serial.println("[WiFi] Knopf gedrückt → schlafen");
                    break;
                }
            }
            btnWasHigh = btnNow;
            delay(5);
        }

        server.stop();
        LittleFS.end();
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);

        if (display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
            display.clearDisplay();
            display.display();
            display.ssd1306_command(SSD1306_DISPLAYOFF);
        }
    }

sleep:
    pixelOff();
    Serial.printf("[Sleep] %d Minuten\n", SLEEP_MINUTES);
    Serial.flush();

    esp_sleep_enable_timer_wakeup((uint64_t)SLEEP_MINUTES * 60ULL * 1000000ULL);
    esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_BUTTON, LOW);
    esp_deep_sleep_start();
}

// ═════════════════════════════════════════════════════════════
// SETUP – läuft nach jedem Aufwachen
// ═════════════════════════════════════════════════════════════
void setup() {
    Serial.begin(115200);
    pinMode(PIN_BUTTON, INPUT_PULLUP);

    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();

    Serial.printf("\n[Boot] Wakeup: %d\n", (int)cause);

    if (cause == ESP_SLEEP_WAKEUP_EXT0) {
        // Button gedrückt → WiFi-Modus
        runWiFiMode();
    } else {
        // Timer-Wake oder erster Boot → normaler Mess-Zyklus
        runSleepCycle();
    }
}

void loop() {
    // Wird nie erreicht — ESP32 schläft nach setup()
}

/**
 * SmartSoil – LittleFS Datenspeicherung
 */

#ifndef STORAGE_H
#define STORAGE_H

#include <LittleFS.h>

#define READINGS_FILE   "/readings.csv"
#define EVENTS_FILE     "/events.csv"
#define MAX_READINGS    1440   // ~30 Tage à 48 Messungen/Tag

// ── Messung speichern ─────────────────────────────────────────
void appendReading(uint32_t ts, float moisture, int raw, bool watered) {
    File f = LittleFS.open(READINGS_FILE, "a");
    if (!f) return;
    f.printf("%lu,%.1f,%d,%d\n", ts, moisture, raw, watered ? 1 : 0);
    f.close();
}

// ── Gießereignis speichern ────────────────────────────────────
void logWateringEvent(uint32_t ts, float fromPct, float toPct) {
    File f = LittleFS.open(EVENTS_FILE, "a");
    if (!f) return;
    f.printf("%lu,%.1f,%.1f\n", ts, fromPct, toPct);
    f.close();
}

// ── Messungen als JSON (letzte 7 Tage = 336 Einträge) ─────────
String getReadingsJSON() {
    File f = LittleFS.open(READINGS_FILE, "r");
    if (!f || f.size() == 0) return "[]";

    // Zeilen zählen
    int total = 0;
    while (f.available()) { if (f.read() == '\n') total++; }
    f.seek(0);

    int skip = (total > 336) ? (total - 336) : 0;
    int lineNum = 0;
    String json = "[";
    bool first = true;

    while (f.available()) {
        String line = f.readStringUntil('\n');
        lineNum++;
        if (lineNum <= skip || line.length() < 5) continue;

        uint32_t ts; float m; int r; int w;
        if (sscanf(line.c_str(), "%lu,%f,%d,%d", &ts, &m, &r, &w) == 4) {
            if (!first) json += ",";
            json += "{\"t\":" + String(ts) +
                    ",\"m\":" + String(m, 1) +
                    ",\"w\":" + String(w) + "}";
            first = false;
        }
    }
    f.close();
    json += "]";
    return json;
}

// ── Gießereignisse als JSON (letzte 10) ───────────────────────
String getEventsJSON() {
    File f = LittleFS.open(EVENTS_FILE, "r");
    if (!f || f.size() == 0) return "[]";

    // Alle Zeilen sammeln, letzte 10 zurückgeben
    String lines[10];
    int idx = 0;
    while (f.available()) {
        String line = f.readStringUntil('\n');
        if (line.length() > 5) {
            lines[idx % 10] = line;
            idx++;
        }
    }
    f.close();

    int count = min(idx, 10);
    int start = (idx > 10) ? (idx % 10) : 0;

    String json = "[";
    bool first = true;
    for (int i = 0; i < count; i++) {
        String& line = lines[(start + i) % 10];
        uint32_t ts; float from; float to;
        if (sscanf(line.c_str(), "%lu,%f,%f", &ts, &from, &to) == 3) {
            if (!first) json += ",";
            json += "{\"t\":" + String(ts) +
                    ",\"f\":" + String(from, 1) +
                    ",\"to\":" + String(to, 1) + "}";
            first = false;
        }
    }
    json += "]";
    return json;
}

// ── Dateigröße prüfen & alte Einträge trimmen ─────────────────
void trimReadingsIfNeeded() {
    File f = LittleFS.open(READINGS_FILE, "r");
    if (!f) return;
    int total = 0;
    while (f.available()) { if (f.read() == '\n') total++; }
    f.close();

    if (total <= MAX_READINGS) return;

    // Zu alt: neue Datei mit den letzten MAX_READINGS Zeilen
    File src = LittleFS.open(READINGS_FILE, "r");
    File tmp = LittleFS.open("/readings_tmp.csv", "w");
    if (!src || !tmp) return;

    int skip = total - MAX_READINGS;
    int lineNum = 0;
    while (src.available()) {
        String line = src.readStringUntil('\n');
        lineNum++;
        if (lineNum > skip) {
            tmp.println(line);
        }
    }
    src.close();
    tmp.close();

    LittleFS.remove(READINGS_FILE);
    LittleFS.rename("/readings_tmp.csv", READINGS_FILE);
}

#endif // STORAGE_H

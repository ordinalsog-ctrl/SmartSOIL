/**
 * SmartSoil – Pflanzenpersönlichkeit & Profile
 */

#ifndef PLANT_H
#define PLANT_H

#include "config.h"

// ── Persönlichkeit ────────────────────────────────────────────
struct PlantPersonality {
    const char* face;       // ASCII Gesicht für OLED
    const char* mood;       // Statuswort
    const char* message;    // Kurznachricht
    uint8_t r, g, b;        // NeoPixel Farbe
};

PlantPersonality getPersonality(float moisture) {
    if (moisture > THRESH_WET)
        return { "(~_~)", "zu nass",   "Ich ertrinke!",      128,   0, 255 };
    if (moisture >= THRESH_OK_HIGH)
        return { "(^_^)", "feucht",    "Schoen feucht!",       0, 100, 255 };
    if (moisture >= THRESH_DRY)
        return { "(^o^)", "optimal",   "Mir geht's gut!",      0, 200,   0 };
    if (moisture >= THRESH_CRITICAL_DRY)
        return { "(-_-)", "trocken",   "Wasser waere nett",  255, 140,   0 };
    return     { "(x_x)", "kritisch!", "HILFE! GIESSEN!",    255,   0,   0 };
}

// ── Pflanzenprofile ────────────────────────────────────────────
struct PlantProfile {
    const char* name;
    float critDry, dry, okHigh, wet;
};

const PlantProfile PLANT_PROFILES[] = {
    { "Kaktus",    5,  10, 30, 40 },
    { "Monstera", 20,  35, 65, 80 },
    { "Orchidee", 30,  45, 70, 80 },
    { "Tomate",   25,  40, 70, 85 },
    { "Standard", 15,  30, 65, 80 },
};
const int PLANT_PROFILE_COUNT = 5;

#endif // PLANT_H

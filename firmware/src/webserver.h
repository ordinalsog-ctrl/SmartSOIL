#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <WiFi.h>
#include <WebServer.h>
#include "config.h"
#include "sensors.h"

class SmartSoilWeb {
public:
    WebServer server;
    SensorData lastData;
    bool running = false;

    SmartSoilWeb() : server(WEBSERVER_PORT) {}

    void begin() {
        WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASS);
        Serial.print("WiFi AP gestartet: ");
        Serial.println(WiFi.softAPIP());

        server.on("/", [this]() { handleRoot(); });
        server.on("/api/data", [this]() { handleAPI(); });
        server.begin();
        running = true;
    }

    void update(const SensorData& data) {
        lastData = data;
        if (running) server.handleClient();
    }

private:
    void handleRoot() {
        String html = R"rawliteral(
<!DOCTYPE html>
<html lang="de">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>SmartSoil</title>
<style>
  * { margin: 0; padding: 0; box-sizing: border-box; }
  body { font-family: -apple-system, sans-serif; background: #1a1a2e; color: #eee; padding: 20px; }
  h1 { text-align: center; margin-bottom: 20px; color: #4ecca3; }
  .card { background: #16213e; border-radius: 12px; padding: 20px; margin-bottom: 16px; }
  .label { font-size: 14px; color: #888; margin-bottom: 4px; }
  .value { font-size: 36px; font-weight: bold; }
  .bar-bg { background: #0f3460; border-radius: 8px; height: 12px; margin-top: 8px; }
  .bar-fill { border-radius: 8px; height: 12px; transition: width 0.5s; }
  .moisture .bar-fill { background: #4ecca3; }
  .ph .bar-fill { background: #e94560; }
  .status { text-align: center; font-size: 20px; padding: 16px;
            background: #0f3460; border-radius: 12px; }
  .status.warn { background: #e94560; }
  .status.ok { background: #4ecca3; color: #1a1a2e; }
  .battery { text-align: right; font-size: 12px; color: #666; }
</style>
</head>
<body>
<h1>SmartSoil</h1>
<div class="battery" id="bat"></div>
<div class="card moisture">
  <div class="label">Bodenfeuchte</div>
  <div class="value" id="moist">--</div>
  <div class="bar-bg"><div class="bar-fill" id="mbar" style="width:0%"></div></div>
</div>
<div class="card ph">
  <div class="label">pH-Wert</div>
  <div class="value" id="ph">--</div>
  <div class="bar-bg"><div class="bar-fill" id="pbar" style="width:0%"></div></div>
</div>
<div class="status" id="status">Verbinde...</div>
<script>
function update() {
  fetch('/api/data').then(r=>r.json()).then(d => {
    document.getElementById('moist').textContent = d.moisture.toFixed(1) + '%';
    document.getElementById('ph').textContent = d.ph.toFixed(1);
    document.getElementById('bat').textContent = 'Akku: ' + d.battery + '%';
    document.getElementById('mbar').style.width = d.moisture + '%';
    document.getElementById('pbar').style.width = (d.ph/14*100) + '%';
    let s = document.getElementById('status');
    s.textContent = d.status;
    s.className = 'status ' + (d.status.includes('optimal') ? 'ok' :
                                d.status.includes('Giessen') ? 'warn' : '');
  }).catch(() => {
    document.getElementById('status').textContent = 'Keine Verbindung';
  });
}
update();
setInterval(update, 5000);
</script>
</body>
</html>
)rawliteral";
        server.send(200, "text/html", html);
    }

    void handleAPI() {
        String status = getStatusText();
        String json = "{";
        json += "\"moisture\":" + String(lastData.moisturePercent, 1) + ",";
        json += "\"ph\":" + String(lastData.phValue, 1) + ",";
        json += "\"battery\":" + String(lastData.batteryPercent) + ",";
        json += "\"batteryV\":" + String(lastData.batteryVoltage, 2) + ",";
        json += "\"status\":\"" + status + "\"";
        json += "}";
        server.send(200, "application/json", json);
    }

    String getStatusText() {
        if (lastData.batteryPercent < 10) return "Akku fast leer!";
        if (lastData.moisturePercent < MOISTURE_THRESHOLD_LOW) {
            if (lastData.phValue < PH_WARN_LOW) return "Giessen + kalken!";
            return "Giessen!";
        }
        if (lastData.moisturePercent > MOISTURE_THRESHOLD_HIGH) return "Zu nass!";
        if (lastData.phValue < PH_WARN_LOW) return "pH zu sauer";
        if (lastData.phValue > PH_WARN_HIGH) return "pH zu basisch";
        return "Alles optimal!";
    }
};

#endif

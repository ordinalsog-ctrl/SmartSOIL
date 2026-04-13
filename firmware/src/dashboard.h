/**
 * SmartSoil – Embedded Web Dashboard
 * Self-contained HTML/CSS/JS, keine externen Abhängigkeiten
 */

#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <WebServer.h>
#include <LittleFS.h>
#include "storage.h"
#include "plant.h"
#include "config.h"

// ── Dashboard HTML (im Flash gespeichert) ─────────────────────
const char DASHBOARD_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="de">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>SmartSoil</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',sans-serif;background:#0d1b2a;color:#e8f4fd;padding:16px;min-height:100vh}
.card{background:#1a2d42;border-radius:20px;padding:20px;margin-bottom:16px;box-shadow:0 4px 20px rgba(0,0,0,0.3)}
.label{font-size:11px;font-weight:600;letter-spacing:1.5px;text-transform:uppercase;color:#5a8ab0;margin-bottom:8px}
.big-val{font-size:80px;font-weight:800;line-height:1;letter-spacing:-2px}
.big-val span{font-size:36px;color:#5a8ab0;font-weight:400}
.mood{font-size:20px;font-weight:600;margin-top:4px}
.msg{font-size:14px;color:#5a8ab0;margin-top:6px}
.bar-wrap{background:#0d1b2a;border-radius:10px;height:14px;margin:18px 0 10px;overflow:hidden}
.bar{height:14px;border-radius:10px;transition:width .8s cubic-bezier(.4,0,.2,1),background .5s}
.face{font-size:52px;margin-bottom:12px}
.grid{display:grid;grid-template-columns:1fr 1fr;gap:12px}
.stat{background:#0d1b2a;border-radius:14px;padding:16px;text-align:center}
.stat-val{font-size:32px;font-weight:700}
.stat-lbl{font-size:11px;color:#5a8ab0;margin-top:4px;letter-spacing:.5px}
canvas{display:block;width:100%}
.ev-item{display:flex;justify-content:space-between;align-items:center;padding:12px 0;border-bottom:1px solid #0d1b2a;font-size:14px}
.ev-item:last-child{border-bottom:none}
.ev-badge{background:#1a5276;color:#85c1e9;padding:3px 8px;border-radius:20px;font-size:12px;font-weight:600}
.badge-ok{background:#1a4731;color:#58d68d}
.badge-dry{background:#4d2600;color:#f0a500}
.badge-crit{background:#4a0000;color:#f44336}
.refresh-btn{display:block;width:100%;padding:14px;background:#1a3a5c;border:none;border-radius:14px;color:#85c1e9;font-size:15px;font-weight:600;cursor:pointer;margin-top:4px;letter-spacing:.5px}
.refresh-btn:active{background:#1a5276}
.header-row{display:flex;justify-content:space-between;align-items:flex-start}
.level-badge{background:#2c3e50;color:#f0c040;padding:4px 12px;border-radius:20px;font-size:13px;font-weight:700}
.last-update{font-size:11px;color:#5a8ab0;margin-top:8px}
</style>
</head>
<body>

<!-- Hauptkarte: Feuchtigkeit + Persönlichkeit -->
<div class="card">
  <div class="header-row">
    <div class="label" id="plantName">Pflanze</div>
    <div class="level-badge" id="levelBadge">Lv. 1</div>
  </div>
  <div class="face" id="faceEmoji">🌱</div>
  <div class="big-val" id="moistureVal">--<span>%</span></div>
  <div class="mood" id="moodText">Lade...</div>
  <div class="bar-wrap"><div class="bar" id="bar" style="width:0%"></div></div>
  <div class="msg" id="msgText"></div>
  <div class="last-update" id="lastUpdate"></div>
</div>

<!-- Streak + Level Stats -->
<div class="grid">
  <div class="stat card" style="margin-bottom:0">
    <div class="stat-val" id="streakVal">–</div>
    <div class="stat-lbl">Tage optimal 🔥</div>
  </div>
  <div class="stat card" style="margin-bottom:0">
    <div class="stat-val" id="lastWaterVal">–</div>
    <div class="stat-lbl">Zuletzt gegossen</div>
  </div>
</div>

<!-- Verlaufsgraph -->
<div class="card" style="margin-top:16px">
  <div class="label">Verlauf (7 Tage)</div>
  <canvas id="chart" height="160"></canvas>
</div>

<!-- Gießereignisse -->
<div class="card">
  <div class="label">Gießereignisse</div>
  <div id="events"><div style="color:#5a8ab0;padding:10px 0">Noch keine Ereignisse</div></div>
</div>

<!-- Aktualisieren -->
<button class="refresh-btn" onclick="loadData()">↻ Aktualisieren</button>

<script>
const CRIT=15,DRY=30,OK=65,WET=80;

function color(m){
  if(m>WET)  return '#9b59b6';
  if(m>=OK)  return '#2196f3';
  if(m>=DRY) return '#27ae60';
  if(m>=CRIT)return '#f39c12';
  return '#e74c3c';
}

function emoji(m){
  if(m>WET)  return '😰';
  if(m>=OK)  return '😊';
  if(m>=DRY) return '😄';
  if(m>=CRIT)return '😐';
  return '😵';
}

function badgeClass(m){
  if(m>WET||m<CRIT) return 'ev-badge badge-crit';
  if(m>=DRY)        return 'ev-badge badge-ok';
  return 'ev-badge badge-dry';
}

function fmtTime(ts){
  if(!ts||ts<1000000) return '?';
  const d=new Date(ts*1000);
  const dd=String(d.getDate()).padStart(2,'0');
  const mm=String(d.getMonth()+1).padStart(2,'0');
  const hh=String(d.getHours()).padStart(2,'0');
  const mi=String(d.getMinutes()).padStart(2,'0');
  return dd+'.'+mm+' '+hh+':'+mi;
}

function fmtAgo(ts){
  if(!ts||ts<1000000) return '–';
  const s=Math.floor(Date.now()/1000)-ts;
  if(s<3600) return Math.floor(s/60)+'min';
  if(s<86400) return Math.floor(s/3600)+'h';
  return Math.floor(s/86400)+'d';
}

function drawChart(readings){
  const canvas=document.getElementById('chart');
  const ctx=canvas.getContext('2d');
  const W=canvas.width=canvas.offsetWidth*window.devicePixelRatio;
  const H=canvas.height=160*window.devicePixelRatio;
  ctx.scale(window.devicePixelRatio,window.devicePixelRatio);
  const w=canvas.offsetWidth,h=160;
  ctx.clearRect(0,0,W,H);

  if(!readings||readings.length<2){
    ctx.fillStyle='#5a8ab0';ctx.font='14px sans-serif';ctx.textAlign='center';
    ctx.fillText('Noch keine Verlaufsdaten',w/2,h/2);return;
  }

  const pad={t:12,r:8,b:22,l:34};
  const cw=w-pad.l-pad.r, ch=h-pad.t-pad.b;

  // Optimale Zone
  const y30=pad.t+ch*(1-DRY/100), y65=pad.t+ch*(1-OK/100);
  ctx.fillStyle='rgba(39,174,96,0.08)';
  ctx.fillRect(pad.l,y65,cw,y30-y65);

  // Grid + Labels
  [0,25,50,75,100].forEach(pct=>{
    const py=pad.t+ch*(1-pct/100);
    ctx.strokeStyle='rgba(90,138,176,0.15)';ctx.lineWidth=1;
    ctx.beginPath();ctx.moveTo(pad.l,py);ctx.lineTo(pad.l+cw,py);ctx.stroke();
    ctx.fillStyle='#5a8ab0';ctx.font='10px sans-serif';ctx.textAlign='right';
    ctx.fillText(pct+'%',pad.l-5,py+3);
  });

  // Füllfläche
  const lastM=readings[readings.length-1].m;
  const grad=ctx.createLinearGradient(0,pad.t,0,pad.t+ch);
  grad.addColorStop(0,color(lastM)+'60');
  grad.addColorStop(1,color(lastM)+'00');
  ctx.beginPath();
  readings.forEach((r,i)=>{
    const x=pad.l+i/(readings.length-1)*cw;
    const y=pad.t+ch*(1-r.m/100);
    i===0?ctx.moveTo(x,y):ctx.lineTo(x,y);
  });
  ctx.lineTo(pad.l+cw,pad.t+ch);ctx.lineTo(pad.l,pad.t+ch);ctx.closePath();
  ctx.fillStyle=grad;ctx.fill();

  // Linie
  ctx.beginPath();ctx.lineWidth=2.5;ctx.lineJoin='round';
  readings.forEach((r,i)=>{
    const x=pad.l+i/(readings.length-1)*cw;
    const y=pad.t+ch*(1-r.m/100);
    i===0?ctx.moveTo(x,y):ctx.lineTo(x,y);
  });
  ctx.strokeStyle=color(lastM);ctx.stroke();

  // Gieß-Dots
  readings.forEach((r,i)=>{
    if(r.w!==1)return;
    const x=pad.l+i/(readings.length-1)*cw;
    const y=pad.t+ch*(1-r.m/100);
    ctx.beginPath();ctx.arc(x,y,5,0,Math.PI*2);
    ctx.fillStyle='#85c1e9';ctx.fill();
    ctx.strokeStyle='#1a2d42';ctx.lineWidth=1.5;ctx.stroke();
  });
}

async function loadData(){
  try{
    const res=await fetch('/api/data');
    const d=await res.json();
    const m=parseFloat(d.moisture)||0;
    const c=color(m);

    document.getElementById('plantName').textContent=d.plant||'Pflanze';
    document.getElementById('levelBadge').textContent='Lv. '+(d.level||1);
    document.getElementById('faceEmoji').textContent=emoji(m);
    document.getElementById('moistureVal').innerHTML=Math.round(m)+'<span>%</span>';
    document.getElementById('moodText').textContent=d.status||'';
    document.getElementById('msgText').textContent=d.message||'';
    document.getElementById('bar').style.width=m+'%';
    document.getElementById('bar').style.background=c;
    document.getElementById('streakVal').textContent=(d.streak||0)+' 🔥';
    document.getElementById('lastWaterVal').textContent=fmtAgo(d.lastWater);
    document.getElementById('lastUpdate').textContent='Letzte Aktualisierung: '+new Date().toLocaleTimeString('de-DE');

    if(d.readings) drawChart(d.readings);

    if(d.events&&d.events.length>0){
      const evDiv=document.getElementById('events');
      evDiv.innerHTML=[...d.events].reverse().map(e=>
        `<div class="ev-item">
          <span>💧 ${fmtTime(e.t)}</span>
          <span class="${badgeClass(e.to)}">${Math.round(e.f)}% → ${Math.round(e.to)}%</span>
        </div>`
      ).join('');
    }
  }catch(e){console.error('Ladefehler:',e)}
}

loadData();
setInterval(loadData,60000);
window.addEventListener('resize',()=>{
  fetch('/api/data').then(r=>r.json()).then(d=>{if(d.readings)drawChart(d.readings)}).catch(()=>{});
});
</script>
</body>
</html>
)rawliteral";

// ── Namespace für Web-Handler-State ───────────────────────────
namespace Dashboard {
    int       streak   = 0;
    int       level    = 1;
    uint32_t  lastWater = 0;
    // Funktionszeiger für Live-Sensor-Lesung
    int   (*fnReadRaw)()       = nullptr;
    float (*fnToPercent)(int)  = nullptr;
}

// ── Web-Routen registrieren ───────────────────────────────────
void setupDashboardRoutes(WebServer& server,
                          int   (*readRawFn)(),
                          float (*toPercentFn)(int),
                          uint32_t lastWater, int streak, int level) {
    Dashboard::fnReadRaw    = readRawFn;
    Dashboard::fnToPercent  = toPercentFn;
    Dashboard::lastWater    = lastWater;
    Dashboard::streak       = streak;
    Dashboard::level        = level;

    server.on("/", HTTP_GET, [&server]() {
        server.send_P(200, "text/html", DASHBOARD_HTML);
    });

    server.on("/api/data", HTTP_GET, [&server]() {
        // Sensor live lesen bei jedem API-Aufruf
        int   raw      = Dashboard::fnReadRaw ? Dashboard::fnReadRaw() : 0;
        float moisture = Dashboard::fnToPercent ? Dashboard::fnToPercent(raw) : 0.0f;
        PlantPersonality p = getPersonality(moisture);

        String json = "{";
        json += "\"device\":\"" + String(DEVICE_ID) + "\",";
        json += "\"plant\":\"" + String(PLANT_NAME) + "\",";
        json += "\"moisture\":" + String(moisture, 1) + ",";
        json += "\"raw\":" + String(raw) + ",";
        json += "\"status\":\"" + String(p.mood) + "\",";
        json += "\"message\":\"" + String(p.message) + "\",";
        json += "\"streak\":" + String(Dashboard::streak) + ",";
        json += "\"level\":" + String(Dashboard::level) + ",";
        json += "\"lastWater\":" + String(Dashboard::lastWater) + ",";
        json += "\"readings\":" + getReadingsJSON() + ",";
        json += "\"events\":" + getEventsJSON();
        json += "}";
        server.send(200, "application/json", json);
    });
}

#endif // DASHBOARD_H

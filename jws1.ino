#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

char namaWifi[] = "JWS-Setting-Jam";
char password[] = "12345678";

ESP8266WebServer server(80);

// Paste kode HTML asli Anda di sini tanpa perubahan
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>JWS Garut</title>
    <style>
        :root {
            --primary: #00ff88;
            --bg: #121212;
            --card: #1e1e1e;
            --input-bg: #2d2d2d;
        }

        body {
            font-family: 'Segoe UI', sans-serif;
            background-color: var(--bg);
            color: white;
            display: flex;
            justify-content: center;
            padding: 15px;
            margin: 0;
        }

        .container {
            background: var(--card);
            padding: 20px;
            border-radius: 20px;
            box-shadow: 0 10px 40px rgba(0,0,0,0.6);
            width: 100%;
            max-width: 400px;
        }

        .header-wrapper {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 20px;
            border-bottom: 1px solid #333;
            padding-bottom: 15px;
        }

        #clock {
            font-size: 2.2rem;
            font-weight: bold;
            color: var(--primary);
            line-height: 1;
        }

        #date {
            font-size: 0.85rem;
            color: #bbb;
            margin-top: 5px;
        }

        .updateDate {
            background: transparent;
            color: var(--primary);
            border: 1px solid var(--primary);
            padding: 5px 12px;
            border-radius: 8px;
            font-size: 0.7rem;
            font-weight: bold;
            cursor: pointer;
            transition: 0.3s;
        }

        .section-title {
            font-size: 0.8rem;
            color: var(--primary);
            margin: 15px 0 10px;
            text-transform: uppercase;
            letter-spacing: 1px;
            display: flex;
            align-items: center;
        }

        .section-title::before {
            content: "";
            width: 3px;
            height: 12px;
            background: var(--primary);
            margin-right: 8px;
        }

        input {
            background: var(--input-bg);
            border: 1px solid transparent;
            padding: 10px;
            border-radius: 8px;
            color: white;
            font-size: 0.9rem;
            width: 100%;
            box-sizing: border-box;
        }

        input:focus { border-color: var(--primary); outline: none; }

        .grid-2-col {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 12px;
            margin-bottom: 10px;
        }

        label { font-size: 0.7rem; color: #888; margin-bottom: 4px; }

        .offset-grid {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 10px;
        }

        .offset-item {
            display: flex;
            justify-content: space-between;
            align-items: center;
            background: var(--input-bg);
            padding: 8px 12px;
            border-radius: 8px;
        }

        .offset-item label { margin: 0; font-weight: bold; color: #ddd; font-size: 0.75rem; }
        .offset-item input {
            width: 50px;
            padding: 2px;
            text-align: center;
            background: #3d3d3d;
            color: var(--primary);
            border-radius: 4px;
        }

        .btn {
            padding: 12px;
            border: none;
            border-radius: 8px;
            font-weight: bold;
            cursor: pointer;
            transition: 0.3s;
            width: 100%;
            margin-top: 10px;
        }

        .btn-save { background: var(--primary); color: #121212; }
        .btn-full { background: linear-gradient(45deg, #00ff88, #00ccff); color: #121212; margin-top: 20px;}
      

        #code-output {
            background: #000;
            color: #00ff00;
            font-family: monospace;
            font-size: 0.7rem;
            padding: 10px;
            margin-top: 15px;
            border-radius: 8px;
            display: none;
            white-space: pre-wrap;
            border: 1px solid #333;
        }

        #status-log {
            font-size: 0.75rem;
            color: #f1c40f;
            text-align: center;
            margin-top: 10px;
        }
    </style>
</head>
<body>

<div class="container">
    <div class="header-wrapper">
        <div>
            <div id="clock">00:00:00</div>
            <div id="date">Memuat waktu...</div>
        </div>
        <button class="updateDate">Update</button>
    </div>

    <div class="section-title">Koordinat & Waktu</div>
    <div class="grid-2-col">
        <div class="input-group">
            <label>Latitude</label>
            <input type="text" value="-7.258163">
        </div>
        <div class="input-group">
            <label>Longitude</label>
            <input type="text" value="107.843739">
        </div>
    </div>

    <div class="input-group" style="margin-bottom: 15px;">
        <label>Timezone Offset (Menit) - WIB</label>
        <input type="number" value="420" placeholder="timzone x 60">
    </div>

    <div class="section-title">Koreksi Waktu / Ihtiati (Menit)</div>
    <div class="offset-grid">
        <div class="offset-item"><label>Imsyak</label><input type="number" value="0"></div>
        <div class="offset-item"><label>Subuh</label><input type="number" value="8"></div>
        <div class="offset-item"><label>Terbit</label><input type="number" value="2"></div>
        <div class="offset-item"><label>Duha</label><input type="number" value="-4"></div>
        <div class="offset-item"><label>Dzuhur</label><input type="number" value="5"></div>
        <div class="offset-item"><label>Ashar</label><input type="number" value="3"></div>
        <div class="offset-item"><label>Maghrib</label><input type="number" value="6"></div>
        <div class="offset-item"><label>Isya</label><input type="number" value="5"></div>
    </div>

    <div class="section-title">Alarm Manual</div>
    <div class="grid-2-col" style="grid-template-columns: 2fr 1fr; align-items: end;">
        <div class="input-group">
            <label>Jam Alarm</label>
            <input type="time">
        </div>
        <button id="alarm-set" class="btn btn-save" style="margin:0">SET</button>
    </div>

    <button id="save-date" class="btn btn-full">SAVE</button>

    <div id="code-output"></div>
    <div id="status-log">Sistem Siap.</div>
</div>

</body>

<script>
    function updateClock(){
        let now = new Date();
        let h = String(now.getHours()).padStart(2,'0');
        let m = String(now.getMinutes()).padStart(2,'0');
        let s = String(now.getSeconds()).padStart(2,'0');
        let d = now.toLocaleDateString('id-ID', {
            weekday:'long', day:'numeric', month:'long', year:'numeric'
        });
        document.getElementById("clock").innerHTML = h+":"+m+":"+s;
        document.getElementById("date").innerHTML = d;
    }
    
    setInterval(updateClock, 1000);
    updateClock();

    function setRTC(){
        let n = new Date();
        let t = n.getFullYear()+"-"+String(n.getMonth()+1).padStart(2,'0')+"-"+String(n.getDate()).padStart(2,'0')+" "+
                String(n.getHours()).padStart(2,'0')+":"+String(n.getMinutes()).padStart(2,'0')+":"+String(n.getSeconds()).padStart(2,'0');

        fetch("/setTime?time="+t)
        .then(r => r.text())
        .then(msg => { document.getElementById("status-log").innerHTML = "RTC Berhasil ✅"; })
        .catch(err => { document.getElementById("status-log").innerHTML = "Gagal RTC ❌"; });
    }

    document.addEventListener("DOMContentLoaded", function(){
        let btnUpdate = document.querySelector(".updateDate");
        if(btnUpdate) btnUpdate.onclick = setRTC;

        // --- LOGIKA TOMBOL SAVE ---
        const btnSave = document.getElementById("save-date");
        btnSave.onclick = function() {
            // 1. Ambil Data Koordinat & Timezone
            const lat = document.querySelector("input[value='-7.258163']").value; // Sebaiknya beri ID pada input agar lebih akurat
            const lon = document.querySelector("input[value='107.843739']").value;
            const tz = document.querySelector("input[placeholder='timzone x 60']").value;

            // 2. Ambil Data Ihtiati (Urutan sesuai di HTML)
            const offsets = document.querySelectorAll(".offset-item input");
            const ihtiati = Array.from(offsets).map(input => input.value).join(",");

            // 3. Susun URL
            // Format: /saveData?lat=-7.25&lon=107.8&tz=420&iht=0,8,2,-4,5,3,6,5
            const url = `/saveData?lat=${lat}&lon=${lon}&tz=${tz}&iht=${ihtiati}`;

            document.getElementById("status-log").innerHTML = "Menyimpan data...";

            fetch(url)
            .then(r => r.text())
            .then(msg => {
                document.getElementById("status-log").innerHTML = "Data Tersimpan ke EEPROM ✅";
            })
            .catch(err => {
                document.getElementById("status-log").innerHTML = "Gagal Simpan ❌";
            });
        };

        // --- LOGIKA TOMBOL ALARM ---
        const btnAlarm = document.getElementById("alarm-set");
        const inputAlarm = document.querySelector("input[type='time']");
        if(btnAlarm) {
            btnAlarm.onclick = function() {
                let timeVal = inputAlarm.value;
                if(!timeVal) return alert("Isi Jam!");
                fetch("/setAlarm?time=" + timeVal)
                .then(r => r.text())
                .then(msg => { document.getElementById("status-log").innerHTML = "Alarm Set: " + timeVal + " ✅"; });
            };
        }
    });
</script>
</html>
)rawliteral";

// Fungsi untuk menangani data yang masuk dari tombol Update
void handleSetTime() {
  // Cek apakah ada parameter bernama "time" (sesuai kode JS Anda)
  if (server.hasArg("time")) {
    String waktuHP = server.arg("time");
    
    // Tampilkan ke Serial Monitor
    Serial.println("");
    Serial.println(">>> DATA DITERIMA DARI WEB <<<");
    Serial.print("Waktu HP: ");
    Serial.println(waktuHP); 
    Serial.println("------------------------------");
    
    // Kirim balasan ke Web agar status-log berubah menjadi sukses
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Gagal: Parameter tidak ditemukan");
  }
}
void handleSetAlarm() {
  if (server.hasArg("time")) {
    String alarmTime = server.arg("time");
    
    Serial.println("===============================");
    Serial.print("ALARM BARU DITERIMA: ");
    Serial.println(alarmTime); // Akan muncul jam format HH:MM
    Serial.println("===============================");
    
    server.send(200, "text/plain", "Alarm diterima: " + alarmTime);
    
    // Tips: Kamu bisa memecah string jam dan menit di sini
    // int jam = alarmTime.substring(0, 2).toInt();
    // int menit = alarmTime.substring(3, 5).toInt();
  } else {
    server.send(400, "text/plain", "Gagal: Data waktu tidak ada");
  }
}
void handleSaveData() {
  String lat = server.arg("lat");
  String lon = server.arg("lon");
  String tz  = server.arg("tz");
  String iht = server.arg("iht"); // Ini berisi data: 0,8,2,-4,5,3,6,5

  Serial.println("======= DATA BARU DITERIMA =======");
  Serial.println("Latitude  : " + lat);
  Serial.println("Longitude : " + lon);
  Serial.println("Timezone  : " + tz);
  Serial.println("Ihtiati   : " + iht);
  Serial.println("==================================");

  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200); // Gunakan baudrate 115200 di Serial Monitor
  
  WiFi.softAP(namaWifi, password);
  
  Serial.println("");
  Serial.print("WiFi Aktif: ");
  Serial.println(namaWifi);
  Serial.print("Buka browser ke IP: ");
  Serial.println(WiFi.softAPIP());

  // Halaman Utama
  server.on("/", []() {
    server.send(200, "text/html", index_html);
  });

  // Endpoint untuk menangkap data waktu
  server.on("/setTime", handleSetTime);
  server.on("/setAlarm", handleSetAlarm);
  server.on("/saveData", handleSaveData);

  server.begin();
}

void loop() {
  server.handleClient();
}
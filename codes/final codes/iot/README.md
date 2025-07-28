# 📁 IoT - Sistem Lampu Lalu Lintas ESP32 dengan Adafruit IO

## 🎯 Overview
Folder ini berisi implementasi sistem lampu lalu lintas ESP32 versi **IoT** dengan fitur lengkap. Sistem ini terintegrasi dengan Adafruit IO untuk monitoring real-time, data logging, dan kontrol remote.

## 🚦 Sistem yang Tersedia

| File | Jenis Intersection | LED | Pin | Durasi Siklus | IoT Features |
|------|-------------------|-----|-----|---------------|--------------|
| `2set.ino` | Jalan Lurus 2 Arah | 6 | 6 | ~50 detik | ✅ |
| `pertigaan_y.ino` | Pertigaan Y-Shape | 9 | 9 | ~60 detik | ✅ |
| `pertigaan_t.ino` | Pertigaan T-Shape | 9 | 9 | ~50 detik | ✅ |
| `perempatan.ino` | Perempatan Lengkap | 12 | 12 | ~120 detik | ✅ |

## 🔧 Komponen Hardware

### Daftar Komponen
- **ESP32 Dev Kit** (1 unit)
- **LED** (sesuai jenis sistem)
- **Resistor 220Ω** (sesuai jumlah LED)
- **Breadboard** (1 unit)
- **Kabel Jumper** (sesuai kebutuhan)
- **Koneksi WiFi** (wajib)
- **2x Push Button** (untuk emergency & pedestrian mode)

### Software Requirements
- **Arduino IDE** dengan ESP32 board package
- **Library Adafruit MQTT**
- **Library WiFi** (built-in ESP32)

## 🌐 Fitur IoT

### Adafruit IO Integration
| Fitur | Deskripsi | Data Type |
|-------|-----------|-----------|
| **Real-time Status** | Status lampu saat ini | String |
| **State Number** | Nomor state (0-18) | Integer |
| **Uptime** | Waktu operasi sistem | Integer (detik) |
| **JSON Data** | Data lengkap dalam JSON | JSON String |
| **Mode Status** | Emergency/Pedestrian mode | String |

### MQTT Topics
```
YOUR_AIO_USERNAME/feeds/traffic-status
YOUR_AIO_USERNAME/feeds/traffic-state  
YOUR_AIO_USERNAME/feeds/traffic-uptime
YOUR_AIO_USERNAME/feeds/traffic-data
YOUR_AIO_USERNAME/feeds/traffic-mode
```

## 📋 Detail Sistem

### 1. 🛣️ **2set.ino** - Jalan Lurus 2 Arah (dengan Fase Belok)

#### Koneksi Hardware
| Pin ESP32 | LED | Arah |
|-----------|-----|------|
| D23 | Hijau | Utara |
| D22 | Kuning | Utara |
| D21 | Merah | Utara |
| D19 | Hijau | Selatan |
| D18 | Kuning | Selatan |
| D5 | Merah | Selatan |
| D25 | Tombol | Emergency |
| D26 | Tombol | Pedestrian |

#### State Machine (11 States)
| State | Utara | Selatan | Durasi | IoT Data |
|-------|-------|---------|--------|----------|
| 1 | 🟢 Hijau | 🟢 Hijau | 15 detik | "Hijau Lurus (Utara & Selatan)" |
| 2 | 🟡 Kuning | 🟡 Kuning | 3 detik | "Kuning (Utara & Selatan)" |
| 3 | 🔴 Merah | 🔴 Merah | 2 detik | "Semua Merah (Transisi)" |
| 4 | 🟢 Hijau | 🔴 Merah | 8 detik | "Hijau Belok (Utara)" |
| 5 | 🟡 Kuning | 🔴 Merah | 3 detik | "Kuning (Utara)" |
| 6 | 🔴 Merah | 🔴 Merah | 2 detik | "Semua Merah (Transisi)" |
| 7 | 🔴 Merah | 🟢 Hijau | 8 detik | "Hijau Belok (Selatan)" |
| 8 | 🔴 Merah | 🟡 Kuning | 3 detik | "Kuning (Selatan)" |
| 9 | 🔴 Merah | 🔴 Merah | 2 detik | "Semua Merah (Transisi)" |
| 10 | Emergency | Blinking | 20 detik | "Mode Darurat" |
| 11 | Pedestrian | All Red | 15 detik | "Mode Pejalan Kaki" |

#### IoT Features
- **Status Update**: Setiap state change
- **Uptime Logging**: Setiap 30 detik
- **JSON Data**: Timestamp, state, duration, direction
- **Mode Tracking**: Emergency/Pedestrian status

### 2. 🔀 **pertigaan_y.ino** - Pertigaan Y-Shape

#### Koneksi Hardware
| Pin ESP32 | LED | Jalur |
|-----------|-----|-------|
| D23 | Hijau | Utara |
| D22 | Kuning | Utara |
| D21 | Merah | Utara |
| D19 | Hijau | Selatan |
| D18 | Kuning | Selatan |
| D5 | Merah | Selatan |
| D4 | Hijau | Timur |
| D2 | Kuning | Timur |
| D15 | Merah | Timur |
| D25 | Tombol | Emergency |
| D26 | Tombol | Pedestrian |

#### State Machine (11 States)
| State | Utara | Selatan | Timur | Durasi | IoT Data |
|-------|-------|---------|-------|--------|----------|
| 1 | 🟢 Hijau | 🔴 Merah | 🔴 Merah | 15 detik | "Utara Hijau" |
| 2 | 🟡 Kuning | 🔴 Merah | 🔴 Merah | 3 detik | "Utara Kuning" |
| 3 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | "Semua Merah (Transisi)" |
| 4 | 🔴 Merah | 🟢 Hijau | 🔴 Merah | 15 detik | "Selatan Hijau" |
| 5 | 🔴 Merah | 🟡 Kuning | 🔴 Merah | 3 detik | "Selatan Kuning" |
| 6 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | "Semua Merah (Transisi)" |
| 7 | 🔴 Merah | 🔴 Merah | 🟢 Hijau | 15 detik | "Timur Hijau" |
| 8 | 🔴 Merah | 🔴 Merah | 🟡 Kuning | 3 detik | "Timur Kuning" |
| 9 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | "Semua Merah (Transisi)" |
| 10 | Emergency | Blinking | Blinking | 20 detik | "Mode Darurat" |
| 11 | Pedestrian | All Red | All Red | 15 detik | "Mode Pejalan Kaki" |

#### IoT Features
- **Direction Tracking**: "utara", "selatan", "timur"
- **Status Monitoring**: "hijau", "kuning", "merah"
- **Type Identification**: "y_shape"
- **Save State**: Menyimpan state sebelum interupsi

### 3. 🔀 **pertigaan_t.ino** - Pertigaan T-Shape

#### Koneksi Hardware
| Pin ESP32 | LED | Jalur |
|-----------|-----|-------|
| D23 | Hijau | Utara-Selatan |
| D22 | Kuning | Utara-Selatan |
| D21 | Merah | Utara-Selatan |
| D19 | Hijau | Timur |
| D18 | Kuning | Timur |
| D5 | Merah | Timur |
| D4 | Hijau | Barat |
| D2 | Kuning | Barat |
| D15 | Merah | Barat |
| D25 | Tombol | Emergency |
| D26 | Tombol | Pedestrian |

#### State Machine (8 States)
| State | Utara-Selatan | Timur | Durasi | IoT Data |
|-------|---------------|-------|--------|----------|
| 1 | 🟢 Hijau | 🔴 Merah | 20 detik | "Utara-Selatan Hijau" |
| 2 | 🟡 Kuning | 🔴 Merah | 3 detik | "Utara-Selatan Kuning" |
| 3 | 🔴 Merah | 🔴 Merah | 2 detik | "Semua Merah (Transisi)" |
| 4 | 🔴 Merah | 🟢 Hijau | 20 detik | "Timur Hijau" |
| 5 | 🔴 Merah | 🟡 Kuning | 3 detik | "Timur Kuning" |
| 6 | 🔴 Merah | 🔴 Merah | 2 detik | "Semua Merah (Transisi)" |
| 7 | Emergency | Blinking | Blinking | 20 detik | "Mode Darurat" |
| 8 | Pedestrian | All Red | All Red | 15 detik | "Mode Pejalan Kaki" |

#### IoT Features
- **Direction Tracking**: "utara_selatan", "timur"
- **Priority Logic**: Main road gets longer green time
- **Type Identification**: "t_shape"
- **Save State**: Menyimpan state sebelum interupsi

### 4. 🚦 **perempatan.ino** - Perempatan Lengkap

#### Koneksi Hardware
| Pin ESP32 | LED | Arah |
|-----------|-----|------|
| D23 | Hijau | Utara |
| D22 | Kuning | Utara |
| D21 | Merah | Utara |
| D19 | Hijau | Selatan |
| D18 | Kuning | Selatan |
| D5 | Merah | Selatan |
| D4 | Hijau | Timur |
| D2 | Kuning | Timur |
| D15 | Merah | Timur |
| D0 | Hijau | Barat |
| D16 | Kuning | Barat |
| D17 | Merah | Barat |
| D25 | Tombol | Emergency |
| D26 | Tombol | Pedestrian |

#### State Machine (20 States)
| State | Utara | Selatan | Timur | Barat | Durasi | IoT Data |
|-------|-------|---------|-------|-------|--------|----------|
| 1 | 🟢 Hijau | 🟢 Hijau | 🔴 Merah | 🔴 Merah | 20 detik | "Utara-Selatan Hijau" |
| 2 | 🟡 Kuning | 🟡 Kuning | 🔴 Merah | 🔴 Merah | 3 detik | "Utara-Selatan Kuning" |
| 3 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | "Semua Merah (Transisi)" |
| 4 | 🔴 Merah | 🔴 Merah | 🟢 Hijau | 🟢 Hijau | 20 detik | "Timur-Barat Hijau" |
| 5 | 🔴 Merah | 🔴 Merah | 🟡 Kuning | 🟡 Kuning | 3 detik | "Timur-Barat Kuning" |
| 6 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | "Semua Merah (Transisi)" |
| 7 | 🟢 Hijau | 🔴 Merah | 🔴 Merah | 🔴 Merah | 10 detik | "Utara Individual" |
| 8 | 🟡 Kuning | 🔴 Merah | 🔴 Merah | 🔴 Merah | 3 detik | "Utara Kuning" |
| 9 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | "Semua Merah (Transisi)" |
| 10 | 🔴 Merah | 🔴 Merah | 🟢 Hijau | 🔴 Merah | 10 detik | "Timur Individual" |
| 11 | 🔴 Merah | 🔴 Merah | 🟡 Kuning | 🔴 Merah | 3 detik | "Timur Kuning" |
| 12 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | "Semua Merah (Transisi)" |
| 13 | 🔴 Merah | 🟢 Hijau | 🔴 Merah | 🔴 Merah | 10 detik | "Selatan Individual" |
| 14 | 🔴 Merah | 🟡 Kuning | 🔴 Merah | 🔴 Merah | 3 detik | "Selatan Kuning" |
| 15 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | "Semua Merah (Transisi)" |
| 16 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🟢 Hijau | 10 detik | "Barat Individual" |
| 17 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🟡 Kuning | 3 detik | "Barat Kuning" |
| 18 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | "Semua Merah (Transisi)" |
| 19 | Emergency | Blinking | Blinking | Blinking | 20 detik | "Mode Darurat" |
| 20 | Pedestrian | All Red | All Red | All Red | 15 detik | "Mode Pejalan Kaki" |

#### IoT Features
- **Direction Tracking**: "utara_selatan", "timur_barat", "individual"
- **Mode Monitoring**: "normal", "emergency", "pedestrian"
- **Type Identification**: "complete"
- **Save State**: Menyimpan state sebelum interupsi

## 🚀 Setup dan Konfigurasi

### 1. Adafruit IO Setup
1. Buat akun di [Adafruit IO](https://io.adafruit.com)
2. Buat feeds berikut:
   - `traffic-status` (Text)
   - `traffic-state` (Number)
   - `traffic-uptime` (Number)
   - `traffic-data` (Text)
   - `traffic-mode` (Text)

### 2. Konfigurasi WiFi
```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
```

### 3. Konfigurasi Adafruit IO
```cpp
#define AIO_USERNAME    "YOUR_AIO_USERNAME"
#define AIO_KEY         "YOUR_AIO_KEY"
```

### 4. Upload dan Testing
1. Upload kode ke ESP32
2. Monitor Serial Monitor untuk koneksi
3. Periksa Adafruit IO dashboard
4. Verifikasi data real-time

## 📊 Data yang Dikirim

### JSON Data Structure
```json
{
  "timestamp": "1234567890",
  "state": 1,
  "duration": 20000,
  "direction": "utara_selatan_lurus",
  "status": "hijau",
  "mode": "normal",
  "type": "2_way"
}
```

### Real-time Monitoring
- **Status**: State lampu saat ini
- **State Number**: 0-20 (sesuai sistem)
- **Uptime**: Waktu operasi dalam detik
- **Mode**: Normal/Emergency/Pedestrian

## 🔍 Troubleshooting IoT

### Masalah Umum
| Masalah | Penyebab | Solusi |
|---------|----------|--------|
| WiFi tidak connect | Kredensial salah | Periksa SSID & password |
| MQTT error | Adafruit IO config | Periksa username & key |
| Data tidak terkirim | Koneksi terputus | Restart ESP32 |
| JSON error | Format data | Periksa kode JSON |
| Button tidak respon | Pin salah | Periksa koneksi tombol |

### Tips IoT
- **Stable WiFi**: Pastikan sinyal WiFi kuat
- **MQTT Keep-alive**: Sistem auto-reconnect
- **Data Validation**: Periksa format JSON
- **Error Handling**: Monitor Serial untuk error
- **Button Debouncing**: Implementasi debouncing untuk tombol

## 📈 Monitoring Dashboard

### Adafruit IO Dashboard
1. **Status Feed**: Real-time state
2. **State Chart**: Grafik state changes
3. **Uptime Monitor**: Waktu operasi
4. **Data Logger**: Historical data
5. **Mode Indicator**: Emergency/Pedestrian status

### Data Analytics
- **Traffic Flow**: Analisis pola traffic
- **System Health**: Uptime monitoring
- **Performance**: State transition timing
- **Alerts**: Emergency mode notifications

## 🎯 Rekomendasi Penggunaan

### Untuk Production
- **Implementasi**: Semua sistem IoT
- **Monitoring**: 24/7 dashboard
- **Backup**: Offline mode capability
- **Security**: Secure WiFi network

### Untuk Development
- **Testing**: Mulai dengan basic version
- **Debugging**: Monitor Serial output
- **Integration**: Step-by-step IoT setup
- **Validation**: Verify data accuracy

---

**💡 Tips**: IoT version memberikan monitoring lengkap dan kontrol remote, ideal untuk sistem traffic light yang membutuhkan supervision!
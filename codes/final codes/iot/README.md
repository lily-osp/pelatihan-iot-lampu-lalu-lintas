# 📁 IoT - Sistem Lampu Lalu Lintas ESP32 dengan Adafruit IO

## 🎯 Overview
Folder ini berisi implementasi sistem lampu lalu lintas ESP32 versi **IoT** dengan fitur lengkap. Sistem ini terintegrasi dengan Adafruit IO untuk monitoring real-time, data logging, dan kontrol remote.

## 🚦 Sistem yang Tersedia

| File | Jenis Intersection | LED | Pin | Durasi Siklus | IoT Features |
|------|-------------------|-----|-----|---------------|--------------|
| `2set.ino` | Jalan Lurus 2 Arah | 6 | 6 | ~50 detik | ✅ |
| `pertigaan_y.ino` | Pertigaan Y-Shape | 9 | 9 | ~120 detik | ✅ |
| `pertigaan_t.ino` | Pertigaan T-Shape | 9 | 9 | ~100 detik | ✅ |
| `perempatan.ino` | Perempatan Lengkap | 12 | 12 | ~180 detik | ✅ |

## 🔧 Komponen Hardware

### Daftar Komponen
- **ESP32 Dev Kit** (1 unit)
- **LED** (sesuai jenis sistem)
- **Resistor 220Ω** (sesuai jumlah LED)
- **Breadboard** (1 unit)
- **Kabel Jumper** (sesuai kebutuhan)
- **Koneksi WiFi** (wajib)

### Software Requirements
- **Arduino IDE** dengan ESP32 board package
- **Library Adafruit MQTT**
- **Library WiFi** (built-in ESP32)

## 🌐 Fitur IoT

### Adafruit IO Integration
| Fitur | Deskripsi | Data Type |
|-------|-----------|-----------|
| **Real-time Status** | Status lampu saat ini | String |
| **State Number** | Nomor state (0-11) | Integer |
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

### 1. 🛣️ **2set.ino** - Jalan Lurus 2 Arah

#### Koneksi Hardware (Sama dengan Basic)
| Pin ESP32 | LED | Arah |
|-----------|-----|------|
| D23 | Hijau | Arah 1 |
| D22 | Kuning | Arah 1 |
| D21 | Merah | Arah 1 |
| D19 | Hijau | Arah 2 |
| D18 | Kuning | Arah 2 |
| D5 | Merah | Arah 2 |

#### State Machine (6 States)
| State | Arah 1 | Arah 2 | Durasi | IoT Data |
|-------|--------|--------|--------|----------|
| 1 | 🟢 Hijau | 🔴 Merah | 20 detik | "State 1" |
| 2 | 🟡 Kuning | 🔴 Merah | 3 detik | "State 2" |
| 3 | 🔴 Merah | 🔴 Merah | 2 detik | "State 3" |
| 4 | 🔴 Merah | 🟢 Hijau | 20 detik | "State 4" |
| 5 | 🔴 Merah | 🟡 Kuning | 3 detik | "State 5" |
| 6 | 🔴 Merah | 🔴 Merah | 2 detik | "State 6" |

#### IoT Features
- **Status Update**: Setiap state change
- **Uptime Logging**: Setiap 30 detik
- **JSON Data**: Timestamp, state, duration, direction

### 2. 🔀 **pertigaan_y.ino** - Pertigaan Y-Shape

#### Koneksi Hardware (Sama dengan Basic)
| Pin ESP32 | LED | Jalur |
|-----------|-----|-------|
| D23 | Hijau | Jalur Utama |
| D22 | Kuning | Jalur Utama |
| D21 | Merah | Jalur Utama |
| D19 | Hijau | Cabang 1 |
| D18 | Kuning | Cabang 1 |
| D5 | Merah | Cabang 1 |
| D4 | Hijau | Cabang 2 |
| D2 | Kuning | Cabang 2 |
| D15 | Merah | Cabang 2 |

#### State Machine (9 States)
| State | Jalur Utama | Cabang 1 | Cabang 2 | Durasi | IoT Data |
|-------|-------------|----------|----------|--------|----------|
| 1 | 🟢 Hijau | 🔴 Merah | 🔴 Merah | 30 detik | "State 1" |
| 2 | 🟡 Kuning | 🔴 Merah | 🔴 Merah | 4 detik | "State 2" |
| 3 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | "State 3" |
| 4 | 🔴 Merah | 🟢 Hijau | 🔴 Merah | 25 detik | "State 4" |
| 5 | 🔴 Merah | 🟡 Kuning | 🔴 Merah | 4 detik | "State 5" |
| 6 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | "State 6" |
| 7 | 🔴 Merah | 🔴 Merah | 🟢 Hijau | 25 detik | "State 7" |
| 8 | 🔴 Merah | 🔴 Merah | 🟡 Kuning | 4 detik | "State 8" |
| 9 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | "State 9" |

#### IoT Features
- **Direction Tracking**: "jalur_utama", "cabang_1", "cabang_2"
- **Status Monitoring**: "hijau", "kuning", "merah"
- **Type Identification**: "y_shape"

### 3. 🔀 **pertigaan_t.ino** - Pertigaan T-Shape

#### Koneksi Hardware (Sama dengan Basic)
| Pin ESP32 | LED | Jalur |
|-----------|-----|-------|
| D23 | Hijau | Main Road |
| D22 | Kuning | Main Road |
| D21 | Merah | Main Road |
| D19 | Hijau | Side Road |
| D18 | Kuning | Side Road |
| D5 | Merah | Side Road |
| D4 | Hijau | Left Turn |
| D2 | Kuning | Left Turn |
| D15 | Merah | Left Turn |

#### State Machine (9 States)
| State | Main Road | Side Road | Left Turn | Durasi | IoT Data |
|-------|-----------|-----------|-----------|--------|----------|
| 1 | 🟢 Hijau | 🔴 Merah | 🔴 Merah | 35 detik | "State 1" |
| 2 | 🟡 Kuning | 🔴 Merah | 🔴 Merah | 4 detik | "State 2" |
| 3 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | "State 3" |
| 4 | 🔴 Merah | 🟢 Hijau | 🔴 Merah | 20 detik | "State 4" |
| 5 | 🔴 Merah | 🟡 Kuning | 🔴 Merah | 4 detik | "State 5" |
| 6 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | "State 6" |
| 7 | 🔴 Merah | 🔴 Merah | 🟢 Hijau | 15 detik | "State 7" |
| 8 | 🔴 Merah | 🔴 Merah | 🟡 Kuning | 4 detik | "State 8" |
| 9 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | "State 9" |

#### IoT Features
- **Direction Tracking**: "main_road", "side_road", "left_turn"
- **Priority Logic**: Main road gets longer green time
- **Type Identification**: "t_shape"

### 4. 🚦 **perempatan.ino** - Perempatan Lengkap

#### Koneksi Hardware (Sama dengan Basic)
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

#### State Machine (12 States)
| State | Utara | Selatan | Timur | Barat | Durasi | IoT Data |
|-------|-------|---------|-------|-------|--------|----------|
| 1 | 🟢 Hijau | 🟢 Hijau | 🔴 Merah | 🔴 Merah | 30 detik | "State 1" |
| 2 | 🟡 Kuning | 🟡 Kuning | 🔴 Merah | 🔴 Merah | 4 detik | "State 2" |
| 3 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | "State 3" |
| 4 | 🔴 Merah | 🔴 Merah | 🟢 Hijau | 🟢 Hijau | 25 detik | "State 4" |
| 5 | 🔴 Merah | 🔴 Merah | 🟡 Kuning | 🟡 Kuning | 4 detik | "State 5" |
| 6 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | "State 6" |
| 7 | 🟢 Hijau | 🔴 Merah | 🔴 Merah | 🔴 Merah | 10 detik | "State 7" |
| 8 | 🔴 Merah | 🔴 Merah | 🟢 Hijau | 🔴 Merah | 10 detik | "State 8" |
| 9 | 🔴 Merah | 🟢 Hijau | 🔴 Merah | 🔴 Merah | 10 detik | "State 9" |
| 10 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🟢 Hijau | 10 detik | "State 10" |
| 11 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🔴 Merah | 5 detik | "Emergency" |
| 12 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🔴 Merah | 15 detik | "Pedestrian" |

#### IoT Features
- **Direction Tracking**: "utara_selatan", "timur_barat", "individual"
- **Mode Monitoring**: "normal", "emergency", "pedestrian"
- **Type Identification**: "complete"

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
  "direction": "main_road",
  "status": "hijau",
  "type": "t_shape"
}
```

### Real-time Monitoring
- **Status**: State lampu saat ini
- **State Number**: 0-11 (sesuai sistem)
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

### Tips IoT
- **Stable WiFi**: Pastikan sinyal WiFi kuat
- **MQTT Keep-alive**: Sistem auto-reconnect
- **Data Validation**: Periksa format JSON
- **Error Handling**: Monitor Serial untuk error

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
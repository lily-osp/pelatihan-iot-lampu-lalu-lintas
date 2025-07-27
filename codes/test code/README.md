# 📁 Test Code - Testing dan Debugging Tools

## 🎯 Overview
Folder ini berisi kode testing dan debugging untuk memverifikasi hardware, koneksi WiFi, dan komponen sistem lampu lalu lintas ESP32.

## 🧪 Sistem Testing yang Tersedia

| File | Tujuan | Komponen | Durasi Test |
|------|--------|----------|-------------|
| `blink_test.ino` | Hardware Verification | LED + Resistor | 5-10 menit |
| `wifi_test.ino` | WiFi Connection Test | ESP32 + WiFi | 2-3 menit |
| `adafruit_test.ino` | Adafruit IO Test | ESP32 + WiFi + MQTT | 3-5 menit |

## 🔧 Komponen Hardware untuk Testing

### Daftar Komponen
- **ESP32 Dev Kit** (1 unit)
- **LED** (sesuai kebutuhan test)
- **Resistor 220Ω** (sesuai jumlah LED)
- **Breadboard** (1 unit)
- **Kabel Jumper** (sesuai kebutuhan)
- **Koneksi WiFi** (untuk wifi_test)

## 📋 Detail Testing

### 1. 🔆 **blink_test.ino** - Hardware Verification

#### Tujuan
- Verifikasi koneksi LED dan resistor
- Test semua pin ESP32 yang digunakan
- Memastikan hardware berfungsi dengan baik
- Debug masalah koneksi

#### Komponen yang Ditest
| Pin ESP32 | LED | Warna | Keterangan |
|-----------|-----|-------|------------|
| D23 | LED 1 | Hijau | Arah 1 / Main Road |
| D22 | LED 2 | Kuning | Arah 1 / Main Road |
| D21 | LED 3 | Merah | Arah 1 / Main Road |
| D19 | LED 4 | Hijau | Arah 2 / Side Road |
| D18 | LED 5 | Kuning | Arah 2 / Side Road |
| D5 | LED 6 | Merah | Arah 2 / Side Road |
| D4 | LED 7 | Hijau | Cabang / Left Turn |
| D2 | LED 8 | Kuning | Cabang / Left Turn |
| D15 | LED 9 | Merah | Cabang / Left Turn |
| D0 | LED 10 | Hijau | Barat (4-way) |
| D16 | LED 11 | Kuning | Barat (4-way) |
| D17 | LED 12 | Merah | Barat (4-way) |

#### Test Sequence
| Test | Durasi | Pattern | Keterangan |
|------|--------|---------|------------|
| 1 | 2 detik | All ON | Test semua LED menyala |
| 2 | 2 detik | All OFF | Test semua LED mati |
| 3 | 1 detik | Sequential | Test satu per satu |
| 4 | 1 detik | Alternating | Test bergantian |
| 5 | 2 detik | Color Pattern | Test per warna |

#### Expected Output
```
=== Hardware Test - LED Verification ===
Test 1: All LEDs ON
Test 2: All LEDs OFF  
Test 3: Sequential Test
  - LED 1 (Pin 23) - ON
  - LED 2 (Pin 22) - ON
  - LED 3 (Pin 21) - ON
  ...
Test 4: Alternating Pattern
Test 5: Color Pattern
  - All Green LEDs
  - All Yellow LEDs
  - All Red LEDs
```

#### Troubleshooting
| Masalah | Penyebab | Solusi |
|---------|----------|--------|
| LED tidak menyala | Koneksi salah | Periksa wiring |
| LED redup | Resistor terlalu besar | Gunakan 220Ω |
| LED terlalu terang | Resistor terlalu kecil | Gunakan 220Ω |
| Beberapa LED mati | Pin rusak | Ganti pin ESP32 |

---

### 2. 📶 **wifi_test.ino** - WiFi Connection Test

#### Tujuan
- Test koneksi WiFi ESP32
- Verifikasi kredensial WiFi
- Menampilkan informasi koneksi WiFi lengkap
- Monitor status WiFi secara real-time

#### Komponen yang Ditest
- **ESP32 WiFi Module**
- **WiFi Network**
- **Network Configuration**

#### Test Sequence
| Test | Durasi | Tujuan | Expected Result |
|------|--------|--------|-----------------|
| 1 | 10 detik | WiFi Connect | "WiFi Connected" |
| 2 | 10 detik | IP Address | "IP: 192.168.x.x" |
| 3 | 15 detik | Adafruit IO | "Adafruit IO Connected" |
| 4 | 30 detik | MQTT Test | "Data sent successfully" |
| 5 | 10 detik | Connection Test | "Connection stable" |

#### Konfigurasi WiFi
```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
```

#### Konfigurasi Adafruit IO
```cpp
#define AIO_USERNAME    "YOUR_AIO_USERNAME"
#define AIO_KEY         "YOUR_AIO_KEY"
```

#### Expected Output
```
=== WiFi Connection Test ===
Connecting to WiFi: YOUR_WIFI_SSID
WiFi Connected!
IP Address: 192.168.1.100
Connecting to Adafruit IO...
Adafruit IO Connected!
Sending test data...
Data sent successfully: Test Message
Connection test completed!
```

#### Troubleshooting WiFi
| Masalah | Penyebab | Solusi |
|---------|----------|--------|
| WiFi tidak connect | SSID/Password salah | Periksa kredensial |
| IP tidak dapat | DHCP error | Restart router |
| Adafruit IO error | Username/Key salah | Periksa AIO config |
| MQTT timeout | Network slow | Periksa bandwidth |

## 🚀 Cara Penggunaan Testing

### 1. Hardware Test (blink_test.ino)
1. **Setup Hardware**
   - Hubungkan LED sesuai tabel
   - Gunakan resistor 220Ω
   - Periksa koneksi ground

2. **Upload dan Test**
   - Upload kode ke ESP32
   - Buka Serial Monitor (115200 baud)
   - Monitor LED dan output serial

3. **Verifikasi**
   - Semua LED menyala sesuai pattern
   - Serial output sesuai expected
   - Tidak ada error messages

### 2. WiFi Test (wifi_test.ino)
1. **Setup Konfigurasi**
   - Edit SSID dan password WiFi
   - Periksa network availability

2. **Upload dan Test**
   - Upload kode ke ESP32
   - Monitor Serial Monitor
   - Periksa koneksi status

3. **Verifikasi**
   - WiFi connected successfully
   - IP address obtained
   - Network information displayed
   - WiFi monitoring active

### 3. Adafruit IO Test (adafruit_test.ino)
1. **Setup Konfigurasi**
   - Edit SSID dan password WiFi
   - Edit Adafruit IO credentials (username & key)
   - Pastikan akun Adafruit IO aktif
   - Buat feeds di Adafruit IO dashboard

2. **Upload dan Test**
   - Upload kode ke ESP32
   - Monitor Serial Monitor
   - Periksa koneksi MQTT
   - Monitor data publishing

3. **Verifikasi**
   - WiFi connected successfully
   - Adafruit IO MQTT broker connected
   - Data publishing works (string, number, JSON)
   - Subscribe functionality works
   - Control messages received

## 📊 Test Results Validation

### Hardware Test Checklist
- [ ] All LEDs light up in sequence
- [ ] All LEDs turn off properly
- [ ] Alternating pattern works
- [ ] Color pattern works
- [ ] No flickering or dimming
- [ ] Serial output matches expected

### WiFi Test Checklist
- [ ] WiFi connects within 10 seconds
- [ ] IP address is obtained
- [ ] Network information displayed
- [ ] WiFi monitoring active
- [ ] Connection remains stable
- [ ] No timeout errors

### Adafruit IO Test Checklist
- [ ] WiFi connects successfully
- [ ] Adafruit IO MQTT broker connects
- [ ] String data publishes successfully
- [ ] Number data publishes successfully
- [ ] JSON data publishes successfully
- [ ] Subscribe to control feed works
- [ ] Control messages received properly
- [ ] Connection remains stable
- [ ] No MQTT errors

## 🔍 Advanced Testing

### Performance Testing
- **Response Time**: < 1 detik untuk LED change
- **WiFi Stability**: > 99% uptime
- **Data Transmission**: < 5 detik delay
- **Memory Usage**: < 80% ESP32 memory

### Stress Testing
- **Continuous Operation**: 24 jam test
- **Network Interruption**: WiFi disconnect/reconnect
- **Power Cycling**: ESP32 restart test
- **Data Volume**: High frequency data sending

## 📈 Test Reporting

### Hardware Test Report
```
Hardware Test Results:
✅ All 12 LEDs functional
✅ All pins working correctly
✅ Resistor values appropriate
✅ Ground connections proper
⏱️ Test duration: 5 minutes
📊 Success rate: 100%
```

### WiFi Test Report
```
WiFi Test Results:
✅ WiFi connection successful
✅ IP address: 192.168.1.100
✅ Network information displayed
✅ WiFi monitoring active
⏱️ Connection time: 8 seconds
📊 Signal strength: -45 dBm
```

### Adafruit IO Test Report
```
Adafruit IO Test Results:
✅ WiFi connection successful
✅ Adafruit IO MQTT broker connected
✅ String data publishing OK
✅ Number data publishing OK
✅ JSON data publishing OK
✅ Subscribe functionality OK
✅ Control messages received
⏱️ MQTT connection time: 3 seconds
📊 Messages sent: 15
```

## 🎯 Rekomendasi Testing

### Sebelum Main Project
1. **Jalankan blink_test.ino** terlebih dahulu
2. **Verifikasi semua LED** berfungsi
3. **Test WiFi** jika menggunakan IoT version
4. **Perbaiki masalah** sebelum lanjut

### Untuk Production
1. **Comprehensive testing** semua komponen
2. **Stress testing** untuk reliability
3. **Documentation** hasil testing
4. **Backup plan** jika ada failure

### Untuk Development
1. **Incremental testing** setiap komponen
2. **Debug mode** untuk troubleshooting
3. **Version control** untuk test results
4. **Iterative improvement** berdasarkan hasil

---

**💡 Tips**: Selalu jalankan test code sebelum implementasi sistem utama untuk memastikan hardware dan koneksi berfungsi dengan baik!
# 📁 Proyek Lampu Lalu Lintas ESP32 - Dokumentasi Lengkap

## 🎯 Overview
Proyek ini berisi implementasi lengkap sistem lampu lalu lintas menggunakan ESP32 dengan berbagai konfigurasi intersection dan fitur IoT.

## 📂 Struktur Folder

```
codes/
├── 📁 basic/           # Versi non-IoT (tanpa WiFi/MQTT)
├── 📁 iot/             # Versi IoT (dengan WiFi & Adafruit IO)
└── 📁 test code/       # Kode testing dan debugging
```

## 🚦 Jenis Sistem Lampu Lalu Lintas

| Jenis | Deskripsi | Komponen | Pin |
|-------|-----------|----------|-----|
| **2-way** | Jalan lurus 2 arah | 6 LED (2×3 warna) | 6 pin |
| **Y-shape** | Pertigaan berbentuk Y | 9 LED (3×3 warna) | 9 pin |
| **T-shape** | Pertigaan berbentuk T | 9 LED (3×3 warna) | 9 pin |
| **4-way** | Perempatan lengkap | 12 LED (4×3 warna) | 12 pin |

## 🔧 Komponen yang Dibutuhkan

### Hardware
- **ESP32 Dev Kit** (1 unit)
- **LED** (6-12 unit sesuai jenis)
- **Resistor 220Ω** (sesuai jumlah LED)
- **Breadboard** (1 unit)
- **Kabel Jumper** (sesuai kebutuhan)

### Software
- **Arduino IDE** dengan ESP32 board package
- **Library Adafruit MQTT** (untuk versi IoT)

## 📋 Navigasi Dokumentasi

### [📁 Basic/](final%20codes/basic/README.md)
Versi sederhana tanpa koneksi internet - ideal untuk pembelajaran dan testing hardware.

### [📁 IoT/](final%20codes/iot/README.md)  
Versi lengkap dengan fitur IoT - monitoring real-time dan kontrol via cloud.

### [📁 Test Code/](test%20code/README.md)
Kode testing untuk debugging dan verifikasi hardware.

## 🚀 Cara Penggunaan

1. **Pilih folder sesuai kebutuhan**
2. **Baca README di folder tersebut**
3. **Siapkan hardware sesuai diagram**
4. **Upload kode ke ESP32**
5. **Monitor via Serial Monitor**

## 📊 Perbandingan Versi

| Fitur | Basic | IoT |
|-------|-------|-----|
| **Koneksi WiFi** | ❌ | ✅ |
| **Cloud Monitoring** | ❌ | ✅ |
| **Real-time Data** | ❌ | ✅ |
| **Kemudahan Setup** | ✅ | ⚠️ |
| **Fungsionalitas** | ✅ | ✅ |
| **Kompleksitas** | Rendah | Tinggi |

## 🔍 Troubleshooting

### Masalah Umum
- **LED tidak menyala**: Periksa koneksi dan resistor
- **State tidak berubah**: Periksa Serial Monitor
- **WiFi tidak connect**: Periksa kredensial WiFi
- **MQTT error**: Periksa kredensial Adafruit IO

### Tips
- Gunakan **test code** terlebih dahulu untuk verifikasi hardware
- Mulai dengan versi **basic** untuk memahami logika
- Upgrade ke versi **IoT** setelah basic berfungsi

## 📞 Support

Untuk pertanyaan atau masalah:
1. Periksa dokumentasi di folder masing-masing
2. Jalankan test code untuk debugging
3. Periksa Serial Monitor untuk error messages

---

**🎯 Rekomendasi**: Mulai dengan `basic/` untuk memahami sistem, lalu upgrade ke `iot/` untuk fitur lengkap! 
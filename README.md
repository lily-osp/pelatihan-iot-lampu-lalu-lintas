# 🚦 IoT Lampu Lalu Lintas Cerdas

**Platform Pembelajaran Komprehensif untuk Sistem Lampu Lalu Lintas Berbasis IoT**

[![ESP32](https://img.shields.io/badge/ESP32-Compatible-green.svg)](https://www.espressif.com/en/products/socs/esp32)
[![Arduino](https://img.shields.io/badge/Arduino-Framework-blue.svg)](https://www.arduino.cc/)
[![MQTT](https://img.shields.io/badge/MQTT-Protocol-orange.svg)](https://mqtt.org/)
[![Adafruit IO](https://img.shields.io/badge/Adafruit%20IO-Cloud-purple.svg)](https://io.adafruit.com/)

## 📋 Daftar Isi

- [🎯 Tentang Proyek](#-tentang-proyek)
- [🚀 Fitur Utama](#-fitur-utama)
- [📁 Struktur Proyek](#-struktur-proyek)
- [🔧 Komponen yang Diperlukan](#-komponen-yang-diperlukan)
- [⚡ Quick Start](#-quick-start)
- [📚 Dokumentasi Lengkap](#-dokumentasi-lengkap)
- [🎮 Demo Interaktif](#-demo-interaktif)
- [🔍 Jenis Sistem](#-jenis-sistem)
- [📊 Perbandingan Versi](#-perbandingan-versi)
- [🛠️ Troubleshooting](#️-troubleshooting)
- [🤝 Kontribusi](#-kontribusi)
- [📄 Lisensi](#-lisensi)

## 🎯 Tentang Proyek

Proyek **IoT Lampu Lalu Lintas Cerdas** adalah platform pembelajaran komprehensif yang menyediakan implementasi lengkap sistem lampu lalu lintas menggunakan ESP32 dan Arduino framework. Proyek ini dirancang untuk memudahkan pembelajaran IoT dari tingkat dasar hingga lanjutan.

### 🎓 Tujuan Pembelajaran

- **Pemrograman Mikrokontroler**: Belajar menggunakan ESP32 dengan Arduino framework
- **IoT Fundamentals**: Implementasi koneksi WiFi, MQTT, dan cloud computing
- **State Machine Logic**: Pemahaman logika state machine untuk sistem kontrol
- **Hardware Integration**: Koneksi sensor, LED, dan komponen elektronik
- **Real-time Monitoring**: Sistem monitoring dan kontrol jarak jauh

## 🚀 Fitur Utama

### 🔧 **Versi Basic (Non-IoT)**
- ✅ Sistem lampu lalu lintas standalone
- ✅ State machine logic yang lengkap
- ✅ Timing yang realistis dan dapat dikonfigurasi
- ✅ Ideal untuk pembelajaran dasar

### 🌐 **Versi IoT (Cloud Connected)**
- ✅ Koneksi WiFi otomatis
- ✅ MQTT communication dengan Adafruit IO
- ✅ Real-time monitoring dan kontrol
- ✅ Data logging dan analytics
- ✅ Alert system dan notifikasi

### 🎮 **Demo Interaktif**
- ✅ Visualisasi state machine logic
- ✅ Grafik durasi state interaktif
- ✅ Perbandingan antar sistem
- ✅ Simulasi real-time

## 📁 Struktur Proyek

```
lampu_lalin/
├── 📄 README.md                    # Dokumentasi utama (file ini)
├── 🌐 index.html                   # Demo interaktif web
├── 📜 script.js                    # JavaScript untuk demo
├── 🎨 styles.css                   # Styling untuk demo
└── 📁 codes/                       # Kode sumber Arduino
    ├── 📄 README.md                # Dokumentasi umum kode
    ├── 📁 test code/               # Kode testing dan verifikasi
    │   ├── 📄 README.md            # Dokumentasi test code
    │   ├── 🔧 blink_test.ino       # Test LED dan hardware
    │   └── 📡 wifi_test.ino        # Test koneksi WiFi
    └── 📁 final codes/             # Implementasi final
        ├── 📁 basic/               # Versi non-IoT
        │   ├── 📄 README.md        # Dokumentasi basic
        │   ├── 🛣️ 2set.ino         # Jalan lurus 2 arah
        │   ├── 🔀 pertigaan_y.ino  # Pertigaan Y-shape
        │   ├── 🔀 pertigaan_t.ino  # Pertigaan T-shape
        │   └── 🚦 perempatan.ino   # Perempatan lengkap
        └── 📁 iot/                 # Versi IoT
            ├── 📄 README.md        # Dokumentasi IoT
            ├── 🛣️ 2set.ino         # Jalan lurus 2 arah + IoT
            ├── 🔀 pertigaan_y.ino  # Pertigaan Y-shape + IoT
            ├── 🔀 pertigaan_t.ino  # Pertigaan T-shape + IoT
            └── 🚦 perempatan.ino   # Perempatan lengkap + IoT
```

## 🔧 Komponen yang Diperlukan

### 🎯 **Hardware Essentials**
| Komponen | Jumlah | Keterangan |
|----------|--------|------------|
| ESP32 Development Board | 1 | Mikrokontroler utama |
| LED Merah | 4-12 | Lampu merah traffic light |
| LED Kuning | 4-12 | Lampu kuning traffic light |
| LED Hijau | 4-12 | Lampu hijau traffic light |
| Resistor 220Ω | 12-36 | Current limiting untuk LED |
| Breadboard | 1 | Prototyping board |
| Jumper Wires | 20+ | Koneksi antar komponen |

### 🌐 **IoT Components (Opsional)**
| Komponen | Keterangan |
|----------|------------|
| WiFi Network | Koneksi internet |
| Adafruit IO Account | Cloud platform |
| MQTT Client | Real-time communication |

### 🛠️ **Tools**
| Tool | Keterangan |
|------|------------|
| Arduino IDE | Development environment |
| Wokwi Simulator | Online testing |
| Serial Monitor | Debugging |

## ⚡ Quick Start

### 🚀 **Langkah 1: Persiapan**
```bash
# Clone repository
git clone [repository-url]
cd lampu_lalin

# Buka Arduino IDE
# Install ESP32 board package
```

### 🔧 **Langkah 2: Pilih Sistem**
1. **Pemula**: Mulai dengan `codes/final codes/basic/2set.ino`
2. **Menengah**: Coba `codes/final codes/basic/pertigaan_y.ino`
3. **Lanjutan**: Implementasi `codes/final codes/iot/perempatan.ino`

### 📡 **Langkah 3: Konfigurasi IoT (Opsional)**
```cpp
// Edit file IoT untuk konfigurasi
#define WIFI_SSID "your_wifi_name"
#define WIFI_PASS "your_wifi_password"
#define AIO_USERNAME "your_adafruit_username"
#define AIO_KEY "your_adafruit_key"
```

### 🎮 **Langkah 4: Demo Interaktif**
Buka `index.html` di browser untuk melihat:
- Visualisasi state machine
- Perbandingan antar sistem
- Grafik durasi state

## 📚 Dokumentasi Lengkap

### 📖 **Dokumentasi Utama**
- **[📁 codes/README.md](codes/README.md)** - Panduan umum kode Arduino
- **[📁 codes/final codes/basic/README.md](codes/final%20codes/basic/README.md)** - Dokumentasi versi basic
- **[📁 codes/final codes/iot/README.md](codes/final%20codes/iot/README.md)** - Dokumentasi versi IoT
- **[📁 codes/test code/README.md](codes/test%20code/README.md)** - Dokumentasi test code

### 🎯 **Sistem yang Tersedia**

| Sistem | File Basic | File IoT | Kompleksitas | Durasi Siklus |
|--------|------------|----------|--------------|---------------|
| **Jalan Lurus 2 Arah** | `2set.ino` | `2set.ino` | ⭐ | ~50 detik |
| **Pertigaan Y-Shape** | `pertigaan_y.ino` | `pertigaan_y.ino` | ⭐⭐ | ~120 detik |
| **Pertigaan T-Shape** | `pertigaan_t.ino` | `pertigaan_t.ino` | ⭐⭐ | ~100 detik |
| **Perempatan Lengkap** | `perempatan.ino` | `perempatan.ino` | ⭐⭐⭐ | ~180 detik |

## 🎮 Demo Interaktif

Buka file `index.html` untuk mengakses demo interaktif yang menyediakan:

### 📊 **Fitur Demo**
- **State Machine Visualization**: Tabel interaktif dengan status lampu
- **Duration Charts**: Grafik durasi setiap state
- **System Comparison**: Perbandingan antar 4 sistem
- **Real-time Updates**: Perubahan dinamis saat memilih sistem

### 🎯 **Cara Menggunakan Demo**
1. Buka `index.html` di browser
2. Pilih sistem yang ingin dilihat
3. Amati tabel state machine dan grafik durasi
4. Bandingkan logika antar sistem

## 🎮 Simulasi Wokwi Online

Test dan verifikasi kode Arduino secara online menggunakan simulator Wokwi sebelum implementasi hardware:

### 🔧 **Test Code**
- **[🔧 Blink Test](https://wokwi.com/projects/437652096534506497)** - Verifikasi hardware dan koneksi LED

### 🛣️ **Basic Version (Non-IoT)**
- **[🛣️ Jalan Lurus 2 Arah](https://wokwi.com/projects/437655672510267393)** - Sistem traffic light 2 arah sederhana
- **[🔀 Pertigaan Y-Shape](https://wokwi.com/projects/437654184425653249)** - Sistem pertigaan Y-shape dengan 3 arah
- **[🔀 Pertigaan T-Shape](https://wokwi.com/projects/437654133817178113)** - Sistem pertigaan T-shape dengan main road priority
- **[🚦 Perempatan Lengkap](https://wokwi.com/projects/437653190980547585)** - Sistem perempatan 4 arah dengan fitur lengkap

### 💡 **Cara Menggunakan Simulasi**
1. Klik link simulasi untuk membuka Wokwi
2. Klik tombol "Start Simulation" untuk menjalankan
3. Monitor Serial untuk melihat log state machine
4. Observe LED behavior sesuai dengan state
5. Gunakan "Stop Simulation" untuk menghentikan

## 🔍 Jenis Sistem

### 🛣️ **1. Jalan Lurus 2 Arah**
- **Aplikasi**: Jalan lurus dengan traffic dua arah
- **States**: 6 states (2 arah × 3 phases)
- **Fitur**: Timing sederhana, transisi smooth

### 🔀 **2. Pertigaan Y-Shape**
- **Aplikasi**: Tiga jalan bertemu dengan sudut seimbang
- **States**: 9 states (3 arah × 3 phases)
- **Fitur**: Traffic flow seimbang, transisi all-red

### 🔀 **3. Pertigaan T-Shape**
- **Aplikasi**: Jalan utama dengan cabang samping
- **States**: 9 states (3 arah × 3 phases)
- **Fitur**: Main road priority, dedicated left turn

### 🚦 **4. Perempatan Lengkap**
- **Aplikasi**: Persimpangan empat arah kompleks
- **States**: 12 states (4 arah + individual turns + emergency)
- **Fitur**: Individual turn signals, emergency mode, pedestrian mode

## 📊 Perbandingan Versi

| Fitur | Basic Version | IoT Version |
|-------|---------------|-------------|
| **Kompleksitas** | ⭐⭐ | ⭐⭐⭐⭐ |
| **Setup Time** | 10 menit | 30 menit |
| **Hardware Cost** | $15-25 | $15-25 |
| **Cloud Features** | ❌ | ✅ |
| **Real-time Monitoring** | ❌ | ✅ |
| **Data Logging** | ❌ | ✅ |
| **Remote Control** | ❌ | ✅ |
| **Learning Curve** | Mudah | Menengah |

## 🛠️ Troubleshooting

### 🔧 **Masalah Umum**

#### **LED Tidak Menyala**
```cpp
// Periksa koneksi
// Pastikan resistor terpasang dengan benar
// Cek pin assignment di kode
```

#### **WiFi Tidak Terhubung**
```cpp
// Periksa SSID dan password
// Pastikan ESP32 mendukung WiFi
// Coba restart ESP32
```

#### **MQTT Connection Failed**
```cpp
// Periksa Adafruit IO credentials
// Pastikan internet connection stabil
// Cek MQTT library installation
```

### 📱 **Debug Tips**
- Gunakan Serial Monitor untuk debugging
- Test dengan `blink_test.ino` terlebih dahulu
- Verifikasi hardware dengan `wifi_test.ino`

## 🤝 Kontribusi

Kami menyambut kontribusi dari komunitas! Berikut cara berkontribusi:

### 🔄 **Cara Berkontribusi**
1. **Fork** repository ini
2. **Create** branch untuk fitur baru (`git checkout -b feature/AmazingFeature`)
3. **Commit** perubahan (`git commit -m 'Add some AmazingFeature'`)
4. **Push** ke branch (`git push origin feature/AmazingFeature`)
5. **Open** Pull Request

### 📝 **Guidelines**
- Gunakan bahasa Indonesia untuk dokumentasi
- Tambahkan komentar yang jelas di kode
- Test kode sebelum submit
- Update dokumentasi jika diperlukan

## 📄 Lisensi

Proyek ini dilisensikan di bawah **MIT License** - lihat file [LICENSE](LICENSE) untuk detail.

---

## 🎓 **Untuk Pembelajaran**

Proyek ini dirancang sebagai platform pembelajaran komprehensif. Mulai dari sistem sederhana dan tingkatkan kompleksitas secara bertahap:

1. **Tahap 1**: Pelajari `2set.ino` (basic)
2. **Tahap 2**: Implementasi `pertigaan_y.ino` (basic)
3. **Tahap 3**: Upgrade ke versi IoT
4. **Tahap 4**: Implementasi sistem kompleks

### 📚 **Referensi Tambahan**
- [ESP32 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- [Arduino Reference](https://www.arduino.cc/reference/en/)
- [MQTT Protocol](https://mqtt.org/documentation)
- [Adafruit IO Guide](https://learn.adafruit.com/adafruit-io)

---

**🎯 Happy Coding! Semoga proyek ini membantu pembelajaran IoT Anda! 🚦** 
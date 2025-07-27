# 📁 Basic - Sistem Lampu Lalu Lintas ESP32 (Non-IoT)

## 🎯 Overview
Folder ini berisi implementasi sistem lampu lalu lintas ESP32 versi **basic** (tanpa IoT). Ideal untuk pembelajaran, testing hardware, dan implementasi sederhana tanpa koneksi internet.

## 🚦 Sistem yang Tersedia

| File | Jenis Intersection | LED | Pin | Durasi Siklus | Kompleksitas |
|------|-------------------|-----|-----|---------------|--------------|
| `2set.ino` | Jalan Lurus 2 Arah | 6 | 6 | ~50 detik | ⭐ |
| `pertigaan_y.ino` | Pertigaan Y-Shape | 9 | 9 | ~120 detik | ⭐⭐ |
| `pertigaan_t.ino` | Pertigaan T-Shape | 9 | 9 | ~100 detik | ⭐⭐⭐ |
| `perempatan.ino` | Perempatan Lengkap | 12 | 12 | ~180 detik | ⭐⭐⭐⭐ |

## 🔧 Komponen Hardware

### Daftar Komponen
- **ESP32 Dev Kit** (1 unit)
- **LED** (sesuai jenis sistem)
- **Resistor 220Ω** (sesuai jumlah LED)
- **Breadboard** (1 unit)
- **Kabel Jumper** (sesuai kebutuhan)

## 📋 Detail Sistem

### 1. 🛣️ **2set.ino** - Jalan Lurus 2 Arah

#### Koneksi Hardware
| Pin ESP32 | LED | Arah |
|-----------|-----|------|
| D23 | Hijau | Arah 1 |
| D22 | Kuning | Arah 1 |
| D21 | Merah | Arah 1 |
| D19 | Hijau | Arah 2 |
| D18 | Kuning | Arah 2 |
| D5 | Merah | Arah 2 |

#### State Machine (6 States)
| State | Arah 1 | Arah 2 | Durasi |
|-------|--------|--------|--------|
| 1 | 🟢 Hijau | 🔴 Merah | 20 detik |
| 2 | 🟡 Kuning | 🔴 Merah | 3 detik |
| 3 | 🔴 Merah | 🔴 Merah | 2 detik |
| 4 | 🔴 Merah | 🟢 Hijau | 20 detik |
| 5 | 🔴 Merah | 🟡 Kuning | 3 detik |
| 6 | 🔴 Merah | 🔴 Merah | 2 detik |

### 2. 🔀 **pertigaan_y.ino** - Pertigaan Y-Shape

#### Koneksi Hardware
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
| State | Jalur Utama | Cabang 1 | Cabang 2 | Durasi |
|-------|-------------|----------|----------|--------|
| 1 | 🟢 Hijau | 🔴 Merah | 🔴 Merah | 30 detik |
| 2 | 🟡 Kuning | 🔴 Merah | 🔴 Merah | 4 detik |
| 3 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik |
| 4 | 🔴 Merah | 🟢 Hijau | 🔴 Merah | 25 detik |
| 5 | 🔴 Merah | 🟡 Kuning | 🔴 Merah | 4 detik |
| 6 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik |
| 7 | 🔴 Merah | 🔴 Merah | 🟢 Hijau | 25 detik |
| 8 | 🔴 Merah | 🔴 Merah | 🟡 Kuning | 4 detik |
| 9 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik |

### 3. 🔀 **pertigaan_t.ino** - Pertigaan T-Shape

#### Koneksi Hardware
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
| State | Main Road | Side Road | Left Turn | Durasi |
|-------|-----------|-----------|-----------|--------|
| 1 | 🟢 Hijau | 🔴 Merah | 🔴 Merah | 35 detik |
| 2 | 🟡 Kuning | 🔴 Merah | 🔴 Merah | 4 detik |
| 3 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik |
| 4 | 🔴 Merah | 🟢 Hijau | 🔴 Merah | 20 detik |
| 5 | 🔴 Merah | 🟡 Kuning | 🔴 Merah | 4 detik |
| 6 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik |
| 7 | 🔴 Merah | 🔴 Merah | 🟢 Hijau | 15 detik |
| 8 | 🔴 Merah | 🔴 Merah | 🟡 Kuning | 4 detik |
| 9 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik |

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

#### State Machine (12 States)
| State | Utara | Selatan | Timur | Barat | Durasi |
|-------|-------|---------|-------|-------|--------|
| 1 | 🟢 Hijau | 🟢 Hijau | 🔴 Merah | 🔴 Merah | 30 detik |
| 2 | 🟡 Kuning | 🟡 Kuning | 🔴 Merah | 🔴 Merah | 4 detik |
| 3 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik |
| 4 | 🔴 Merah | 🔴 Merah | 🟢 Hijau | 🟢 Hijau | 25 detik |
| 5 | 🔴 Merah | 🔴 Merah | 🟡 Kuning | 🟡 Kuning | 4 detik |
| 6 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik |
| 7 | 🟢 Hijau | 🔴 Merah | 🔴 Merah | 🔴 Merah | 10 detik |
| 8 | 🔴 Merah | 🔴 Merah | 🟢 Hijau | 🔴 Merah | 10 detik |
| 9 | 🔴 Merah | 🟢 Hijau | 🔴 Merah | 🔴 Merah | 10 detik |
| 10 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🟢 Hijau | 10 detik |
| 11 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🔴 Merah | 5 detik |
| 12 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🔴 Merah | 15 detik |

## 🚀 Cara Penggunaan

### 1. Persiapan Hardware
1. Siapkan ESP32 dan komponen sesuai tabel
2. Hubungkan LED dengan resistor 220Ω
3. Hubungkan ke pin ESP32 sesuai diagram

### 2. Upload Kode
1. Buka Arduino IDE
2. Pilih board ESP32
3. Upload file `.ino` yang dipilih
4. Buka Serial Monitor (115200 baud)

### 3. Testing
1. Monitor Serial Monitor untuk status
2. Periksa LED menyala sesuai state
3. Verifikasi timing sesuai spesifikasi

## 🔍 Troubleshooting

### Masalah Umum
| Masalah | Penyebab | Solusi |
|---------|----------|--------|
| LED tidak menyala | Koneksi salah | Periksa wiring dan resistor |
| State tidak berubah | Kode error | Periksa Serial Monitor |
| Timing tidak akurat | ESP32 clock | Reset ESP32 |
| LED redup | Resistor terlalu besar | Gunakan 220Ω |

## 📊 Perbandingan Sistem

| Aspek | 2-way | Y-shape | T-shape | 4-way |
|-------|-------|---------|---------|-------|
| **Kompleksitas** | ⭐ | ⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐ |
| **LED Required** | 6 | 9 | 9 | 12 |
| **Pin Used** | 6 | 9 | 9 | 12 |
| **Setup Time** | 15 min | 25 min | 25 min | 35 min |
| **Learning Curve** | Mudah | Sedang | Sedang | Sulit |

## 🎯 Rekomendasi Penggunaan

### Untuk Pemula
- **Mulai dengan**: `2set.ino`
- **Lanjut ke**: `pertigaan_y.ino`
- **Tujuan**: Memahami dasar state machine

### Untuk Intermediate
- **Coba**: `pertigaan_t.ino`
- **Pelajari**: Logika prioritas traffic
- **Tujuan**: Implementasi realistic timing

### Untuk Advanced
- **Implementasi**: `perempatan.ino`
- **Fitur**: Emergency mode, pedestrian crossing
- **Tujuan**: Sistem kompleks lengkap

---

**💡 Tips**: Semua sistem menggunakan logika yang sama, hanya berbeda dalam jumlah LED dan kompleksitas state machine!
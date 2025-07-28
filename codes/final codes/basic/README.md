# 📁 Basic - Sistem Lampu Lalu Lintas ESP32 (Non-IoT)

## 🎯 Overview
Folder ini berisi implementasi sistem lampu lalu lintas ESP32 versi **basic** (tanpa IoT). Ideal untuk pembelajaran, testing hardware, dan implementasi sederhana tanpa koneksi internet.

## 🚦 Sistem yang Tersedia

| File | Jenis Intersection | LED | Pin | Durasi Siklus | Kompleksitas |
|------|-------------------|-----|-----|---------------|--------------|
| `2set.ino` | Jalan Lurus 2 Arah | 6 | 6 | ~50 detik | ⭐ |
| `pertigaan_y.ino` | Pertigaan Y-Shape | 9 | 9 | ~60 detik | ⭐⭐ |
| `pertigaan_t.ino` | Pertigaan T-Shape | 9 | 9 | ~50 detik | ⭐⭐⭐ |
| `perempatan.ino` | Perempatan Lengkap | 12 | 12 | ~120 detik | ⭐⭐⭐⭐ |

## 🔧 Komponen Hardware

### Daftar Komponen
- **ESP32 Dev Kit** (1 unit)
- **LED** (sesuai jenis sistem)
- **Resistor 220Ω** (sesuai jumlah LED)
- **Push Button** (2 unit)
- **Breadboard** (1 unit)
- **Kabel Jumper** (sesuai kebutuhan)

## 📋 Detail Sistem

### 1. 🛣️ **2set.ino** - Jalan Lurus 2 Arah dengan Fase Belok

#### Koneksi Hardware
| Pin ESP32 | LED | Arah |
|-----------|-----|------|
| D23 | Hijau | Arah 1 (Utara) |
| D22 | Kuning | Arah 1 (Utara) |
| D21 | Merah | Arah 1 (Utara) |
| D19 | Hijau | Arah 2 (Selatan) |
| D18 | Kuning | Arah 2 (Selatan) |
| D5 | Merah | Arah 2 (Selatan) |

#### State Machine (9 States dengan Fase Belok)
| State | Arah 1 | Arah 2 | Durasi | Keterangan |
|-------|--------|--------|--------|------------|
| 1 | 🟢 Hijau | 🟢 Hijau | 15 detik | Lurus Bersamaan |
| 2 | 🟡 Kuning | 🟡 Kuning | 3 detik | Transisi |
| 3 | 🔴 Merah | 🔴 Merah | 2 detik | Semua Merah |
| 4 | 🟢 Hijau | 🔴 Merah | 8 detik | Belok Utara |
| 5 | 🟡 Kuning | 🔴 Merah | 3 detik | Transisi |
| 6 | 🔴 Merah | 🔴 Merah | 2 detik | Semua Merah |
| 7 | 🔴 Merah | 🟢 Hijau | 8 detik | Belok Selatan |
| 8 | 🔴 Merah | 🟡 Kuning | 3 detik | Transisi |
| 9 | 🔴 Merah | 🔴 Merah | 2 detik | Semua Merah |

#### Fitur Khusus
- **Mode Darurat**: Tombol D25 (20 detik, kuning berkedip)
- **Mode Pejalan Kaki**: Tombol D26 (15 detik, semua merah)

### 2. 🔀 **pertigaan_y.ino** - Pertigaan Y-Shape

#### Koneksi Hardware
| Pin ESP32 | LED | Jalur |
|-----------|-----|-------|
| D23 | Hijau | Jalur Utara |
| D22 | Kuning | Jalur Utara |
| D21 | Merah | Jalur Utara |
| D19 | Hijau | Jalur Selatan |
| D18 | Kuning | Jalur Selatan |
| D5 | Merah | Jalur Selatan |
| D4 | Hijau | Jalur Timur |
| D2 | Kuning | Jalur Timur |
| D15 | Merah | Jalur Timur |

#### State Machine (9 States Individual)
| State | Jalur Utara | Jalur Selatan | Jalur Timur | Durasi |
|-------|-------------|---------------|-------------|--------|
| 1 | 🟢 Hijau | 🔴 Merah | 🔴 Merah | 15 detik |
| 2 | 🟡 Kuning | 🔴 Merah | 🔴 Merah | 3 detik |
| 3 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik |
| 4 | 🔴 Merah | 🟢 Hijau | 🔴 Merah | 15 detik |
| 5 | 🔴 Merah | 🟡 Kuning | 🔴 Merah | 3 detik |
| 6 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik |
| 7 | 🔴 Merah | 🔴 Merah | 🟢 Hijau | 15 detik |
| 8 | 🔴 Merah | 🔴 Merah | 🟡 Kuning | 3 detik |
| 9 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik |

#### Fitur Khusus
- **Mode Darurat**: Tombol D25 (20 detik, kuning berkedip)
- **Mode Pejalan Kaki**: Tombol D26 (15 detik, semua merah)
- **Save State**: Menyimpan posisi sebelum interupsi

### 3. 🔀 **pertigaan_t.ino** - Pertigaan T-Shape

#### Koneksi Hardware
| Pin ESP32 | LED | Jalur |
|-----------|-----|-------|
| D23 | Hijau | Jalur Utara |
| D22 | Kuning | Jalur Utara |
| D21 | Merah | Jalur Utara |
| D19 | Hijau | Jalur Selatan |
| D18 | Kuning | Jalur Selatan |
| D5 | Merah | Jalur Selatan |
| D4 | Hijau | Jalur Timur |
| D2 | Kuning | Jalur Timur |
| D15 | Merah | Jalur Timur |

#### State Machine (6 States)
| State | Jalur Utara | Jalur Selatan | Jalur Timur | Durasi |
|-------|-------------|---------------|-------------|--------|
| 1 | 🟢 Hijau | 🟢 Hijau | 🔴 Merah | 20 detik |
| 2 | 🟡 Kuning | 🟡 Kuning | 🔴 Merah | 3 detik |
| 3 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik |
| 4 | 🔴 Merah | 🔴 Merah | 🟢 Hijau | 20 detik |
| 5 | 🔴 Merah | 🔴 Merah | 🟡 Kuning | 3 detik |
| 6 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik |

#### Fitur Khusus
- **Mode Darurat**: Tombol D25 (20 detik, kuning berkedip)
- **Mode Pejalan Kaki**: Tombol D26 (15 detik, semua merah)
- **Save State**: Menyimpan posisi sebelum interupsi

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

#### State Machine (18 States dengan Fase Belok Individual)
| State | Utara | Selatan | Timur | Barat | Durasi | Keterangan |
|-------|-------|---------|-------|-------|--------|------------|
| 1 | 🟢 Hijau | 🟢 Hijau | 🔴 Merah | 🔴 Merah | 20 detik | Lurus Utara-Selatan |
| 2 | 🟡 Kuning | 🟡 Kuning | 🔴 Merah | 🔴 Merah | 3 detik | Transisi |
| 3 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | Semua Merah |
| 4 | 🔴 Merah | 🔴 Merah | 🟢 Hijau | 🟢 Hijau | 20 detik | Lurus Timur-Barat |
| 5 | 🔴 Merah | 🔴 Merah | 🟡 Kuning | 🟡 Kuning | 3 detik | Transisi |
| 6 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | Semua Merah |
| 7 | 🟢 Hijau | 🔴 Merah | 🔴 Merah | 🔴 Merah | 10 detik | Belok Utara |
| 8 | 🟡 Kuning | 🔴 Merah | 🔴 Merah | 🔴 Merah | 3 detik | Transisi |
| 9 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | Semua Merah |
| 10 | 🔴 Merah | 🔴 Merah | 🟢 Hijau | 🔴 Merah | 10 detik | Belok Timur |
| 11 | 🔴 Merah | 🔴 Merah | 🟡 Kuning | 🔴 Merah | 3 detik | Transisi |
| 12 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | Semua Merah |
| 13 | 🔴 Merah | 🟢 Hijau | 🔴 Merah | 🔴 Merah | 10 detik | Belok Selatan |
| 14 | 🔴 Merah | 🟡 Kuning | 🔴 Merah | 🔴 Merah | 3 detik | Transisi |
| 15 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | Semua Merah |
| 16 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🟢 Hijau | 10 detik | Belok Barat |
| 17 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🟡 Kuning | 3 detik | Transisi |
| 18 | 🔴 Merah | 🔴 Merah | 🔴 Merah | 🔴 Merah | 2 detik | Semua Merah |

#### Fitur Khusus
- **Mode Darurat**: Tombol D25 (20 detik, kuning berkedip)
- **Mode Pejalan Kaki**: Tombol D26 (15 detik, semua merah)
- **Save State**: Menyimpan posisi sebelum interupsi dan melanjutkan siklus

## 🚀 Cara Penggunaan

### 1. Persiapan Hardware
1. Siapkan ESP32 dan komponen sesuai tabel
2. Hubungkan LED dengan resistor 220Ω
3. Hubungkan ke pin ESP32 sesuai diagram
4. Hubungkan tombol ke pin D25 (Darurat) dan D26 (Pejalan Kaki)

### 2. Upload Kode
1. Buka Arduino IDE
2. Pilih board ESP32
3. Upload file `.ino` yang dipilih
4. Buka Serial Monitor (115200 baud)

### 3. Testing
1. Monitor Serial Monitor untuk status
2. Periksa LED menyala sesuai state
3. Verifikasi timing sesuai spesifikasi
4. Test mode darurat dan pejalan kaki

## 🔍 Troubleshooting

### Masalah Umum
| Masalah | Penyebab | Solusi |
|---------|----------|--------|
| LED tidak menyala | Koneksi salah | Periksa wiring dan resistor |
| State tidak berubah | Kode error | Periksa Serial Monitor |
| Timing tidak akurat | ESP32 clock | Reset ESP32 |
| LED redup | Resistor terlalu besar | Gunakan 220Ω |
| Tombol tidak responsif | Debounce | Periksa koneksi dan kode debounce |

### Debug Mode
Semua sistem memiliki output Serial yang detail:
- Status state saat ini
- Durasi setiap fase
- Aktivasi mode khusus
- Error handling

## 📊 Perbandingan Sistem

| Aspek | 2-way | Y-shape | T-shape | 4-way |
|-------|-------|---------|---------|-------|
| **Kompleksitas** | ⭐ | ⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐ |
| **LED Required** | 6 | 9 | 9 | 12 |
| **Pin Used** | 6 | 9 | 9 | 12 |
| **Setup Time** | 15 min | 25 min | 25 min | 35 min |
| **Learning Curve** | Mudah | Sedang | Sedang | Sulit |
| **Fase Belok** | ✅ | ✅ | ❌ | ✅ |
| **Save State** | ❌ | ✅ | ✅ | ✅ |

## 🎯 Rekomendasi Penggunaan

### Untuk Pemula
- **Mulai dengan**: `2set.ino`
- **Lanjut ke**: `pertigaan_y.ino`
- **Tujuan**: Memahami dasar state machine dan fase belok

### Untuk Intermediate
- **Coba**: `pertigaan_t.ino`
- **Pelajari**: Logika prioritas traffic dan save state
- **Tujuan**: Implementasi realistic timing dengan interupsi

### Untuk Advanced
- **Implementasi**: `perempatan.ino`
- **Fitur**: Emergency mode, pedestrian crossing, save state
- **Tujuan**: Sistem kompleks lengkap dengan semua fitur

## 🔧 Fitur Teknis

### State Management
- **Non-blocking**: Menggunakan `millis()` untuk timing
- **Debouncing**: 50ms untuk tombol input
- **Save State**: Menyimpan posisi sebelum interupsi
- **Priority System**: Emergency > Pedestrian > Normal

### Timing System
- **Configurable**: Durasi dapat diubah di bagian konstanta
- **Realistic**: Timing sesuai standar traffic light
- **Flexible**: Mudah disesuaikan untuk kebutuhan berbeda

### Error Handling
- **Safe State**: Selalu mulai dari state merah
- **Recovery**: Otomatis kembali ke siklus normal
- **Debug**: Output Serial untuk monitoring

---

**💡 Tips**: Semua sistem menggunakan logika yang sama, hanya berbeda dalam jumlah LED dan kompleksitas state machine. Fitur save state memastikan siklus berlanjut dengan benar setelah interupsi!
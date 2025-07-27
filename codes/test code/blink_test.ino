/*
 * Blink Test - ESP32
 * Program sederhana untuk menguji koneksi LED
 *
 * Komponen:
 * - ESP32 Dev Kit
 * - 12x LED (4x Merah, 4x Kuning, 4x Hijau) - untuk perempatan
 * - 9x LED (3x Merah, 3x Kuning, 3x Hijau) - untuk pertigaan  
 * - 6x LED (2x Merah, 2x Kuning, 2x Hijau) - untuk jalan lurus
 * - Resistor 220Ω sesuai jumlah LED
 * - Breadboard & Kabel Jumper
 *
 * Koneksi untuk Perempatan (4 Set Lampu):
 * - Pin D23 -> LED Hijau Utara
 * - Pin D22 -> LED Kuning Utara
 * - Pin D21 -> LED Merah Utara
 * - Pin D19 -> LED Hijau Selatan
 * - Pin D18 -> LED Kuning Selatan
 * - Pin D5  -> LED Merah Selatan
 * - Pin D4  -> LED Hijau Timur
 * - Pin D2  -> LED Kuning Timur
 * - Pin D15 -> LED Merah Timur
 * - Pin D0  -> LED Hijau Barat
 * - Pin D16 -> LED Kuning Barat
 * - Pin D17 -> LED Merah Barat
 *
 * Koneksi untuk Pertigaan (3 Set Lampu):
 * - Pin D23 -> LED Hijau Jalur Utama
 * - Pin D22 -> LED Kuning Jalur Utama
 * - Pin D21 -> LED Merah Jalur Utama
 * - Pin D19 -> LED Hijau Jalur Cabang 1
 * - Pin D18 -> LED Kuning Jalur Cabang 1
 * - Pin D5  -> LED Merah Jalur Cabang 1
 * - Pin D4  -> LED Hijau Jalur Cabang 2
 * - Pin D2  -> LED Kuning Jalur Cabang 2
 * - Pin D15 -> LED Merah Jalur Cabang 2
 *
 * Koneksi untuk Jalan Lurus (2 Set Lampu):
 * - Pin D23 -> LED Hijau Arah 1
 * - Pin D22 -> LED Kuning Arah 1
 * - Pin D21 -> LED Merah Arah 1
 * - Pin D19 -> LED Hijau Arah 2
 * - Pin D18 -> LED Kuning Arah 2
 * - Pin D5  -> LED Merah Arah 2
 */

// Pin LED - Semua konfigurasi
// Perempatan (4 Set Lampu)
const int PIN_LED_HIJAU_UTARA = 23;
const int PIN_LED_KUNING_UTARA = 22;
const int PIN_LED_MERAH_UTARA = 21;
const int PIN_LED_HIJAU_SELATAN = 19;
const int PIN_LED_KUNING_SELATAN = 18;
const int PIN_LED_MERAH_SELATAN = 5;
const int PIN_LED_HIJAU_TIMUR = 4;
const int PIN_LED_KUNING_TIMUR = 2;
const int PIN_LED_MERAH_TIMUR = 15;
const int PIN_LED_HIJAU_BARAT = 0;
const int PIN_LED_KUNING_BARAT = 16;
const int PIN_LED_MERAH_BARAT = 17;

// Pertigaan (3 Set Lampu)
const int PIN_LED_HIJAU_UTAMA = 23;
const int PIN_LED_KUNING_UTAMA = 22;
const int PIN_LED_MERAH_UTAMA = 21;
const int PIN_LED_HIJAU_CABANG1 = 19;
const int PIN_LED_KUNING_CABANG1 = 18;
const int PIN_LED_MERAH_CABANG1 = 5;
const int PIN_LED_HIJAU_CABANG2 = 4;
const int PIN_LED_KUNING_CABANG2 = 2;
const int PIN_LED_MERAH_CABANG2 = 15;

// Jalan Lurus (2 Set Lampu)
const int PIN_LED_HIJAU_ARAH1 = 23;
const int PIN_LED_KUNING_ARAH1 = 22;
const int PIN_LED_MERAH_ARAH1 = 21;
const int PIN_LED_HIJAU_ARAH2 = 19;
const int PIN_LED_KUNING_ARAH2 = 18;
const int PIN_LED_MERAH_ARAH2 = 5;

// Array semua pin LED untuk memudahkan testing
const int allLEDs[] = {
  PIN_LED_HIJAU_UTARA, PIN_LED_KUNING_UTARA, PIN_LED_MERAH_UTARA,
  PIN_LED_HIJAU_SELATAN, PIN_LED_KUNING_SELATAN, PIN_LED_MERAH_SELATAN,
  PIN_LED_HIJAU_TIMUR, PIN_LED_KUNING_TIMUR, PIN_LED_MERAH_TIMUR,
  PIN_LED_HIJAU_BARAT, PIN_LED_KUNING_BARAT, PIN_LED_MERAH_BARAT
};

const int numLEDs = sizeof(allLEDs) / sizeof(allLEDs[0]);

void setup() {
  Serial.begin(115200);
  Serial.println("=== Blink Test ESP32 ===");
  
  // Setup semua pin LED sebagai output
  for (int i = 0; i < numLEDs; i++) {
    pinMode(allLEDs[i], OUTPUT);
  }
  
  // Matikan semua LED di awal
  turnOffAllLights();
  
  Serial.println("Setup selesai. Memulai blink test...");
}

void loop() {
  Serial.println("=== Test 1: Blink Semua LED ===");
  blinkAllLEDs(3); // Blink 3 kali
  
  delay(1000);
  
  Serial.println("=== Test 2: Blink Berurutan ===");
  blinkSequential();
  
  delay(1000);
  
  Serial.println("=== Test 3: Animasi Warna ===");
  colorAnimation();
  
  delay(1000);
  
  Serial.println("=== Test 4: Animasi Wave ===");
  waveAnimation();
  
  delay(1000);
  
  Serial.println("=== Test 5: Animasi Chase ===");
  chaseAnimation();
  
  delay(1000);
  
  Serial.println("=== Test 6: Simulasi Lampu Lalu Lintas ===");
  simulateTrafficLight();
  
  delay(2000);
}

void blinkAllLEDs(int times) {
  for (int t = 0; t < times; t++) {
    Serial.println("Blink " + String(t + 1) + "/" + String(times));
    
    // Nyalakan semua LED
    for (int i = 0; i < numLEDs; i++) {
      digitalWrite(allLEDs[i], HIGH);
    }
    delay(500);
    
    // Matikan semua LED
    for (int i = 0; i < numLEDs; i++) {
      digitalWrite(allLEDs[i], LOW);
    }
    delay(500);
  }
}

void blinkSequential() {
  for (int i = 0; i < numLEDs; i++) {
    Serial.println("Testing LED pada pin " + String(allLEDs[i]));
    digitalWrite(allLEDs[i], HIGH);
    delay(300);
    digitalWrite(allLEDs[i], LOW);
    delay(200);
  }
}

void simulateTrafficLight() {
  Serial.println("=== Simulasi Perempatan ===");
  Serial.println("Simulasi: Utara & Selatan Hijau, Timur & Barat Merah");
  digitalWrite(PIN_LED_HIJAU_UTARA, HIGH);
  digitalWrite(PIN_LED_HIJAU_SELATAN, HIGH);
  digitalWrite(PIN_LED_MERAH_TIMUR, HIGH);
  digitalWrite(PIN_LED_MERAH_BARAT, HIGH);
  delay(2000);

  Serial.println("Simulasi: Utara & Selatan Kuning, Timur & Barat Merah");
  digitalWrite(PIN_LED_HIJAU_UTARA, LOW);
  digitalWrite(PIN_LED_HIJAU_SELATAN, LOW);
  digitalWrite(PIN_LED_KUNING_UTARA, HIGH);
  digitalWrite(PIN_LED_KUNING_SELATAN, HIGH);
  delay(1000);

  Serial.println("Simulasi: Utara & Selatan Merah, Timur & Barat Hijau");
  digitalWrite(PIN_LED_KUNING_UTARA, LOW);
  digitalWrite(PIN_LED_KUNING_SELATAN, LOW);
  digitalWrite(PIN_LED_MERAH_UTARA, HIGH);
  digitalWrite(PIN_LED_MERAH_SELATAN, HIGH);
  digitalWrite(PIN_LED_MERAH_TIMUR, LOW);
  digitalWrite(PIN_LED_MERAH_BARAT, LOW);
  digitalWrite(PIN_LED_HIJAU_TIMUR, HIGH);
  digitalWrite(PIN_LED_HIJAU_BARAT, HIGH);
  delay(2000);

  Serial.println("Simulasi: Utara & Selatan Merah, Timur & Barat Kuning");
  digitalWrite(PIN_LED_HIJAU_TIMUR, LOW);
  digitalWrite(PIN_LED_HIJAU_BARAT, LOW);
  digitalWrite(PIN_LED_KUNING_TIMUR, HIGH);
  digitalWrite(PIN_LED_KUNING_BARAT, HIGH);
  delay(1000);

  // Matikan semua
  turnOffAllLights();
  delay(1000);

  Serial.println("=== Simulasi Pertigaan ===");
  Serial.println("Simulasi: Jalur Utama Hijau, Cabang 1 & 2 Merah");
  digitalWrite(PIN_LED_HIJAU_UTAMA, HIGH);
  digitalWrite(PIN_LED_MERAH_CABANG1, HIGH);
  digitalWrite(PIN_LED_MERAH_CABANG2, HIGH);
  delay(2000);

  Serial.println("Simulasi: Jalur Utama Merah, Cabang 1 Hijau, Cabang 2 Merah");
  digitalWrite(PIN_LED_HIJAU_UTAMA, LOW);
  digitalWrite(PIN_LED_MERAH_UTAMA, HIGH);
  digitalWrite(PIN_LED_MERAH_CABANG1, LOW);
  digitalWrite(PIN_LED_HIJAU_CABANG1, HIGH);
  delay(2000);

  Serial.println("Simulasi: Jalur Utama Merah, Cabang 1 Merah, Cabang 2 Hijau");
  digitalWrite(PIN_LED_HIJAU_CABANG1, LOW);
  digitalWrite(PIN_LED_MERAH_CABANG1, HIGH);
  digitalWrite(PIN_LED_MERAH_CABANG2, LOW);
  digitalWrite(PIN_LED_HIJAU_CABANG2, HIGH);
  delay(2000);

  // Matikan semua
  turnOffAllLights();
  delay(1000);

  Serial.println("=== Simulasi Jalan Lurus ===");
  Serial.println("Simulasi: Arah 1 Hijau, Arah 2 Merah");
  digitalWrite(PIN_LED_HIJAU_ARAH1, HIGH);
  digitalWrite(PIN_LED_MERAH_ARAH2, HIGH);
  delay(2000);

  Serial.println("Simulasi: Arah 1 Merah, Arah 2 Hijau");
  digitalWrite(PIN_LED_HIJAU_ARAH1, LOW);
  digitalWrite(PIN_LED_MERAH_ARAH1, HIGH);
  digitalWrite(PIN_LED_MERAH_ARAH2, LOW);
  digitalWrite(PIN_LED_HIJAU_ARAH2, HIGH);
  delay(2000);

  // Matikan semua
  turnOffAllLights();
}

void turnOffAllLights() {
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(allLEDs[i], LOW);
  }
}

// ===== ANIMASI BARU =====

void colorAnimation() {
  Serial.println("Animasi: Semua LED Merah");
  turnOffAllLights();
  for (int i = 0; i < numLEDs; i++) {
    if (allLEDs[i] == PIN_LED_MERAH_UTARA || allLEDs[i] == PIN_LED_MERAH_SELATAN || 
        allLEDs[i] == PIN_LED_MERAH_TIMUR || allLEDs[i] == PIN_LED_MERAH_BARAT ||
        allLEDs[i] == PIN_LED_MERAH_UTAMA || allLEDs[i] == PIN_LED_MERAH_CABANG1 || 
        allLEDs[i] == PIN_LED_MERAH_CABANG2 || allLEDs[i] == PIN_LED_MERAH_ARAH1 || 
        allLEDs[i] == PIN_LED_MERAH_ARAH2) {
      digitalWrite(allLEDs[i], HIGH);
    }
  }
  delay(2000);
  
  Serial.println("Animasi: Semua LED Kuning");
  turnOffAllLights();
  for (int i = 0; i < numLEDs; i++) {
    if (allLEDs[i] == PIN_LED_KUNING_UTARA || allLEDs[i] == PIN_LED_KUNING_SELATAN || 
        allLEDs[i] == PIN_LED_KUNING_TIMUR || allLEDs[i] == PIN_LED_KUNING_BARAT ||
        allLEDs[i] == PIN_LED_KUNING_UTAMA || allLEDs[i] == PIN_LED_KUNING_CABANG1 || 
        allLEDs[i] == PIN_LED_KUNING_CABANG2 || allLEDs[i] == PIN_LED_KUNING_ARAH1 || 
        allLEDs[i] == PIN_LED_KUNING_ARAH2) {
      digitalWrite(allLEDs[i], HIGH);
    }
  }
  delay(2000);
  
  Serial.println("Animasi: Semua LED Hijau");
  turnOffAllLights();
  for (int i = 0; i < numLEDs; i++) {
    if (allLEDs[i] == PIN_LED_HIJAU_UTARA || allLEDs[i] == PIN_LED_HIJAU_SELATAN || 
        allLEDs[i] == PIN_LED_HIJAU_TIMUR || allLEDs[i] == PIN_LED_HIJAU_BARAT ||
        allLEDs[i] == PIN_LED_HIJAU_UTAMA || allLEDs[i] == PIN_LED_HIJAU_CABANG1 || 
        allLEDs[i] == PIN_LED_HIJAU_CABANG2 || allLEDs[i] == PIN_LED_HIJAU_ARAH1 || 
        allLEDs[i] == PIN_LED_HIJAU_ARAH2) {
      digitalWrite(allLEDs[i], HIGH);
    }
  }
  delay(2000);
  
  turnOffAllLights();
}

void waveAnimation() {
  Serial.println("Animasi Wave: Merah -> Kuning -> Hijau");
  
  // Wave Merah
  for (int i = 0; i < numLEDs; i++) {
    if (allLEDs[i] == PIN_LED_MERAH_UTARA || allLEDs[i] == PIN_LED_MERAH_SELATAN || 
        allLEDs[i] == PIN_LED_MERAH_TIMUR || allLEDs[i] == PIN_LED_MERAH_BARAT ||
        allLEDs[i] == PIN_LED_MERAH_UTAMA || allLEDs[i] == PIN_LED_MERAH_CABANG1 || 
        allLEDs[i] == PIN_LED_MERAH_CABANG2 || allLEDs[i] == PIN_LED_MERAH_ARAH1 || 
        allLEDs[i] == PIN_LED_MERAH_ARAH2) {
      digitalWrite(allLEDs[i], HIGH);
      delay(100);
    }
  }
  delay(500);
  
  // Wave Kuning
  turnOffAllLights();
  for (int i = 0; i < numLEDs; i++) {
    if (allLEDs[i] == PIN_LED_KUNING_UTARA || allLEDs[i] == PIN_LED_KUNING_SELATAN || 
        allLEDs[i] == PIN_LED_KUNING_TIMUR || allLEDs[i] == PIN_LED_KUNING_BARAT ||
        allLEDs[i] == PIN_LED_KUNING_UTAMA || allLEDs[i] == PIN_LED_KUNING_CABANG1 || 
        allLEDs[i] == PIN_LED_KUNING_CABANG2 || allLEDs[i] == PIN_LED_KUNING_ARAH1 || 
        allLEDs[i] == PIN_LED_KUNING_ARAH2) {
      digitalWrite(allLEDs[i], HIGH);
      delay(100);
    }
  }
  delay(500);
  
  // Wave Hijau
  turnOffAllLights();
  for (int i = 0; i < numLEDs; i++) {
    if (allLEDs[i] == PIN_LED_HIJAU_UTARA || allLEDs[i] == PIN_LED_HIJAU_SELATAN || 
        allLEDs[i] == PIN_LED_HIJAU_TIMUR || allLEDs[i] == PIN_LED_HIJAU_BARAT ||
        allLEDs[i] == PIN_LED_HIJAU_UTAMA || allLEDs[i] == PIN_LED_HIJAU_CABANG1 || 
        allLEDs[i] == PIN_LED_HIJAU_CABANG2 || allLEDs[i] == PIN_LED_HIJAU_ARAH1 || 
        allLEDs[i] == PIN_LED_HIJAU_ARAH2) {
      digitalWrite(allLEDs[i], HIGH);
      delay(100);
    }
  }
  delay(500);
  
  turnOffAllLights();
}

void chaseAnimation() {
  Serial.println("Animasi Chase: LED bergerak berurutan");
  
  // Chase Merah
  for (int i = 0; i < numLEDs; i++) {
    if (allLEDs[i] == PIN_LED_MERAH_UTARA || allLEDs[i] == PIN_LED_MERAH_SELATAN || 
        allLEDs[i] == PIN_LED_MERAH_TIMUR || allLEDs[i] == PIN_LED_MERAH_BARAT ||
        allLEDs[i] == PIN_LED_MERAH_UTAMA || allLEDs[i] == PIN_LED_MERAH_CABANG1 || 
        allLEDs[i] == PIN_LED_MERAH_CABANG2 || allLEDs[i] == PIN_LED_MERAH_ARAH1 || 
        allLEDs[i] == PIN_LED_MERAH_ARAH2) {
      digitalWrite(allLEDs[i], HIGH);
      delay(200);
      digitalWrite(allLEDs[i], LOW);
    }
  }
  
  // Chase Kuning
  for (int i = 0; i < numLEDs; i++) {
    if (allLEDs[i] == PIN_LED_KUNING_UTARA || allLEDs[i] == PIN_LED_KUNING_SELATAN || 
        allLEDs[i] == PIN_LED_KUNING_TIMUR || allLEDs[i] == PIN_LED_KUNING_BARAT ||
        allLEDs[i] == PIN_LED_KUNING_UTAMA || allLEDs[i] == PIN_LED_KUNING_CABANG1 || 
        allLEDs[i] == PIN_LED_KUNING_CABANG2 || allLEDs[i] == PIN_LED_KUNING_ARAH1 || 
        allLEDs[i] == PIN_LED_KUNING_ARAH2) {
      digitalWrite(allLEDs[i], HIGH);
      delay(200);
      digitalWrite(allLEDs[i], LOW);
    }
  }
  
  // Chase Hijau
  for (int i = 0; i < numLEDs; i++) {
    if (allLEDs[i] == PIN_LED_HIJAU_UTARA || allLEDs[i] == PIN_LED_HIJAU_SELATAN || 
        allLEDs[i] == PIN_LED_HIJAU_TIMUR || allLEDs[i] == PIN_LED_HIJAU_BARAT ||
        allLEDs[i] == PIN_LED_HIJAU_UTAMA || allLEDs[i] == PIN_LED_HIJAU_CABANG1 || 
        allLEDs[i] == PIN_LED_HIJAU_CABANG2 || allLEDs[i] == PIN_LED_HIJAU_ARAH1 || 
        allLEDs[i] == PIN_LED_HIJAU_ARAH2) {
      digitalWrite(allLEDs[i], HIGH);
      delay(200);
      digitalWrite(allLEDs[i], LOW);
    }
  }
}

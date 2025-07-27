/*
 * Proyek Lampu Lalu Lintas - Pertigaan Y-Shape (3-way Y-Intersection)
 * ESP32 Basic Version (Non-IoT)
 * 
 * Sistem traffic light pertigaan Y-shape yang realistis dengan fitur:
 * - 3 arah dengan traffic yang seimbang
 * - Timing yang realistis untuk traffic flow
 * - Support untuk belok kiri dan lurus
 * - Pedestrian crossing consideration
 * 
 * Komponen:
 * - ESP32 Dev Kit
 * - 9x LED (3x Merah, 3x Kuning, 3x Hijau)
 * - 9x Resistor 220Ω
 * - Breadboard & Kabel Jumper
 * 
 * Koneksi:
 * - Pin D23 -> LED Hijau Jalur Utama
 * - Pin D22 -> LED Kuning Jalur Utama  
 * - Pin D21 -> LED Merah Jalur Utama
 * - Pin D19 -> LED Hijau Jalur Cabang 1
 * - Pin D18 -> LED Kuning Jalur Cabang 1
 * - Pin D5  -> LED Merah Jalur Cabang 1
 * - Pin D4  -> LED Hijau Jalur Cabang 2
 * - Pin D2  -> LED Kuning Jalur Cabang 2
 * - Pin D15 -> LED Merah Jalur Cabang 2
 */

// Pin LED - 3 Set Lampu Lalu Lintas
// Set 1: Jalur Utama
const int PIN_LED_HIJAU_UTAMA = 23;
const int PIN_LED_KUNING_UTAMA = 22;
const int PIN_LED_MERAH_UTAMA = 21;

// Set 2: Jalur Cabang 1
const int PIN_LED_HIJAU_CABANG1 = 19;
const int PIN_LED_KUNING_CABANG1 = 18;
const int PIN_LED_MERAH_CABANG1 = 5;

// Set 3: Jalur Cabang 2
const int PIN_LED_HIJAU_CABANG2 = 4;
const int PIN_LED_KUNING_CABANG2 = 2;
const int PIN_LED_MERAH_CABANG2 = 15;

// State machine untuk lampu lalu lintas pertigaan Y-shape
enum TrafficState {
  STATE_1, // Jalur Utama Hijau (Lurus & Belok Kanan), Cabang 1 & 2 Merah
  STATE_2, // Jalur Utama Kuning, Cabang 1 & 2 Merah
  STATE_3, // Semua Merah (Transisi)
  STATE_4, // Cabang 1 Hijau (Lurus & Belok Kanan), Jalur Utama & Cabang 2 Merah
  STATE_5, // Cabang 1 Kuning, Jalur Utama & Cabang 2 Merah
  STATE_6, // Semua Merah (Transisi)
  STATE_7, // Cabang 2 Hijau (Lurus & Belok Kanan), Jalur Utama & Cabang 1 Merah
  STATE_8, // Cabang 2 Kuning, Jalur Utama & Cabang 1 Merah
  STATE_9  // Semua Merah (Transisi)
};

TrafficState currentState = STATE_1;
unsigned long lastStateChange = 0;

// Durasi setiap state (dalam milidetik) - Realistic timing untuk Y-shape
const unsigned long DURASI_HIJAU_UTAMA = 30000;    // 30 detik - Jalur utama
const unsigned long DURASI_HIJAU_CABANG1 = 25000;  // 25 detik - Cabang 1
const unsigned long DURASI_HIJAU_CABANG2 = 25000;  // 25 detik - Cabang 2
const unsigned long DURASI_KUNING = 4000;          // 4 detik
const unsigned long DURASI_TRANSISI = 2000;        // 2 detik

void setup() {
  Serial.begin(115200);
  Serial.println("=== Proyek Lampu Lalu Lintas Pertigaan Y-Shape (Basic) ===");
  
  // Setup pin LED sebagai output - 3 Set Lampu
  // Set 1: Jalur Utama
  pinMode(PIN_LED_HIJAU_UTAMA, OUTPUT);
  pinMode(PIN_LED_KUNING_UTAMA, OUTPUT);
  pinMode(PIN_LED_MERAH_UTAMA, OUTPUT);
  
  // Set 2: Jalur Cabang 1
  pinMode(PIN_LED_HIJAU_CABANG1, OUTPUT);
  pinMode(PIN_LED_KUNING_CABANG1, OUTPUT);
  pinMode(PIN_LED_MERAH_CABANG1, OUTPUT);
  
  // Set 3: Jalur Cabang 2
  pinMode(PIN_LED_HIJAU_CABANG2, OUTPUT);
  pinMode(PIN_LED_KUNING_CABANG2, OUTPUT);
  pinMode(PIN_LED_MERAH_CABANG2, OUTPUT);
  
  // Matikan semua LED di awal
  turnOffAllLights();
  
  // Set initial state lights
  turnOnLights(PIN_LED_HIJAU_UTAMA, PIN_LED_MERAH_CABANG1, PIN_LED_MERAH_CABANG2);
  
  Serial.println("Setup selesai. Memulai sistem lampu lalu lintas pertigaan Y-shape...");
  Serial.println("State 1: Jalur Utama Hijau, Cabang 1 & 2 Merah");
}

void loop() {
  // Logika state machine non-blocking
  unsigned long currentTime = millis();
  
  if (currentTime - lastStateChange >= getStateDuration()) {
    changeToNextState();
    lastStateChange = currentTime;
  }
}

unsigned long getStateDuration() {
  switch (currentState) {
    case STATE_1: return DURASI_HIJAU_UTAMA;
    case STATE_2: return DURASI_KUNING;
    case STATE_3: return DURASI_TRANSISI;
    case STATE_4: return DURASI_HIJAU_CABANG1;
    case STATE_5: return DURASI_KUNING;
    case STATE_6: return DURASI_TRANSISI;
    case STATE_7: return DURASI_HIJAU_CABANG2;
    case STATE_8: return DURASI_KUNING;
    case STATE_9: return DURASI_TRANSISI;
    default: return 5000;
  }
}

void changeToNextState() {
  // Matikan semua lampu
  turnOffAllLights();
  
  // Ubah ke state berikutnya
  switch (currentState) {
    case STATE_1:
      currentState = STATE_2;
      // Jalur Utama Kuning, Cabang 1 & 2 Merah
      turnOnLights(PIN_LED_KUNING_UTAMA, PIN_LED_MERAH_CABANG1, PIN_LED_MERAH_CABANG2);
      Serial.println("State 2: Jalur Utama Kuning, Cabang 1 & 2 Merah");
      break;
      
    case STATE_2:
      currentState = STATE_3;
      // Semua Merah (Transisi)
      turnOnLights(PIN_LED_MERAH_UTAMA, PIN_LED_MERAH_CABANG1, PIN_LED_MERAH_CABANG2);
      Serial.println("State 3: Semua Merah (Transisi)");
      break;
      
    case STATE_3:
      currentState = STATE_4;
      // Cabang 1 Hijau, Jalur Utama & Cabang 2 Merah
      turnOnLights(PIN_LED_MERAH_UTAMA, PIN_LED_HIJAU_CABANG1, PIN_LED_MERAH_CABANG2);
      Serial.println("State 4: Cabang 1 Hijau, Jalur Utama & Cabang 2 Merah");
      break;
      
    case STATE_4:
      currentState = STATE_5;
      // Cabang 1 Kuning, Jalur Utama & Cabang 2 Merah
      turnOnLights(PIN_LED_MERAH_UTAMA, PIN_LED_KUNING_CABANG1, PIN_LED_MERAH_CABANG2);
      Serial.println("State 5: Cabang 1 Kuning, Jalur Utama & Cabang 2 Merah");
      break;
      
    case STATE_5:
      currentState = STATE_6;
      // Semua Merah (Transisi)
      turnOnLights(PIN_LED_MERAH_UTAMA, PIN_LED_MERAH_CABANG1, PIN_LED_MERAH_CABANG2);
      Serial.println("State 6: Semua Merah (Transisi)");
      break;
      
    case STATE_6:
      currentState = STATE_7;
      // Cabang 2 Hijau, Jalur Utama & Cabang 1 Merah
      turnOnLights(PIN_LED_MERAH_UTAMA, PIN_LED_MERAH_CABANG1, PIN_LED_HIJAU_CABANG2);
      Serial.println("State 7: Cabang 2 Hijau, Jalur Utama & Cabang 1 Merah");
      break;
      
    case STATE_7:
      currentState = STATE_8;
      // Cabang 2 Kuning, Jalur Utama & Cabang 1 Merah
      turnOnLights(PIN_LED_MERAH_UTAMA, PIN_LED_MERAH_CABANG1, PIN_LED_KUNING_CABANG2);
      Serial.println("State 8: Cabang 2 Kuning, Jalur Utama & Cabang 1 Merah");
      break;
      
    case STATE_8:
      currentState = STATE_9;
      // Semua Merah (Transisi)
      turnOnLights(PIN_LED_MERAH_UTAMA, PIN_LED_MERAH_CABANG1, PIN_LED_MERAH_CABANG2);
      Serial.println("State 9: Semua Merah (Transisi)");
      break;
      
    case STATE_9:
      currentState = STATE_1;
      // Jalur Utama Hijau, Cabang 1 & 2 Merah
      turnOnLights(PIN_LED_HIJAU_UTAMA, PIN_LED_MERAH_CABANG1, PIN_LED_MERAH_CABANG2);
      Serial.println("State 1: Jalur Utama Hijau, Cabang 1 & 2 Merah");
      break;
  }
}

void turnOffAllLights() {
  // Matikan semua LED dari 3 set lampu
  digitalWrite(PIN_LED_HIJAU_UTAMA, LOW);
  digitalWrite(PIN_LED_KUNING_UTAMA, LOW);
  digitalWrite(PIN_LED_MERAH_UTAMA, LOW);
  
  digitalWrite(PIN_LED_HIJAU_CABANG1, LOW);
  digitalWrite(PIN_LED_KUNING_CABANG1, LOW);
  digitalWrite(PIN_LED_MERAH_CABANG1, LOW);
  
  digitalWrite(PIN_LED_HIJAU_CABANG2, LOW);
  digitalWrite(PIN_LED_KUNING_CABANG2, LOW);
  digitalWrite(PIN_LED_MERAH_CABANG2, LOW);
}

void turnOnLights(int pin1, int pin2, int pin3) {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, HIGH);
} 
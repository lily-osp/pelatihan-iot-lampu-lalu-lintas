/*
 * Proyek Lampu Lalu Lintas - Perempatan Complete (4-way Traffic)
 * ESP32 Basic Version (Non-IoT)
 * 
 * Sistem traffic light perempatan yang lengkap dengan fitur:
 * - 4 arah individual (Utara, Selatan, Timur, Barat)
 * - Support untuk belok kiri dan lurus
 * - Timing yang realistis untuk traffic flow
 * - Emergency mode dan pedestrian crossing
 * 
 * Komponen:
 * - ESP32 Dev Kit
 * - 12x LED (4x Merah, 4x Kuning, 4x Hijau)
 * - 12x Resistor 220Ω
 * - Breadboard & Kabel Jumper
 * 
 * Koneksi untuk Perempatan (4 Set Lampu):
 * Set 1: Arah Utara
 * - Pin D23 -> LED Hijau Utara
 * - Pin D22 -> LED Kuning Utara  
 * - Pin D21 -> LED Merah Utara
 * 
 * Set 2: Arah Selatan
 * - Pin D19 -> LED Hijau Selatan
 * - Pin D18 -> LED Kuning Selatan
 * - Pin D5  -> LED Merah Selatan
 * 
 * Set 3: Arah Timur
 * - Pin D4  -> LED Hijau Timur
 * - Pin D2  -> LED Kuning Timur
 * - Pin D15 -> LED Merah Timur
 * 
 * Set 4: Arah Barat
 * - Pin D0  -> LED Hijau Barat
 * - Pin D16 -> LED Kuning Barat
 * - Pin D17 -> LED Merah Barat
 * 
 * Logika: Sistem mendukung individual green lights untuk belok kiri/kanan
 * dan timing yang realistis untuk traffic flow
 */

// Pin LED - 4 Set Lampu Lalu Lintas Perempatan
// Set 1: Arah Utara
const int PIN_LED_HIJAU_UTARA = 23;
const int PIN_LED_KUNING_UTARA = 22;
const int PIN_LED_MERAH_UTARA = 21;

// Set 2: Arah Selatan
const int PIN_LED_HIJAU_SELATAN = 19;
const int PIN_LED_KUNING_SELATAN = 18;
const int PIN_LED_MERAH_SELATAN = 5;

// Set 3: Arah Timur
const int PIN_LED_HIJAU_TIMUR = 4;
const int PIN_LED_KUNING_TIMUR = 2;
const int PIN_LED_MERAH_TIMUR = 15;

// Set 4: Arah Barat
const int PIN_LED_HIJAU_BARAT = 0;
const int PIN_LED_KUNING_BARAT = 16;
const int PIN_LED_MERAH_BARAT = 17;

// State machine untuk lampu lalu lintas perempatan complete
enum TrafficState {
  STATE_1,  // Utara & Selatan Hijau (Lurus & Belok Kanan), Timur & Barat Merah
  STATE_2,  // Utara & Selatan Kuning, Timur & Barat Merah
  STATE_3,  // Semua Merah (Transisi)
  STATE_4,  // Timur & Barat Hijau (Lurus & Belok Kanan), Utara & Selatan Merah
  STATE_5,  // Timur & Barat Kuning, Utara & Selatan Merah
  STATE_6,  // Semua Merah (Transisi)
  STATE_7,  // Individual: Utara Hijau (Belok Kiri), Lainnya Merah
  STATE_8,  // Individual: Timur Hijau (Belok Kiri), Lainnya Merah
  STATE_9,  // Individual: Selatan Hijau (Belok Kiri), Lainnya Merah
  STATE_10, // Individual: Barat Hijau (Belok Kiri), Lainnya Merah
  STATE_11, // Emergency Mode: Semua Merah
  STATE_12  // Pedestrian Mode: Semua Merah
};

TrafficState currentState = STATE_1;
unsigned long lastStateChange = 0;

// Durasi setiap state (dalam milidetik) - Realistic timing
const unsigned long DURASI_HIJAU_UTARA_SELATAN = 30000;  // 30 detik
const unsigned long DURASI_HIJAU_TIMUR_BARAT = 25000;    // 25 detik
const unsigned long DURASI_KUNING = 4000;                // 4 detik
const unsigned long DURASI_TRANSISI = 2000;              // 2 detik
const unsigned long DURASI_INDIVIDUAL = 10000;           // 10 detik untuk belok individual
const unsigned long DURASI_EMERGENCY = 5000;             // 5 detik emergency
const unsigned long DURASI_PEDESTRIAN = 15000;           // 15 detik pedestrian

// Mode flags
bool emergencyMode = false;
bool pedestrianMode = false;

void setup() {
  Serial.begin(115200);
  Serial.println("=== Proyek Lampu Lalu Lintas Perempatan Complete (Basic) ===");
  
  // Setup pin LED sebagai output - 4 Set Lampu Perempatan
  // Set 1: Arah Utara
  pinMode(PIN_LED_HIJAU_UTARA, OUTPUT);
  pinMode(PIN_LED_KUNING_UTARA, OUTPUT);
  pinMode(PIN_LED_MERAH_UTARA, OUTPUT);
  
  // Set 2: Arah Selatan
  pinMode(PIN_LED_HIJAU_SELATAN, OUTPUT);
  pinMode(PIN_LED_KUNING_SELATAN, OUTPUT);
  pinMode(PIN_LED_MERAH_SELATAN, OUTPUT);
  
  // Set 3: Arah Timur
  pinMode(PIN_LED_HIJAU_TIMUR, OUTPUT);
  pinMode(PIN_LED_KUNING_TIMUR, OUTPUT);
  pinMode(PIN_LED_MERAH_TIMUR, OUTPUT);
  
  // Set 4: Arah Barat
  pinMode(PIN_LED_HIJAU_BARAT, OUTPUT);
  pinMode(PIN_LED_KUNING_BARAT, OUTPUT);
  pinMode(PIN_LED_MERAH_BARAT, OUTPUT);
  
  // Matikan semua LED di awal
  turnOffAllLights();
  
  // Set initial state lights
  turnOnLights(PIN_LED_HIJAU_UTARA, PIN_LED_HIJAU_SELATAN, PIN_LED_MERAH_TIMUR, PIN_LED_MERAH_BARAT);
  
  Serial.println("Setup selesai. Memulai sistem lampu lalu lintas perempatan complete...");
  Serial.println("State 1: Utara & Selatan Hijau, Timur & Barat Merah");
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
    case STATE_1: return DURASI_HIJAU_UTARA_SELATAN;
    case STATE_2: return DURASI_KUNING;
    case STATE_3: return DURASI_TRANSISI;
    case STATE_4: return DURASI_HIJAU_TIMUR_BARAT;
    case STATE_5: return DURASI_KUNING;
    case STATE_6: return DURASI_TRANSISI;
    case STATE_7: return DURASI_INDIVIDUAL;
    case STATE_8: return DURASI_INDIVIDUAL;
    case STATE_9: return DURASI_INDIVIDUAL;
    case STATE_10: return DURASI_INDIVIDUAL;
    case STATE_11: return DURASI_EMERGENCY;
    case STATE_12: return DURASI_PEDESTRIAN;
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
      // Utara & Selatan Kuning, Timur & Barat Merah
      turnOnLights(PIN_LED_KUNING_UTARA, PIN_LED_KUNING_SELATAN, PIN_LED_MERAH_TIMUR, PIN_LED_MERAH_BARAT);
      Serial.println("State 2: Utara & Selatan Kuning, Timur & Barat Merah");
      break;
      
    case STATE_2:
      currentState = STATE_3;
      // Semua Merah (Transisi)
      turnOnLights(PIN_LED_MERAH_UTARA, PIN_LED_MERAH_SELATAN, PIN_LED_MERAH_TIMUR, PIN_LED_MERAH_BARAT);
      Serial.println("State 3: Semua Merah (Transisi)");
      break;
      
    case STATE_3:
      currentState = STATE_4;
      // Timur & Barat Hijau, Utara & Selatan Merah
      turnOnLights(PIN_LED_MERAH_UTARA, PIN_LED_MERAH_SELATAN, PIN_LED_HIJAU_TIMUR, PIN_LED_HIJAU_BARAT);
      Serial.println("State 4: Timur & Barat Hijau, Utara & Selatan Merah");
      break;
      
    case STATE_4:
      currentState = STATE_5;
      // Timur & Barat Kuning, Utara & Selatan Merah
      turnOnLights(PIN_LED_MERAH_UTARA, PIN_LED_MERAH_SELATAN, PIN_LED_KUNING_TIMUR, PIN_LED_KUNING_BARAT);
      Serial.println("State 5: Timur & Barat Kuning, Utara & Selatan Merah");
      break;
      
    case STATE_5:
      currentState = STATE_6;
      // Semua Merah (Transisi)
      turnOnLights(PIN_LED_MERAH_UTARA, PIN_LED_MERAH_SELATAN, PIN_LED_MERAH_TIMUR, PIN_LED_MERAH_BARAT);
      Serial.println("State 6: Semua Merah (Transisi)");
      break;
      
    case STATE_6:
      currentState = STATE_7;
      // Individual: Utara Hijau (Belok Kiri), Lainnya Merah
      turnOnLights(PIN_LED_HIJAU_UTARA, PIN_LED_MERAH_SELATAN, PIN_LED_MERAH_TIMUR, PIN_LED_MERAH_BARAT);
      Serial.println("State 7: Individual - Utara Hijau (Belok Kiri)");
      break;
      
    case STATE_7:
      currentState = STATE_8;
      // Individual: Timur Hijau (Belok Kiri), Lainnya Merah
      turnOnLights(PIN_LED_MERAH_UTARA, PIN_LED_MERAH_SELATAN, PIN_LED_HIJAU_TIMUR, PIN_LED_MERAH_BARAT);
      Serial.println("State 8: Individual - Timur Hijau (Belok Kiri)");
      break;
      
    case STATE_8:
      currentState = STATE_9;
      // Individual: Selatan Hijau (Belok Kiri), Lainnya Merah
      turnOnLights(PIN_LED_MERAH_UTARA, PIN_LED_HIJAU_SELATAN, PIN_LED_MERAH_TIMUR, PIN_LED_MERAH_BARAT);
      Serial.println("State 9: Individual - Selatan Hijau (Belok Kiri)");
      break;
      
    case STATE_9:
      currentState = STATE_10;
      // Individual: Barat Hijau (Belok Kiri), Lainnya Merah
      turnOnLights(PIN_LED_MERAH_UTARA, PIN_LED_MERAH_SELATAN, PIN_LED_MERAH_TIMUR, PIN_LED_HIJAU_BARAT);
      Serial.println("State 10: Individual - Barat Hijau (Belok Kiri)");
      break;
      
    case STATE_10:
      currentState = STATE_1;
      // Kembali ke siklus utama: Utara & Selatan Hijau, Timur & Barat Merah
      turnOnLights(PIN_LED_HIJAU_UTARA, PIN_LED_HIJAU_SELATAN, PIN_LED_MERAH_TIMUR, PIN_LED_MERAH_BARAT);
      Serial.println("State 1: Utara & Selatan Hijau, Timur & Barat Merah");
      break;
      
    case STATE_11:
      // Emergency Mode: Semua Merah
      turnOnLights(PIN_LED_MERAH_UTARA, PIN_LED_MERAH_SELATAN, PIN_LED_MERAH_TIMUR, PIN_LED_MERAH_BARAT);
      Serial.println("Emergency Mode: Semua Merah");
      currentState = STATE_1; // Kembali ke normal
      break;
      
    case STATE_12:
      // Pedestrian Mode: Semua Merah
      turnOnLights(PIN_LED_MERAH_UTARA, PIN_LED_MERAH_SELATAN, PIN_LED_MERAH_TIMUR, PIN_LED_MERAH_BARAT);
      Serial.println("Pedestrian Mode: Semua Merah");
      currentState = STATE_1; // Kembali ke normal
      break;
  }
}

void turnOffAllLights() {
  // Matikan semua LED dari 4 set lampu perempatan
  digitalWrite(PIN_LED_HIJAU_UTARA, LOW);
  digitalWrite(PIN_LED_KUNING_UTARA, LOW);
  digitalWrite(PIN_LED_MERAH_UTARA, LOW);
  
  digitalWrite(PIN_LED_HIJAU_SELATAN, LOW);
  digitalWrite(PIN_LED_KUNING_SELATAN, LOW);
  digitalWrite(PIN_LED_MERAH_SELATAN, LOW);
  
  digitalWrite(PIN_LED_HIJAU_TIMUR, LOW);
  digitalWrite(PIN_LED_KUNING_TIMUR, LOW);
  digitalWrite(PIN_LED_MERAH_TIMUR, LOW);
  
  digitalWrite(PIN_LED_HIJAU_BARAT, LOW);
  digitalWrite(PIN_LED_KUNING_BARAT, LOW);
  digitalWrite(PIN_LED_MERAH_BARAT, LOW);
}

void turnOnLights(int pin1, int pin2, int pin3, int pin4) {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, HIGH);
}

void setEmergencyMode() {
  emergencyMode = true;
  currentState = STATE_11;
  turnOffAllLights();
  turnOnLights(PIN_LED_MERAH_UTARA, PIN_LED_MERAH_SELATAN, PIN_LED_MERAH_TIMUR, PIN_LED_MERAH_BARAT);
  Serial.println("Emergency Mode Activated");
}

void setPedestrianMode() {
  pedestrianMode = true;
  currentState = STATE_12;
  turnOffAllLights();
  turnOnLights(PIN_LED_MERAH_UTARA, PIN_LED_MERAH_SELATAN, PIN_LED_MERAH_TIMUR, PIN_LED_MERAH_BARAT);
  Serial.println("Pedestrian Mode Activated");
} 
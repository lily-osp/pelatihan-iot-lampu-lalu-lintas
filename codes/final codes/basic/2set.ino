/*
 * Proyek Lampu Lalu Lintas - Jalan Lurus (2-way Traffic)
 * ESP32 Basic Version (Non-IoT)
 * 
 * Komponen:
 * - ESP32 Dev Kit
 * - 6x LED (2x Merah, 2x Kuning, 2x Hijau)
 * - 6x Resistor 220Ω
 * - Breadboard & Kabel Jumper
 * 
 * Koneksi:
 * - Pin D23 -> LED Hijau Arah 1
 * - Pin D22 -> LED Kuning Arah 1  
 * - Pin D21 -> LED Merah Arah 1
 * - Pin D19 -> LED Hijau Arah 2
 * - Pin D18 -> LED Kuning Arah 2
 * - Pin D5  -> LED Merah Arah 2
 */

// Pin LED - 2 Set Lampu Lalu Lintas
// Set 1: Arah 1
const int PIN_LED_HIJAU_ARAH1 = 23;
const int PIN_LED_KUNING_ARAH1 = 22;
const int PIN_LED_MERAH_ARAH1 = 21;

// Set 2: Arah 2
const int PIN_LED_HIJAU_ARAH2 = 19;
const int PIN_LED_KUNING_ARAH2 = 18;
const int PIN_LED_MERAH_ARAH2 = 5;

// State machine untuk lampu lalu lintas jalan lurus
enum TrafficState {
  STATE_1, // Arah 1 Hijau, Arah 2 Merah
  STATE_2, // Arah 1 Kuning, Arah 2 Merah
  STATE_3, // Kedua Arah Merah (Transisi)
  STATE_4, // Arah 1 Merah, Arah 2 Hijau
  STATE_5, // Arah 1 Merah, Arah 2 Kuning
  STATE_6  // Kedua Arah Merah (Transisi)
};

TrafficState currentState = STATE_1;
unsigned long lastStateChange = 0;

// Durasi setiap state (dalam milidetik)
const unsigned long DURASI_HIJAU_ARAH1 = 20000;   // 20 detik
const unsigned long DURASI_KUNING = 3000;         // 3 detik
const unsigned long DURASI_TRANSISI = 2000;       // 2 detik
const unsigned long DURASI_HIJAU_ARAH2 = 20000;   // 20 detik

void setup() {
  Serial.begin(115200);
  Serial.println("=== Proyek Lampu Lalu Lintas Jalan Lurus (Basic) ===");
  
  // Setup pin LED sebagai output - 2 Set Lampu
  // Set 1: Arah 1
  pinMode(PIN_LED_HIJAU_ARAH1, OUTPUT);
  pinMode(PIN_LED_KUNING_ARAH1, OUTPUT);
  pinMode(PIN_LED_MERAH_ARAH1, OUTPUT);
  
  // Set 2: Arah 2
  pinMode(PIN_LED_HIJAU_ARAH2, OUTPUT);
  pinMode(PIN_LED_KUNING_ARAH2, OUTPUT);
  pinMode(PIN_LED_MERAH_ARAH2, OUTPUT);
  
  // Matikan semua LED di awal
  turnOffAllLights();
  
  // Set initial state lights
  turnOnLights(PIN_LED_HIJAU_ARAH1, PIN_LED_MERAH_ARAH2);
  
  Serial.println("Setup selesai. Memulai sistem lampu lalu lintas jalan lurus...");
  Serial.println("State 1: Arah 1 Hijau, Arah 2 Merah");
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
    case STATE_1: return DURASI_HIJAU_ARAH1;
    case STATE_2: return DURASI_KUNING;
    case STATE_3: return DURASI_TRANSISI;
    case STATE_4: return DURASI_HIJAU_ARAH2;
    case STATE_5: return DURASI_KUNING;
    case STATE_6: return DURASI_TRANSISI;
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
      // Arah 1 Kuning, Arah 2 Merah
      turnOnLights(PIN_LED_KUNING_ARAH1, PIN_LED_MERAH_ARAH2);
      Serial.println("State 2: Arah 1 Kuning, Arah 2 Merah");
      break;
      
    case STATE_2:
      currentState = STATE_3;
      // Kedua Arah Merah (Transisi)
      turnOnLights(PIN_LED_MERAH_ARAH1, PIN_LED_MERAH_ARAH2);
      Serial.println("State 3: Kedua Arah Merah (Transisi)");
      break;
      
    case STATE_3:
      currentState = STATE_4;
      // Arah 1 Merah, Arah 2 Hijau
      turnOnLights(PIN_LED_MERAH_ARAH1, PIN_LED_HIJAU_ARAH2);
      Serial.println("State 4: Arah 1 Merah, Arah 2 Hijau");
      break;
      
    case STATE_4:
      currentState = STATE_5;
      // Arah 1 Merah, Arah 2 Kuning
      turnOnLights(PIN_LED_MERAH_ARAH1, PIN_LED_KUNING_ARAH2);
      Serial.println("State 5: Arah 1 Merah, Arah 2 Kuning");
      break;
      
    case STATE_5:
      currentState = STATE_6;
      // Kedua Arah Merah (Transisi)
      turnOnLights(PIN_LED_MERAH_ARAH1, PIN_LED_MERAH_ARAH2);
      Serial.println("State 6: Kedua Arah Merah (Transisi)");
      break;
      
    case STATE_6:
      currentState = STATE_1;
      // Arah 1 Hijau, Arah 2 Merah
      turnOnLights(PIN_LED_HIJAU_ARAH1, PIN_LED_MERAH_ARAH2);
      Serial.println("State 1: Arah 1 Hijau, Arah 2 Merah");
      break;
  }
}

void turnOffAllLights() {
  // Matikan semua LED dari 2 set lampu
  digitalWrite(PIN_LED_HIJAU_ARAH1, LOW);
  digitalWrite(PIN_LED_KUNING_ARAH1, LOW);
  digitalWrite(PIN_LED_MERAH_ARAH1, LOW);
  
  digitalWrite(PIN_LED_HIJAU_ARAH2, LOW);
  digitalWrite(PIN_LED_KUNING_ARAH2, LOW);
  digitalWrite(PIN_LED_MERAH_ARAH2, LOW);
}

void turnOnLights(int pin1, int pin2) {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
} 
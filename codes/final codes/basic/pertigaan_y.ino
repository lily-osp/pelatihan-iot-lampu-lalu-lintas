/*
 * Proyek Lampu Lalu Lintas - Pertigaan (3-way Y-Junction)
 * ESP32 Basic Version (Non-IoT) - VERSI FINAL DENGAN SAVE STATE
 * * Sistem traffic light pertigaan yang lengkap dengan fitur:
 * - 3 arah (Utara, Selatan, Timur) mendapatkan giliran hijau individual
 * - Menyimpan state terakhir sebelum mode darurat/pejalan kaki dan melanjutkannya
 * - Timing yang realistis untuk traffic flow
 * - Mode darurat berwaktu (20 detik) yang diaktifkan dengan tombol (Prioritas Tertinggi)
 * - Mode pejalan kaki berwaktu (15 detik) yang diaktifkan dengan tombol
 * * Komponen:
 * - ESP32 Dev Kit
 * - 9x LED (3x Merah, 3x Kuning, 3x Hijau)
 * - 9x Resistor 220Ω
 * - 2x Push Button
 * - Breadboard & Kabel Jumper
 * * Koneksi untuk Pertigaan (3 Set Lampu):
 * Set 1: Arah Utara
 * - Pin D23 -> LED Hijau Utara
 * - Pin D22 -> LED Kuning Utara  
 * - Pin D21 -> LED Merah Utara
 * * Set 2: Arah Selatan
 * - Pin D19 -> LED Hijau Selatan
 * - Pin D18 -> LED Kuning Selatan
 * - Pin D5  -> LED Merah Selatan
 * * Set 3: Arah Timur
 * - Pin D4  -> LED Hijau Timur
 * - Pin D2  -> LED Kuning Timur
 * - Pin D15 -> LED Merah Timur
 * * Koneksi Tombol:
 * - Pin D25 -> Tombol Darurat -> GND
 * - Pin D26 -> Tombol Pejalan Kaki -> GND
 */

// Pin LED - 3 Set Lampu Lalu Lintas Pertigaan
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

// Pin untuk tombol
const int PIN_TOMBOL_DARURAT = 25;
const int PIN_TOMBOL_PEDESTRIAN = 26;

// State machine yang disesuaikan untuk pertigaan Y
enum TrafficState {
  // Siklus Utama
  STATE_N_GREEN,
  STATE_N_YELLOW,
  STATE_ALL_RED_1,  // Transisi dari Utara ke Selatan
  STATE_S_GREEN,
  STATE_S_YELLOW,
  STATE_ALL_RED_2,  // Transisi dari Selatan ke Timur
  STATE_E_GREEN,
  STATE_E_YELLOW,
  STATE_ALL_RED_3,  // Transisi dari Timur kembali ke Utara

  // Mode Khusus
  STATE_EMERGENCY,
  STATE_PEDESTRIAN
};

TrafficState currentState = STATE_ALL_RED_3; // Mulai dari state aman untuk memulai siklus dari awal
TrafficState lastNormalState = STATE_ALL_RED_3; // Variabel untuk menyimpan state sebelum interupsi
unsigned long lastStateChange = 0;

// Durasi setiap state (dalam milidetik)
const unsigned long DURASI_HIJAU_UTAMA = 15000;      // 15 detik untuk setiap arah
const unsigned long DURASI_KUNING = 3000;            // 3 detik
const unsigned long DURASI_TRANSISI_MERAH = 2000;    // 2 detik
const unsigned long DURASI_EMERGENCY = 20000;        // 20 detik untuk mode darurat
const unsigned long DURASI_PEDESTRIAN = 15000;       // 15 detik untuk mode pejalan kaki

// Mode flags
bool emergencyMode = false;
bool pedestrianMode = false;

// Variabel untuk mode darurat (blinking)
unsigned long lastBlinkTime = 0;
bool emergencyBlinkState = true;
const unsigned long BLINK_INTERVAL = 500; // interval kedip 500ms

// Variabel untuk debouncing tombol
unsigned long lastEmergencyDebounceTime = 0;
unsigned long lastPedestrianDebounceTime = 0;
unsigned long debounceDelay = 50; // 50ms debounce delay

int lastEmergencyButtonReading = HIGH;
int emergencyButtonState = HIGH;
int lastPedestrianButtonReading = HIGH;
int pedestrianButtonState = HIGH;


void setup() {
  Serial.begin(115200);
  Serial.println("=== Proyek Lampu Lalu Lintas Pertigaan-Y (Versi Final dengan Save State) ===");
  
  // Setup pin LED sebagai output
  pinMode(PIN_LED_HIJAU_UTARA, OUTPUT);
  pinMode(PIN_LED_KUNING_UTARA, OUTPUT);
  pinMode(PIN_LED_MERAH_UTARA, OUTPUT);
  
  pinMode(PIN_LED_HIJAU_SELATAN, OUTPUT);
  pinMode(PIN_LED_KUNING_SELATAN, OUTPUT);
  pinMode(PIN_LED_MERAH_SELATAN, OUTPUT);
  
  pinMode(PIN_LED_HIJAU_TIMUR, OUTPUT);
  pinMode(PIN_LED_KUNING_TIMUR, OUTPUT);
  pinMode(PIN_LED_MERAH_TIMUR, OUTPUT);

  // Setup pin tombol sebagai input dengan pull-up internal
  pinMode(PIN_TOMBOL_DARURAT, INPUT_PULLUP);
  pinMode(PIN_TOMBOL_PEDESTRIAN, INPUT_PULLUP);
  
  // Matikan semua LED di awal dan mulai state pertama
  changeToNextState(); 
  lastStateChange = millis();
}

void loop() {
  unsigned long currentTime = millis();

  // 1. Cek input tombol
  checkEmergencyButton();
  checkPedestrianButton();

  // 2. Jalankan logika state machine
  // Logika khusus untuk blinking saat dalam mode darurat
  if (currentState == STATE_EMERGENCY) {
    if (currentTime - lastBlinkTime >= BLINK_INTERVAL) {
      emergencyBlinkState = !emergencyBlinkState;
      if (emergencyBlinkState) {
        turnOffAllLights();
        digitalWrite(PIN_LED_KUNING_UTARA, HIGH);
        digitalWrite(PIN_LED_KUNING_SELATAN, HIGH);
        digitalWrite(PIN_LED_KUNING_TIMUR, HIGH);
      } else {
        turnOffAllLights();
      }
      lastBlinkTime = currentTime;
    }
  }

  // Logika state machine non-blocking untuk semua siklus
  if (currentTime - lastStateChange >= getStateDuration()) {
    changeToNextState();
    lastStateChange = currentTime;
  }
}

void checkEmergencyButton() {
  // Mode darurat memiliki prioritas tertinggi dan dapat menginterupsi mode lain.
  int reading = digitalRead(PIN_TOMBOL_DARURAT);

  if (reading != lastEmergencyButtonReading) {
    lastEmergencyDebounceTime = millis();
  }

  if ((millis() - lastEmergencyDebounceTime) > debounceDelay) {
    if (reading != emergencyButtonState) {
      emergencyButtonState = reading;
      if (emergencyButtonState == LOW && !emergencyMode) {
        activateEmergencyMode();
      }
    }
  }
  lastEmergencyButtonReading = reading;
}

void checkPedestrianButton() {
  if (emergencyMode) return;

  int reading = digitalRead(PIN_TOMBOL_PEDESTRIAN);

  if (reading != lastPedestrianButtonReading) {
    lastPedestrianDebounceTime = millis();
  }

  if ((millis() - lastPedestrianDebounceTime) > debounceDelay) {
    if (reading != pedestrianButtonState) {
      pedestrianButtonState = reading;
      if (pedestrianButtonState == LOW && !pedestrianMode) {
        activatePedestrianMode();
      }
    }
  }
  lastPedestrianButtonReading = reading;
}


unsigned long getStateDuration() {
  switch (currentState) {
    case STATE_N_GREEN:   return DURASI_HIJAU_UTAMA;
    case STATE_S_GREEN:   return DURASI_HIJAU_UTAMA;
    case STATE_E_GREEN:   return DURASI_HIJAU_UTAMA;
    
    case STATE_N_YELLOW:  return DURASI_KUNING;
    case STATE_S_YELLOW:  return DURASI_KUNING;
    case STATE_E_YELLOW:  return DURASI_KUNING;
      
    case STATE_ALL_RED_1: return DURASI_TRANSISI_MERAH;
    case STATE_ALL_RED_2: return DURASI_TRANSISI_MERAH;
    case STATE_ALL_RED_3: return DURASI_TRANSISI_MERAH;

    case STATE_EMERGENCY:  return DURASI_EMERGENCY;
    case STATE_PEDESTRIAN: return DURASI_PEDESTRIAN;

    default: return 2000; // Durasi default jika terjadi error
  }
}

TrafficState determineNextStateAfterInterruption() {
    // Fungsi ini menentukan state selanjutnya setelah interupsi,
    // memastikan siklus berlanjut dengan benar.
    switch (lastNormalState) {
        case STATE_N_GREEN:
        case STATE_N_YELLOW:
            return STATE_ALL_RED_1; // Lanjut ke fase Selatan

        case STATE_S_GREEN:
        case STATE_S_YELLOW:
            return STATE_ALL_RED_2; // Lanjut ke fase Timur

        case STATE_E_GREEN:
        case STATE_E_YELLOW:
            return STATE_ALL_RED_3; // Lanjut kembali ke fase Utara

        // Jika interupsi terjadi saat semua lampu merah, lanjutkan ke state berikutnya yang sesuai
        case STATE_ALL_RED_1: return STATE_S_GREEN;
        case STATE_ALL_RED_2: return STATE_E_GREEN;
        case STATE_ALL_RED_3: return STATE_N_GREEN;

        default:
            return STATE_ALL_RED_3; // Default aman
    }
}


void changeToNextState() {
  turnOffAllLights();
  
  TrafficState nextState;

  // 1. Tentukan state selanjutnya berdasarkan state saat ini
  switch (currentState) {
    case STATE_EMERGENCY:
      emergencyMode = false;
      nextState = determineNextStateAfterInterruption();
      Serial.println("Mode Darurat Selesai. Melanjutkan siklus...");
      break;

    case STATE_PEDESTRIAN:
      pedestrianMode = false;
      nextState = determineNextStateAfterInterruption();
      Serial.println("Mode Pejalan Kaki Selesai. Melanjutkan siklus...");
      break;

    case STATE_ALL_RED_3: // Kondisi awal atau akhir dari siklus
      nextState = STATE_N_GREEN;
      break;

    case STATE_N_GREEN:
      nextState = STATE_N_YELLOW;
      break;

    case STATE_N_YELLOW:
      nextState = STATE_ALL_RED_1;
      break;

    case STATE_ALL_RED_1:
      nextState = STATE_S_GREEN;
      break;
      
    case STATE_S_GREEN:
      nextState = STATE_S_YELLOW;
      break;

    case STATE_S_YELLOW:
      nextState = STATE_ALL_RED_2;
      break;

    case STATE_ALL_RED_2:
      nextState = STATE_E_GREEN;
      break;

    case STATE_E_GREEN:
      nextState = STATE_E_YELLOW;
      break;

    case STATE_E_YELLOW:
      nextState = STATE_ALL_RED_3;
      break;

    default: // Jika terjadi state yang tidak diketahui, kembali ke awal siklus
      nextState = STATE_ALL_RED_3;
      break;
  }
  
  currentState = nextState; // Update state global

  // 2. Aktifkan lampu berdasarkan state yang BARU
  switch (currentState) {
      case STATE_N_GREEN:
          Serial.println("State: Utara Hijau");
          digitalWrite(PIN_LED_HIJAU_UTARA, HIGH);
          digitalWrite(PIN_LED_MERAH_SELATAN, HIGH);
          digitalWrite(PIN_LED_MERAH_TIMUR, HIGH);
          break;
      case STATE_N_YELLOW:
          Serial.println("State: Utara Kuning");
          digitalWrite(PIN_LED_KUNING_UTARA, HIGH);
          digitalWrite(PIN_LED_MERAH_SELATAN, HIGH);
          digitalWrite(PIN_LED_MERAH_TIMUR, HIGH);
          break;
      case STATE_S_GREEN:
          Serial.println("State: Selatan Hijau");
          digitalWrite(PIN_LED_MERAH_UTARA, HIGH);
          digitalWrite(PIN_LED_HIJAU_SELATAN, HIGH);
          digitalWrite(PIN_LED_MERAH_TIMUR, HIGH);
          break;
      case STATE_S_YELLOW:
          Serial.println("State: Selatan Kuning");
          digitalWrite(PIN_LED_MERAH_UTARA, HIGH);
          digitalWrite(PIN_LED_KUNING_SELATAN, HIGH);
          digitalWrite(PIN_LED_MERAH_TIMUR, HIGH);
          break;
      case STATE_E_GREEN:
          Serial.println("State: Timur Hijau");
          digitalWrite(PIN_LED_MERAH_UTARA, HIGH);
          digitalWrite(PIN_LED_MERAH_SELATAN, HIGH);
          digitalWrite(PIN_LED_HIJAU_TIMUR, HIGH);
          break;
      case STATE_E_YELLOW:
          Serial.println("State: Timur Kuning");
          digitalWrite(PIN_LED_MERAH_UTARA, HIGH);
          digitalWrite(PIN_LED_MERAH_SELATAN, HIGH);
          digitalWrite(PIN_LED_KUNING_TIMUR, HIGH);
          break;
      case STATE_ALL_RED_1:
      case STATE_ALL_RED_2:
      case STATE_ALL_RED_3:
          Serial.println("State: Semua Merah (Transisi)");
          setAllRed();
          break;
  }
}

void turnOffAllLights() {
  digitalWrite(PIN_LED_HIJAU_UTARA, LOW);
  digitalWrite(PIN_LED_KUNING_UTARA, LOW);
  digitalWrite(PIN_LED_MERAH_UTARA, LOW);
  
  digitalWrite(PIN_LED_HIJAU_SELATAN, LOW);
  digitalWrite(PIN_LED_KUNING_SELATAN, LOW);
  digitalWrite(PIN_LED_MERAH_SELATAN, LOW);
  
  digitalWrite(PIN_LED_HIJAU_TIMUR, LOW);
  digitalWrite(PIN_LED_KUNING_TIMUR, LOW);
  digitalWrite(PIN_LED_MERAH_TIMUR, LOW);
}

void setAllRed() {
  digitalWrite(PIN_LED_MERAH_UTARA, HIGH);
  digitalWrite(PIN_LED_MERAH_SELATAN, HIGH);
  digitalWrite(PIN_LED_MERAH_TIMUR, HIGH);
}

void activateEmergencyMode() {
  if (!emergencyMode) {
    lastNormalState = currentState; // Simpan state saat ini
  }
  pedestrianMode = false;
  emergencyMode = true;
  currentState = STATE_EMERGENCY;
  lastStateChange = millis();
  lastBlinkTime = millis();
  emergencyBlinkState = true;
  Serial.println("!!! MODE DARURAT DIAKTIFKAN (20 Detik) !!!");
  
  turnOffAllLights();
  digitalWrite(PIN_LED_KUNING_UTARA, HIGH);
  digitalWrite(PIN_LED_KUNING_SELATAN, HIGH);
  digitalWrite(PIN_LED_KUNING_TIMUR, HIGH);
}

void activatePedestrianMode() {
  if (!pedestrianMode) {
    lastNormalState = currentState; // Simpan state saat ini
  }
  pedestrianMode = true;
  currentState = STATE_PEDESTRIAN;
  lastStateChange = millis();
  Serial.println(">>> MODE PEJALAN KAKI DIAKTIFKAN (15 Detik) <<<");
  
  turnOffAllLights();
  setAllRed();
}

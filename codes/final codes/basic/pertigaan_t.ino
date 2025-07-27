/*
 * Proyek Lampu Lalu Lintas - Pertigaan T-Shape (3-way T-Intersection)
 * ESP32 Basic Version (Non-IoT)
 * 
 * Sistem traffic light pertigaan T-shape yang realistis dengan fitur:
 * - Main road (jalan utama) dengan traffic lurus dan belok kanan
 * - Side road (jalan samping) dengan Belok Kanan dan kanan
 * - Timing yang berbeda untuk main road vs side road
 * - Left turn phase untuk side road
 * - Pedestrian crossing consideration
 * 
 * Komponen:
 * - ESP32 Dev Kit
 * - 9x LED (3x Merah, 3x Kuning, 3x Hijau)
 * - 9x Resistor 220Ω
 * - Breadboard & Kabel Jumper
 * 
 * Koneksi untuk Pertigaan T-Shape (3 Set Lampu):
 * Set 1: Main Road (Jalan Utama)
 * - Pin D23 -> LED Hijau Main Road
 * - Pin D22 -> LED Kuning Main Road  
 * - Pin D21 -> LED Merah Main Road
 * 
 * Set 2: Side Road (Jalan Samping)
 * - Pin D19 -> LED Hijau Side Road
 * - Pin D18 -> LED Kuning Side Road
 * - Pin D5  -> LED Merah Side Road
 * 
 * Set 3: Left Turn (Belok Kanan dari Side Road)
 * - Pin D4  -> LED Hijau Left Turn
 * - Pin D2  -> LED Kuning Left Turn
 * - Pin D15 -> LED Merah Left Turn
 * 
 * Logika T-Shape:
 * - Main road gets priority (longer green time)
 * - Side road gets shorter green time
 * - Left turn from side road gets dedicated phase
 * - Realistic timing for traffic flow
 */

// Pin LED - 3 Set Lampu Lalu Lintas T-Shape
// Set 1: Main Road (Jalan Utama)
const int PIN_LED_HIJAU_MAIN = 23;
const int PIN_LED_KUNING_MAIN = 22;
const int PIN_LED_MERAH_MAIN = 21;

// Set 2: Side Road (Jalan Samping)
const int PIN_LED_HIJAU_SIDE = 19;
const int PIN_LED_KUNING_SIDE = 18;
const int PIN_LED_MERAH_SIDE = 5;

// Set 3: Left Turn (Belok Kanan dari Side Road)
const int PIN_LED_HIJAU_LEFT = 4;
const int PIN_LED_KUNING_LEFT = 2;
const int PIN_LED_MERAH_LEFT = 15;

// State machine untuk lampu lalu lintas pertigaan T-shape
enum TrafficState {
  STATE_1, // Main Road Hijau (Lurus & Belok Kanan), Side Road & Left Turn Merah
  STATE_2, // Main Road Kuning, Side Road & Left Turn Merah
  STATE_3, // Semua Merah (Transisi)
  STATE_4, // Side Road Hijau (Belok Kanan), Main Road & Left Turn Merah
  STATE_5, // Side Road Kuning, Main Road & Left Turn Merah
  STATE_6, // Semua Merah (Transisi)
  STATE_7, // Left Turn Hijau (Belok Kanan dari Side Road), Main Road & Side Road Merah
  STATE_8, // Left Turn Kuning, Main Road & Side Road Merah
  STATE_9  // Semua Merah (Transisi)
};

TrafficState currentState = STATE_1;
unsigned long lastStateChange = 0;

// Durasi setiap state (dalam milidetik) - Realistic timing untuk T-shape
const unsigned long DURASI_HIJAU_MAIN = 35000;     // 35 detik - Main road priority
const unsigned long DURASI_KUNING = 4000;          // 4 detik
const unsigned long DURASI_TRANSISI = 2000;        // 2 detik
const unsigned long DURASI_HIJAU_SIDE = 20000;     // 20 detik - Side road shorter
const unsigned long DURASI_HIJAU_LEFT = 15000;     // 15 detik - Left turn phase

// Emergency mode variables
bool emergencyMode = false;
unsigned long lastBlinkTime = 0;
bool emergencyBlinkState = false;
const unsigned long BLINK_INTERVAL = 500; // 500ms for blinking

void setup() {
  Serial.begin(115200);
  Serial.println("=== Proyek Lampu Lalu Lintas Pertigaan T-Shape (Basic) ===");
  
  // Setup pin LED sebagai output - 3 Set Lampu T-Shape
  // Set 1: Main Road
  pinMode(PIN_LED_HIJAU_MAIN, OUTPUT);
  pinMode(PIN_LED_KUNING_MAIN, OUTPUT);
  pinMode(PIN_LED_MERAH_MAIN, OUTPUT);
  
  // Set 2: Side Road
  pinMode(PIN_LED_HIJAU_SIDE, OUTPUT);
  pinMode(PIN_LED_KUNING_SIDE, OUTPUT);
  pinMode(PIN_LED_MERAH_SIDE, OUTPUT);
  
  // Set 3: Left Turn
  pinMode(PIN_LED_HIJAU_LEFT, OUTPUT);
  pinMode(PIN_LED_KUNING_LEFT, OUTPUT);
  pinMode(PIN_LED_MERAH_LEFT, OUTPUT);
  
  // Matikan semua LED di awal
  turnOffAllLights();
  
  // Set initial state lights
  turnOnLights(PIN_LED_HIJAU_MAIN, PIN_LED_MERAH_SIDE, PIN_LED_MERAH_LEFT);
  
  Serial.println("Setup selesai. Memulai sistem lampu lalu lintas pertigaan T-shape...");
  Serial.println("State 1: Main Road Hijau, Side Road & Left Turn Merah");
}

void loop() {
  // Logika state machine non-blocking
  unsigned long currentTime = millis();
  
  // Handle emergency mode blinking
  if (emergencyMode) {
    if (currentTime - lastBlinkTime >= BLINK_INTERVAL) {
      emergencyBlinkState = !emergencyBlinkState;
      if (emergencyBlinkState) {
        // Turn on all yellow LEDs
        turnOnLights(PIN_LED_KUNING_MAIN, PIN_LED_KUNING_SIDE, PIN_LED_KUNING_LEFT);
      } else {
        // Turn off all LEDs
        turnOffAllLights();
      }
      lastBlinkTime = currentTime;
    }
    return; // Don't proceed with normal state machine during emergency
  }
  
  if (currentTime - lastStateChange >= getStateDuration()) {
    changeToNextState();
    lastStateChange = currentTime;
  }
}

unsigned long getStateDuration() {
  switch (currentState) {
    case STATE_1: return DURASI_HIJAU_MAIN;
    case STATE_2: return DURASI_KUNING;
    case STATE_3: return DURASI_TRANSISI;
    case STATE_4: return DURASI_HIJAU_SIDE;
    case STATE_5: return DURASI_KUNING;
    case STATE_6: return DURASI_TRANSISI;
    case STATE_7: return DURASI_HIJAU_LEFT;
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
      // Main Road Kuning, Side Road & Left Turn Merah
      turnOnLights(PIN_LED_KUNING_MAIN, PIN_LED_MERAH_SIDE, PIN_LED_MERAH_LEFT);
      Serial.println("State 2: Main Road Kuning, Side Road & Left Turn Merah");
      break;
      
    case STATE_2:
      currentState = STATE_3;
      // Semua Merah (Transisi)
      turnOnLights(PIN_LED_MERAH_MAIN, PIN_LED_MERAH_SIDE, PIN_LED_MERAH_LEFT);
      Serial.println("State 3: Semua Merah (Transisi)");
      break;
      
    case STATE_3:
      currentState = STATE_4;
      // Side Road Hijau, Main Road & Left Turn Merah
      turnOnLights(PIN_LED_MERAH_MAIN, PIN_LED_HIJAU_SIDE, PIN_LED_MERAH_LEFT);
      Serial.println("State 4: Side Road Hijau, Main Road & Left Turn Merah");
      break;
      
    case STATE_4:
      currentState = STATE_5;
      // Side Road Kuning, Main Road & Left Turn Merah
      turnOnLights(PIN_LED_MERAH_MAIN, PIN_LED_KUNING_SIDE, PIN_LED_MERAH_LEFT);
      Serial.println("State 5: Side Road Kuning, Main Road & Left Turn Merah");
      break;
      
    case STATE_5:
      currentState = STATE_6;
      // Semua Merah (Transisi)
      turnOnLights(PIN_LED_MERAH_MAIN, PIN_LED_MERAH_SIDE, PIN_LED_MERAH_LEFT);
      Serial.println("State 6: Semua Merah (Transisi)");
      break;
      
    case STATE_6:
      currentState = STATE_7;
      // Left Turn Hijau, Main Road & Side Road Merah
      turnOnLights(PIN_LED_MERAH_MAIN, PIN_LED_MERAH_SIDE, PIN_LED_HIJAU_LEFT);
      Serial.println("State 7: Left Turn Hijau, Main Road & Side Road Merah");
      break;
      
    case STATE_7:
      currentState = STATE_8;
      // Left Turn Kuning, Main Road & Side Road Merah
      turnOnLights(PIN_LED_MERAH_MAIN, PIN_LED_MERAH_SIDE, PIN_LED_KUNING_LEFT);
      Serial.println("State 8: Left Turn Kuning, Main Road & Side Road Merah");
      break;
      
    case STATE_8:
      currentState = STATE_9;
      // Semua Merah (Transisi)
      turnOnLights(PIN_LED_MERAH_MAIN, PIN_LED_MERAH_SIDE, PIN_LED_MERAH_LEFT);
      Serial.println("State 9: Semua Merah (Transisi)");
      break;
      
    case STATE_9:
      currentState = STATE_1;
      // Kembali ke siklus: Main Road Hijau, Side Road & Left Turn Merah
      turnOnLights(PIN_LED_HIJAU_MAIN, PIN_LED_MERAH_SIDE, PIN_LED_MERAH_LEFT);
      Serial.println("State 1: Main Road Hijau, Side Road & Left Turn Merah");
      break;
  }
}

void turnOffAllLights() {
  // Matikan semua LED dari 3 set lampu T-shape
  digitalWrite(PIN_LED_HIJAU_MAIN, LOW);
  digitalWrite(PIN_LED_KUNING_MAIN, LOW);
  digitalWrite(PIN_LED_MERAH_MAIN, LOW);
  
  digitalWrite(PIN_LED_HIJAU_SIDE, LOW);
  digitalWrite(PIN_LED_KUNING_SIDE, LOW);
  digitalWrite(PIN_LED_MERAH_SIDE, LOW);
  
  digitalWrite(PIN_LED_HIJAU_LEFT, LOW);
  digitalWrite(PIN_LED_KUNING_LEFT, LOW);
  digitalWrite(PIN_LED_MERAH_LEFT, LOW);
}

void turnOnLights(int pin1, int pin2, int pin3) {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, HIGH);
}

void setEmergencyMode() {
  emergencyMode = true;
  turnOffAllLights();
  lastBlinkTime = millis();
  emergencyBlinkState = false;
  Serial.println("Emergency Mode Activated - Blinking Yellow");
}

void clearEmergencyMode() {
  emergencyMode = false;
  turnOffAllLights();
  Serial.println("Emergency Mode Cleared - Returning to Normal");
} 
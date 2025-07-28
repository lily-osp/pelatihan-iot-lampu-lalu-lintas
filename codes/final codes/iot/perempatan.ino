/*
 * Proyek IoT Lampu Lalu Lintas - Perempatan Complete (4-way Traffic)
 * ESP32 dengan Adafruit IO Integration - VERSI FINAL DENGAN SAVE STATE
 * * Sistem traffic light perempatan yang lengkap dengan fitur:
 * - 4 arah individual (Utara, Selatan, Timur, Barat)
 * - Menyimpan state terakhir sebelum mode darurat/pejalan kaki dan melanjutkannya
 * - Timing yang realistis untuk traffic flow
 * - Mode darurat berwaktu (20 detik) yang diaktifkan dengan tombol (Prioritas Tertinggi)
 * - Mode pejalan kaki berwaktu (15 detik) yang diaktifkan dengan tombol
 * - IoT Integration dengan Adafruit IO
 * * Komponen:
 * - ESP32 Dev Kit
 * - 12x LED (4x Merah, 4x Kuning, 4x Hijau)
 * - 12x Resistor 220Ω
 * - 2x Push Button
 * - Breadboard & Kabel Jumper
 * * Koneksi untuk Perempatan (4 Set Lampu):
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
 * * Set 4: Arah Barat
 * - Pin D0  -> LED Hijau Barat
 * - Pin D16 -> LED Kuning Barat
 * - Pin D17 -> LED Merah Barat
 * * Koneksi Tombol:
 * - Pin D25 -> Tombol Darurat -> GND
 * - Pin D26 -> Tombol Pejalan Kaki -> GND
 */

#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

// Konfigurasi WiFi
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Konfigurasi Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "YOUR_AIO_USERNAME"
#define AIO_KEY         "YOUR_AIO_KEY"

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

// Pin untuk tombol
const int PIN_TOMBOL_DARURAT = 25;
const int PIN_TOMBOL_PEDESTRIAN = 26;

// State machine yang telah diperbaiki untuk lampu lalu lintas perempatan
enum TrafficState {
  // Siklus Utama
  STATE_NS_GREEN,   // 1. Utara & Selatan Hijau
  STATE_NS_YELLOW,  // 2. Utara & Selatan Kuning
  STATE_ALL_RED_1,  // 3. Semua Merah (Transisi)
  STATE_EW_GREEN,   // 4. Timur & Barat Hijau
  STATE_EW_YELLOW,  // 5. Timur & Barat Kuning
  STATE_ALL_RED_2,  // 6. Semua Merah (Transisi)
  
  // Siklus Belok Kanan Individual (dengan transisi yang benar)
  STATE_N_GREEN,    // 7. Utara Hijau (Belok)
  STATE_N_YELLOW,   // 8. Utara Kuning
  STATE_ALL_RED_3,  // 9. Semua Merah (Transisi)
  
  STATE_E_GREEN,    // 10. Timur Hijau (Belok)
  STATE_E_YELLOW,   // 11. Timur Kuning
  STATE_ALL_RED_4,  // 12. Semua Merah (Transisi)
  
  STATE_S_GREEN,    // 13. Selatan Hijau (Belok)
  STATE_S_YELLOW,   // 14. Selatan Kuning
  STATE_ALL_RED_5,  // 15. Semua Merah (Transisi)
  
  STATE_W_GREEN,    // 16. Barat Hijau (Belok)
  STATE_W_YELLOW,   // 17. Barat Kuning
  STATE_ALL_RED_6,  // 18. Semua Merah (Transisi sebelum kembali ke awal siklus)

  // Mode Khusus
  STATE_EMERGENCY,
  STATE_PEDESTRIAN
};

TrafficState currentState = STATE_ALL_RED_6; // Mulai dari state aman
TrafficState lastNormalState = STATE_ALL_RED_6; // Variabel untuk menyimpan state sebelum interupsi
unsigned long lastStateChange = 0;

// Durasi setiap state (dalam milidetik)
const unsigned long DURASI_HIJAU_UTAMA = 20000;      // 20 detik untuk lurus
const unsigned long DURASI_HIJAU_INDIVIDUAL = 10000; // 10 detik untuk belok
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

// Setup WiFi client dan MQTT
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Setup MQTT feeds
Adafruit_MQTT_Publish statusFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/traffic-status");
Adafruit_MQTT_Publish stateFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/traffic-state");
Adafruit_MQTT_Publish uptimeFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/traffic-uptime");
Adafruit_MQTT_Publish dataFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/traffic-data");
Adafruit_MQTT_Publish modeFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/traffic-mode");

void setup() {
  Serial.begin(115200);
  Serial.println("=== Proyek Lampu Lalu Lintas Perempatan (Versi Final dengan Save State) ===");
  
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
  
  pinMode(PIN_LED_HIJAU_BARAT, OUTPUT);
  pinMode(PIN_LED_KUNING_BARAT, OUTPUT);
  pinMode(PIN_LED_MERAH_BARAT, OUTPUT);

  // Setup pin tombol sebagai input dengan pull-up internal
  pinMode(PIN_TOMBOL_DARURAT, INPUT_PULLUP);
  pinMode(PIN_TOMBOL_PEDESTRIAN, INPUT_PULLUP);
  
  // Matikan semua LED di awal dan mulai state pertama
  changeToNextState(); 
  lastStateChange = millis();
  
  // Koneksi WiFi
  connectToWiFi();
  
  // Koneksi MQTT
  connectToMQTT();
  
  Serial.println("Setup selesai. Memulai sistem lampu lalu lintas perempatan...");
}

void loop() {
  // Periksa koneksi MQTT dan reconnect jika perlu
  if (!mqtt.connected()) {
    connectToMQTT();
  }
  mqtt.processPackets(100);
  
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
        digitalWrite(PIN_LED_KUNING_BARAT, HIGH);
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
    
    // Kirim data ke Adafruit IO
    sendDataToCloud();
  }
  
  // Kirim uptime setiap 30 detik
  static unsigned long lastUptimeSend = 0;
  if (currentTime - lastUptimeSend >= 30000) {
    sendUptimeToCloud();
    lastUptimeSend = currentTime;
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
    case STATE_NS_GREEN:  return DURASI_HIJAU_UTAMA;
    case STATE_EW_GREEN:  return DURASI_HIJAU_UTAMA;
    case STATE_N_GREEN:   return DURASI_HIJAU_INDIVIDUAL;
    case STATE_E_GREEN:   return DURASI_HIJAU_INDIVIDUAL;
    case STATE_S_GREEN:   return DURASI_HIJAU_INDIVIDUAL;
    case STATE_W_GREEN:   return DURASI_HIJAU_INDIVIDUAL;
    
    case STATE_NS_YELLOW: return DURASI_KUNING;
    case STATE_EW_YELLOW: return DURASI_KUNING;
    case STATE_N_YELLOW:  return DURASI_KUNING;
    case STATE_E_YELLOW:  return DURASI_KUNING;
    case STATE_S_YELLOW:  return DURASI_KUNING;
    case STATE_W_YELLOW:  return DURASI_KUNING;
      
    case STATE_ALL_RED_1: return DURASI_TRANSISI_MERAH;
    case STATE_ALL_RED_2: return DURASI_TRANSISI_MERAH;
    case STATE_ALL_RED_3: return DURASI_TRANSISI_MERAH;
    case STATE_ALL_RED_4: return DURASI_TRANSISI_MERAH;
    case STATE_ALL_RED_5: return DURASI_TRANSISI_MERAH;
    case STATE_ALL_RED_6: return DURASI_TRANSISI_MERAH;

    case STATE_EMERGENCY:  return DURASI_EMERGENCY;
    case STATE_PEDESTRIAN: return DURASI_PEDESTRIAN;

    default: return 2000; // Durasi default jika terjadi error
  }
}

TrafficState determineNextStateAfterInterruption() {
    // Fungsi ini menentukan state selanjutnya setelah interupsi,
    // memastikan siklus berlanjut dengan benar.
    switch (lastNormalState) {
        case STATE_NS_GREEN:
        case STATE_NS_YELLOW:
            return STATE_ALL_RED_1; // Lanjut ke fase Timur-Barat

        case STATE_EW_GREEN:
        case STATE_EW_YELLOW:
            return STATE_ALL_RED_2; // Lanjut ke fase Belok Utara

        case STATE_N_GREEN:
        case STATE_N_YELLOW:
            return STATE_ALL_RED_3; // Lanjut ke fase Belok Timur

        case STATE_E_GREEN:
        case STATE_E_YELLOW:
            return STATE_ALL_RED_4; // Lanjut ke fase Belok Selatan
        
        case STATE_S_GREEN:
        case STATE_S_YELLOW:
            return STATE_ALL_RED_5; // Lanjut ke fase Belok Barat

        case STATE_W_GREEN:
        case STATE_W_YELLOW:
            return STATE_ALL_RED_6; // Lanjut kembali ke fase Utara-Selatan

        // Jika interupsi terjadi saat semua lampu merah, lanjutkan ke state berikutnya yang sesuai
        case STATE_ALL_RED_1: return STATE_EW_GREEN;
        case STATE_ALL_RED_2: return STATE_N_GREEN;
        case STATE_ALL_RED_3: return STATE_E_GREEN;
        case STATE_ALL_RED_4: return STATE_S_GREEN;
        case STATE_ALL_RED_5: return STATE_W_GREEN;
        case STATE_ALL_RED_6: return STATE_NS_GREEN;

        default:
            return STATE_ALL_RED_6; // Default aman
    }
}


void changeToNextState() {
  turnOffAllLights();
  
  TrafficState nextState;

  switch (currentState) {
    case STATE_EMERGENCY:
      emergencyMode = false; // Nonaktifkan flag darurat
      nextState = determineNextStateAfterInterruption();
      Serial.println("Mode Darurat Selesai. Melanjutkan siklus...");
      break;

    case STATE_PEDESTRIAN:
      pedestrianMode = false; // Nonaktifkan flag pejalan kaki
      nextState = determineNextStateAfterInterruption();
      Serial.println("Mode Pejalan Kaki Selesai. Melanjutkan siklus...");
      break;

    case STATE_ALL_RED_6: 
      nextState = STATE_NS_GREEN;
      Serial.println("State: Utara & Selatan Hijau");
      digitalWrite(PIN_LED_HIJAU_UTARA, HIGH);
      digitalWrite(PIN_LED_HIJAU_SELATAN, HIGH);
      digitalWrite(PIN_LED_MERAH_TIMUR, HIGH);
      digitalWrite(PIN_LED_MERAH_BARAT, HIGH);
      break;

    case STATE_NS_GREEN:
      nextState = STATE_NS_YELLOW;
      Serial.println("State: Utara & Selatan Kuning");
      digitalWrite(PIN_LED_KUNING_UTARA, HIGH);
      digitalWrite(PIN_LED_KUNING_SELATAN, HIGH);
      digitalWrite(PIN_LED_MERAH_TIMUR, HIGH);
      digitalWrite(PIN_LED_MERAH_BARAT, HIGH);
      break;

    case STATE_NS_YELLOW:
      nextState = STATE_ALL_RED_1;
      Serial.println("State: Semua Merah (Transisi)");
      setAllRed();
      break;

    case STATE_ALL_RED_1:
      nextState = STATE_EW_GREEN;
      Serial.println("State: Timur & Barat Hijau");
      digitalWrite(PIN_LED_MERAH_UTARA, HIGH);
      digitalWrite(PIN_LED_MERAH_SELATAN, HIGH);
      digitalWrite(PIN_LED_HIJAU_TIMUR, HIGH);
      digitalWrite(PIN_LED_HIJAU_BARAT, HIGH);
      break;
      
    case STATE_EW_GREEN:
      nextState = STATE_EW_YELLOW;
      Serial.println("State: Timur & Barat Kuning");
      digitalWrite(PIN_LED_MERAH_UTARA, HIGH);
      digitalWrite(PIN_LED_MERAH_SELATAN, HIGH);
      digitalWrite(PIN_LED_KUNING_TIMUR, HIGH);
      digitalWrite(PIN_LED_KUNING_BARAT, HIGH);
      break;

    case STATE_EW_YELLOW:
      nextState = STATE_ALL_RED_2;
      Serial.println("State: Semua Merah (Transisi)");
      setAllRed();
      break;

    case STATE_ALL_RED_2:
      nextState = STATE_N_GREEN;
      Serial.println("State: Belok Kanan - Utara Hijau");
      digitalWrite(PIN_LED_HIJAU_UTARA, HIGH);
      digitalWrite(PIN_LED_MERAH_SELATAN, HIGH);
      digitalWrite(PIN_LED_MERAH_TIMUR, HIGH);
      digitalWrite(PIN_LED_MERAH_BARAT, HIGH);
      break;

    case STATE_N_GREEN:
      nextState = STATE_N_YELLOW;
      Serial.println("State: Belok Kanan - Utara Kuning");
      digitalWrite(PIN_LED_KUNING_UTARA, HIGH);
      digitalWrite(PIN_LED_MERAH_SELATAN, HIGH);
      digitalWrite(PIN_LED_MERAH_TIMUR, HIGH);
      digitalWrite(PIN_LED_MERAH_BARAT, HIGH);
      break;

    case STATE_N_YELLOW:
      nextState = STATE_ALL_RED_3;
      Serial.println("State: Semua Merah (Transisi)");
      setAllRed();
      break;

    case STATE_ALL_RED_3:
      nextState = STATE_E_GREEN;
      Serial.println("State: Belok Kanan - Timur Hijau");
      digitalWrite(PIN_LED_MERAH_UTARA, HIGH);
      digitalWrite(PIN_LED_MERAH_SELATAN, HIGH);
      digitalWrite(PIN_LED_HIJAU_TIMUR, HIGH);
      digitalWrite(PIN_LED_MERAH_BARAT, HIGH);
      break;

    case STATE_E_GREEN:
      nextState = STATE_E_YELLOW;
      Serial.println("State: Belok Kanan - Timur Kuning");
      digitalWrite(PIN_LED_MERAH_UTARA, HIGH);
      digitalWrite(PIN_LED_MERAH_SELATAN, HIGH);
      digitalWrite(PIN_LED_KUNING_TIMUR, HIGH);
      digitalWrite(PIN_LED_MERAH_BARAT, HIGH);
      break;

    case STATE_E_YELLOW:
      nextState = STATE_ALL_RED_4;
      Serial.println("State: Semua Merah (Transisi)");
      setAllRed();
      break;

    case STATE_ALL_RED_4:
      nextState = STATE_S_GREEN;
      Serial.println("State: Belok Kanan - Selatan Hijau");
      digitalWrite(PIN_LED_MERAH_UTARA, HIGH);
      digitalWrite(PIN_LED_HIJAU_SELATAN, HIGH);
      digitalWrite(PIN_LED_MERAH_TIMUR, HIGH);
      digitalWrite(PIN_LED_MERAH_BARAT, HIGH);
      break;
      
    case STATE_S_GREEN:
      nextState = STATE_S_YELLOW;
      Serial.println("State: Belok Kanan - Selatan Kuning");
      digitalWrite(PIN_LED_MERAH_UTARA, HIGH);
      digitalWrite(PIN_LED_KUNING_SELATAN, HIGH);
      digitalWrite(PIN_LED_MERAH_TIMUR, HIGH);
      digitalWrite(PIN_LED_MERAH_BARAT, HIGH);
      break;

    case STATE_S_YELLOW:
      nextState = STATE_ALL_RED_5;
      Serial.println("State: Semua Merah (Transisi)");
      setAllRed();
      break;

    case STATE_ALL_RED_5:
      nextState = STATE_W_GREEN;
      Serial.println("State: Belok Kanan - Barat Hijau");
      digitalWrite(PIN_LED_MERAH_UTARA, HIGH);
      digitalWrite(PIN_LED_MERAH_SELATAN, HIGH);
      digitalWrite(PIN_LED_MERAH_TIMUR, HIGH);
      digitalWrite(PIN_LED_HIJAU_BARAT, HIGH);
      break;

    case STATE_W_GREEN:
      nextState = STATE_W_YELLOW;
      Serial.println("State: Belok Kanan - Barat Kuning");
      digitalWrite(PIN_LED_MERAH_UTARA, HIGH);
      digitalWrite(PIN_LED_MERAH_SELATAN, HIGH);
      digitalWrite(PIN_LED_MERAH_TIMUR, HIGH);
      digitalWrite(PIN_LED_KUNING_BARAT, HIGH);
      break;

    case STATE_W_YELLOW:
      nextState = STATE_ALL_RED_6;
      Serial.println("State: Semua Merah (Transisi)");
      setAllRed();
      break;

    default: 
      nextState = STATE_ALL_RED_6;
      setAllRed();
      break;
  }
  currentState = nextState;
  
  // Jika state selanjutnya adalah state transisi 'semua merah', langsung jalankan logikanya
  if (currentState == STATE_ALL_RED_1 || currentState == STATE_ALL_RED_2 || currentState == STATE_ALL_RED_3 ||
      currentState == STATE_ALL_RED_4 || currentState == STATE_ALL_RED_5 || currentState == STATE_ALL_RED_6) {
      setAllRed();
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
  
  digitalWrite(PIN_LED_HIJAU_BARAT, LOW);
  digitalWrite(PIN_LED_KUNING_BARAT, LOW);
  digitalWrite(PIN_LED_MERAH_BARAT, LOW);
}

void setAllRed() {
  digitalWrite(PIN_LED_MERAH_UTARA, HIGH);
  digitalWrite(PIN_LED_MERAH_SELATAN, HIGH);
  digitalWrite(PIN_LED_MERAH_TIMUR, HIGH);
  digitalWrite(PIN_LED_MERAH_BARAT, HIGH);
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
  digitalWrite(PIN_LED_KUNING_BARAT, HIGH);
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

void connectToWiFi() {
  Serial.print("Menghubungkan ke WiFi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi terhubung!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void connectToMQTT() {
  Serial.println("Menghubungkan ke Adafruit IO...");
  
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Mencoba koneksi ulang dalam 5 detik...");
    mqtt.disconnect();
    delay(5000);
  }
  
  Serial.println("Adafruit IO terhubung!");
}

void sendDataToCloud() {
  // Kirim status lampu
  String status = getStateDescription();
  if (!statusFeed.publish(status.c_str())) {
    Serial.println("Gagal mengirim status ke Adafruit IO");
  }
  
  // Kirim state number
  if (!stateFeed.publish(currentState)) {
    Serial.println("Gagal mengirim state ke Adafruit IO");
  }
  
  // Kirim mode status
  String modeStatus = emergencyMode ? "emergency" : (pedestrianMode ? "pedestrian" : "normal");
  if (!modeFeed.publish(modeStatus.c_str())) {
    Serial.println("Gagal mengirim mode ke Adafruit IO");
  }
  
  // Kirim data JSON dengan informasi detail
  String jsonData = createJSONData();
  if (!dataFeed.publish(jsonData.c_str())) {
    Serial.println("Gagal mengirim data JSON ke Adafruit IO");
  }
  
  Serial.println("Data terkirim ke Adafruit IO: " + status);
}

String getStateDescription() {
  switch (currentState) {
    case STATE_NS_GREEN: return "Utara-Selatan Hijau";
    case STATE_NS_YELLOW: return "Utara-Selatan Kuning";
    case STATE_EW_GREEN: return "Timur-Barat Hijau";
    case STATE_EW_YELLOW: return "Timur-Barat Kuning";
    case STATE_N_GREEN: return "Utara Individual Hijau";
    case STATE_N_YELLOW: return "Utara Individual Kuning";
    case STATE_E_GREEN: return "Timur Individual Hijau";
    case STATE_E_YELLOW: return "Timur Individual Kuning";
    case STATE_S_GREEN: return "Selatan Individual Hijau";
    case STATE_S_YELLOW: return "Selatan Individual Kuning";
    case STATE_W_GREEN: return "Barat Individual Hijau";
    case STATE_W_YELLOW: return "Barat Individual Kuning";
    case STATE_ALL_RED_1:
    case STATE_ALL_RED_2:
    case STATE_ALL_RED_3:
    case STATE_ALL_RED_4:
    case STATE_ALL_RED_5:
    case STATE_ALL_RED_6: return "Semua Merah (Transisi)";
    case STATE_EMERGENCY: return "Mode Darurat";
    case STATE_PEDESTRIAN: return "Mode Pejalan Kaki";
    default: return "Unknown State";
  }
}

String createJSONData() {
  // Buat JSON data dengan informasi detail
  String json = "{";
  json += "\"timestamp\":\"" + String(millis()) + "\",";
  json += "\"state\":" + String(currentState) + ",";
  json += "\"duration\":" + String(getStateDuration()) + ",";
  json += "\"direction\":\"";
  
  switch (currentState) {
    case STATE_NS_GREEN:
    case STATE_NS_YELLOW:
      json += "utara_selatan";
      break;
    case STATE_EW_GREEN:
    case STATE_EW_YELLOW:
      json += "timur_barat";
      break;
    case STATE_N_GREEN:
    case STATE_N_YELLOW:
      json += "utara_individual";
      break;
    case STATE_E_GREEN:
    case STATE_E_YELLOW:
      json += "timur_individual";
      break;
    case STATE_S_GREEN:
    case STATE_S_YELLOW:
      json += "selatan_individual";
      break;
    case STATE_W_GREEN:
    case STATE_W_YELLOW:
      json += "barat_individual";
      break;
    case STATE_EMERGENCY:
      json += "emergency";
      break;
    case STATE_PEDESTRIAN:
      json += "pedestrian";
      break;
    default:
      json += "transisi";
      break;
  }
  
  json += "\",\"status\":\"";
  
  switch (currentState) {
    case STATE_NS_GREEN:
    case STATE_EW_GREEN:
    case STATE_N_GREEN:
    case STATE_E_GREEN:
    case STATE_S_GREEN:
    case STATE_W_GREEN:
      json += "hijau";
      break;
    case STATE_NS_YELLOW:
    case STATE_EW_YELLOW:
    case STATE_N_YELLOW:
    case STATE_E_YELLOW:
    case STATE_S_YELLOW:
    case STATE_W_YELLOW:
      json += "kuning";
      break;
    default:
      json += "merah";
      break;
  }
  
  json += "\",\"mode\":\"";
  json += emergencyMode ? "emergency" : (pedestrianMode ? "pedestrian" : "normal");
  json += "\",\"type\":\"complete\"}";
  
  return json;
}

void sendUptimeToCloud() {
  unsigned long uptime = millis() / 1000; // Konversi ke detik
  if (!uptimeFeed.publish(uptime)) {
    Serial.println("Gagal mengirim uptime ke Adafruit IO");
  } else {
    Serial.println("Uptime terkirim: " + String(uptime) + " detik");
  }
}

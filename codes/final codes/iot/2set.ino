/*
 * Proyek IoT Lampu Lalu Lintas - Perempatan Jalan Utama & Jalan Kecil (2 Set Lampu)
 * ESP32 dengan Adafruit IO Integration - VERSI UNTUK KURSUS (DENGAN FASE BELOK)
 * * Skenario: Jalan utama (Utara-Selatan) yang sibuk bersilangan dengan
 * jalan kecil. Lampu lalu lintas hanya dipasang di jalan utama.
 * * Fitur:
 * - 2 set lampu (Utara & Selatan) untuk mengontrol jalan utama.
 * - Siklus lengkap: Hijau bersamaan (lurus) & Hijau individual (belok).
 * - Mode darurat dan pejalan kaki yang berfungsi penuh.
 * - IoT Integration dengan Adafruit IO
 * * Komponen:
 * - ESP32 Dev Kit
 * - 6x LED (2x Merah, 2x Kuning, 2x Hijau)
 * - 6x Resistor 220Ω
 * - 2x Push Button
 * - Breadboard & Kabel Jumper
 * * Koneksi untuk Lampu (2 Set):
 * Set 1: Arah Utara
 * - Pin D23 -> LED Hijau Utara
 * - Pin D22 -> LED Kuning Utara  
 * - Pin D21 -> LED Merah Utara
 * * Set 2: Arah Selatan
 * - Pin D19 -> LED Hijau Selatan
 * - Pin D18 -> LED Kuning Selatan
 * - Pin D5  -> LED Merah Selatan
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

// Pin LED - 2 Set Lampu Lalu Lintas
const int PIN_LED_HIJAU_UTARA = 23;
const int PIN_LED_KUNING_UTARA = 22;
const int PIN_LED_MERAH_UTARA = 21;

// Set 2: Arah Selatan
const int PIN_LED_HIJAU_SELATAN = 19;
const int PIN_LED_KUNING_SELATAN = 18;
const int PIN_LED_MERAH_SELATAN = 5;

// Pin untuk tombol
const int PIN_TOMBOL_DARURAT = 25;
const int PIN_TOMBOL_PEDESTRIAN = 26;

// State machine dengan fase belok individual
enum TrafficState {
  STATE_NS_GREEN,   // Fase Lurus
  STATE_NS_YELLOW,
  STATE_ALL_RED_1,  // Transisi
  STATE_N_GREEN,    // Fase Belok Utara
  STATE_N_YELLOW,
  STATE_ALL_RED_2,  // Transisi
  STATE_S_GREEN,    // Fase Belok Selatan
  STATE_S_YELLOW,
  STATE_ALL_RED_3,  // Transisi kembali ke awal

  // Mode Khusus
  STATE_EMERGENCY,
  STATE_PEDESTRIAN
};

TrafficState currentState = STATE_ALL_RED_3; // Mulai dari state aman
unsigned long lastStateChange = 0;

// Durasi setiap state (dalam milidetik)
const unsigned long DURASI_HIJAU_LURUS = 15000;      // 15 detik
const unsigned long DURASI_HIJAU_BELOK = 8000;       // 8 detik
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
  Serial.println("=== Proyek IoT Lampu Lalu Lintas 2-Arah (dengan Fase Belok) ===");
  
  // Setup pin LED sebagai output
  pinMode(PIN_LED_HIJAU_UTARA, OUTPUT);
  pinMode(PIN_LED_KUNING_UTARA, OUTPUT);
  pinMode(PIN_LED_MERAH_UTARA, OUTPUT);
  
  pinMode(PIN_LED_HIJAU_SELATAN, OUTPUT);
  pinMode(PIN_LED_KUNING_SELATAN, OUTPUT);
  pinMode(PIN_LED_MERAH_SELATAN, OUTPUT);

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
  
  Serial.println("Setup selesai. Memulai sistem lampu lalu lintas 2-arah...");
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

void checkEmergencyButton() {
  // Mode darurat memiliki prioritas tertinggi
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
  if (emergencyMode) return; // Tidak bisa diaktifkan saat darurat

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
    case STATE_NS_GREEN:  return DURASI_HIJAU_LURUS;
    case STATE_N_GREEN:   return DURASI_HIJAU_BELOK;
    case STATE_S_GREEN:   return DURASI_HIJAU_BELOK;
    case STATE_NS_YELLOW:
    case STATE_N_YELLOW:
    case STATE_S_YELLOW:
      return DURASI_KUNING;
    case STATE_ALL_RED_1:
    case STATE_ALL_RED_2:
    case STATE_ALL_RED_3:
      return DURASI_TRANSISI_MERAH;
    case STATE_EMERGENCY:  return DURASI_EMERGENCY;
    case STATE_PEDESTRIAN: return DURASI_PEDESTRIAN;
    default: return 2000;
  }
}

void changeToNextState() {
  turnOffAllLights();
  
  TrafficState nextState;

  // Tentukan state selanjutnya berdasarkan state saat ini
  switch (currentState) {
    case STATE_EMERGENCY:
    case STATE_PEDESTRIAN:
      emergencyMode = false;
      pedestrianMode = false;
      nextState = STATE_ALL_RED_3; // Kembali ke state merah aman sebelum siklus baru
      Serial.println("Mode Khusus Selesai. Kembali ke siklus normal.");
      break;

    case STATE_ALL_RED_3:
      nextState = STATE_NS_GREEN;
      break;
    case STATE_NS_GREEN:
      nextState = STATE_NS_YELLOW;
      break;
    case STATE_NS_YELLOW:
      nextState = STATE_ALL_RED_1;
      break;
    case STATE_ALL_RED_1:
      nextState = STATE_N_GREEN;
      break;
    case STATE_N_GREEN:
      nextState = STATE_N_YELLOW;
      break;
    case STATE_N_YELLOW:
      nextState = STATE_ALL_RED_2;
      break;
    case STATE_ALL_RED_2:
      nextState = STATE_S_GREEN;
      break;
    case STATE_S_GREEN:
      nextState = STATE_S_YELLOW;
      break;
    case STATE_S_YELLOW:
      nextState = STATE_ALL_RED_3;
      break;
    default:
      nextState = STATE_ALL_RED_3;
      break;
  }
  
  currentState = nextState; // Update state global

  // Aktifkan lampu berdasarkan state yang BARU
  switch (currentState) {
      case STATE_NS_GREEN:
          Serial.println("State: Hijau Lurus (Utara & Selatan)");
          digitalWrite(PIN_LED_HIJAU_UTARA, HIGH);
          digitalWrite(PIN_LED_HIJAU_SELATAN, HIGH);
          break;
      case STATE_NS_YELLOW:
          Serial.println("State: Kuning (Utara & Selatan)");
          digitalWrite(PIN_LED_KUNING_UTARA, HIGH);
          digitalWrite(PIN_LED_KUNING_SELATAN, HIGH);
          break;
      case STATE_N_GREEN:
          Serial.println("State: Hijau Belok (Utara)");
          digitalWrite(PIN_LED_HIJAU_UTARA, HIGH);
          digitalWrite(PIN_LED_MERAH_SELATAN, HIGH);
          break;
      case STATE_N_YELLOW:
          Serial.println("State: Kuning (Utara)");
          digitalWrite(PIN_LED_KUNING_UTARA, HIGH);
          digitalWrite(PIN_LED_MERAH_SELATAN, HIGH);
          break;
      case STATE_S_GREEN:
          Serial.println("State: Hijau Belok (Selatan)");
          digitalWrite(PIN_LED_MERAH_UTARA, HIGH);
          digitalWrite(PIN_LED_HIJAU_SELATAN, HIGH);
          break;
      case STATE_S_YELLOW:
          Serial.println("State: Kuning (Selatan)");
          digitalWrite(PIN_LED_MERAH_UTARA, HIGH);
          digitalWrite(PIN_LED_KUNING_SELATAN, HIGH);
          break;
      case STATE_ALL_RED_1:
      case STATE_ALL_RED_2:
      case STATE_ALL_RED_3:
          Serial.println("State: Semua Merah (Transisi)");
          digitalWrite(PIN_LED_MERAH_UTARA, HIGH);
          digitalWrite(PIN_LED_MERAH_SELATAN, HIGH);
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
}

void activateEmergencyMode() {
  pedestrianMode = false; // Mode darurat membatalkan mode pejalan kaki
  emergencyMode = true;
  currentState = STATE_EMERGENCY;
  lastStateChange = millis();
  lastBlinkTime = millis();
  emergencyBlinkState = true;
  Serial.println("!!! MODE DARURAT DIAKTIFKAN (20 Detik) !!!");
  
  // Berikan umpan balik visual instan
  turnOffAllLights();
  digitalWrite(PIN_LED_KUNING_UTARA, HIGH);
  digitalWrite(PIN_LED_KUNING_SELATAN, HIGH);
}

void activatePedestrianMode() {
  pedestrianMode = true;
  currentState = STATE_PEDESTRIAN;
  lastStateChange = millis();
  Serial.println(">>> MODE PEJALAN KAKI DIAKTIFKAN (15 Detik) <<<");
  
  // Langsung set semua lampu ke merah
  turnOffAllLights();
  digitalWrite(PIN_LED_MERAH_UTARA, HIGH);
  digitalWrite(PIN_LED_MERAH_SELATAN, HIGH);
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
    case STATE_NS_GREEN: return "Hijau Lurus (Utara & Selatan)";
    case STATE_NS_YELLOW: return "Kuning (Utara & Selatan)";
    case STATE_N_GREEN: return "Hijau Belok (Utara)";
    case STATE_N_YELLOW: return "Kuning (Utara)";
    case STATE_S_GREEN: return "Hijau Belok (Selatan)";
    case STATE_S_YELLOW: return "Kuning (Selatan)";
    case STATE_ALL_RED_1:
    case STATE_ALL_RED_2:
    case STATE_ALL_RED_3: return "Semua Merah (Transisi)";
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
      json += "utara_selatan_lurus";
      break;
    case STATE_N_GREEN:
    case STATE_N_YELLOW:
      json += "utara_belok";
      break;
    case STATE_S_GREEN:
    case STATE_S_YELLOW:
      json += "selatan_belok";
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
    case STATE_N_GREEN:
    case STATE_S_GREEN:
      json += "hijau";
      break;
    case STATE_NS_YELLOW:
    case STATE_N_YELLOW:
    case STATE_S_YELLOW:
      json += "kuning";
      break;
    default:
      json += "merah";
      break;
  }
  
  json += "\",\"mode\":\"";
  json += emergencyMode ? "emergency" : (pedestrianMode ? "pedestrian" : "normal");
  json += "\",\"type\":\"2_way\"}";
  
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
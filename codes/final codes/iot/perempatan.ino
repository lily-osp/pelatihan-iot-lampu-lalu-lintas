/*
 * Proyek IoT Lampu Lalu Lintas - Perempatan Complete (4-way Traffic)
 * ESP32 dengan Adafruit IO Integration
 * 
 * Sistem traffic light perempatan yang lengkap dengan fitur:
 * - 4 arah individual (Utara, Selatan, Timur, Barat)
 * - Support untuk Belok Kanan dan lurus
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
 * Logika: Sistem mendukung individual green lights untuk Belok Kanan/kanan
 * dan timing yang realistis untuk traffic flow
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

// State machine untuk lampu lalu lintas perempatan complete
enum TrafficState {
  STATE_1,  // Utara & Selatan Hijau (Lurus & Belok Kanan), Timur & Barat Merah
  STATE_2,  // Utara & Selatan Kuning, Timur & Barat Merah
  STATE_3,  // Semua Merah (Transisi)
  STATE_4,  // Timur & Barat Hijau (Lurus & Belok Kanan), Utara & Selatan Merah
  STATE_5,  // Timur & Barat Kuning, Utara & Selatan Merah
  STATE_6,  // Semua Merah (Transisi)
  STATE_7,  // Individual: Utara Hijau (Belok Kanan), Lainnya Merah
  STATE_8,  // Individual: Timur Hijau (Belok Kanan), Lainnya Merah
  STATE_9,  // Individual: Selatan Hijau (Belok Kanan), Lainnya Merah
  STATE_10, // Individual: Barat Hijau (Belok Kanan), Lainnya Merah
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

// Emergency mode blinking variables
unsigned long lastBlinkTime = 0;
bool emergencyBlinkState = false;
const unsigned long BLINK_INTERVAL = 500; // 500ms for blinking

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
  Serial.println("=== Proyek IoT Lampu Lalu Lintas Perempatan Complete ===");
  
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
  
  // Koneksi WiFi
  connectToWiFi();
  
  // Koneksi MQTT
  connectToMQTT();
  
  Serial.println("Setup selesai. Memulai sistem lampu lalu lintas perempatan complete...");
}

void loop() {
  // Periksa koneksi MQTT dan reconnect jika perlu
  if (!mqtt.connected()) {
    connectToMQTT();
  }
  mqtt.processPackets(100);
  
  // Logika state machine non-blocking
  unsigned long currentTime = millis();
  
  // Handle emergency mode blinking
  if (emergencyMode && currentState == STATE_11) {
    if (currentTime - lastBlinkTime >= BLINK_INTERVAL) {
      emergencyBlinkState = !emergencyBlinkState;
      if (emergencyBlinkState) {
        // Turn on all yellow LEDs
        turnOnLights(PIN_LED_KUNING_UTARA, PIN_LED_KUNING_SELATAN, PIN_LED_KUNING_TIMUR, PIN_LED_KUNING_BARAT);
      } else {
        // Turn off all LEDs
        turnOffAllLights();
      }
      lastBlinkTime = currentTime;
    }
  }
  
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
      // Individual: Utara Hijau (Belok Kanan), Lainnya Merah
      turnOnLights(PIN_LED_HIJAU_UTARA, PIN_LED_MERAH_SELATAN, PIN_LED_MERAH_TIMUR, PIN_LED_MERAH_BARAT);
      Serial.println("State 7: Individual - Utara Hijau (Belok Kanan)");
      break;
      
    case STATE_7:
      currentState = STATE_8;
      // Individual: Timur Hijau (Belok Kanan), Lainnya Merah
      turnOnLights(PIN_LED_MERAH_UTARA, PIN_LED_MERAH_SELATAN, PIN_LED_HIJAU_TIMUR, PIN_LED_MERAH_BARAT);
      Serial.println("State 8: Individual - Timur Hijau (Belok Kanan)");
      break;
      
    case STATE_8:
      currentState = STATE_9;
      // Individual: Selatan Hijau (Belok Kanan), Lainnya Merah
      turnOnLights(PIN_LED_MERAH_UTARA, PIN_LED_HIJAU_SELATAN, PIN_LED_MERAH_TIMUR, PIN_LED_MERAH_BARAT);
      Serial.println("State 9: Individual - Selatan Hijau (Belok Kanan)");
      break;
      
    case STATE_9:
      currentState = STATE_10;
      // Individual: Barat Hijau (Belok Kanan), Lainnya Merah
      turnOnLights(PIN_LED_MERAH_UTARA, PIN_LED_MERAH_SELATAN, PIN_LED_MERAH_TIMUR, PIN_LED_HIJAU_BARAT);
      Serial.println("State 10: Individual - Barat Hijau (Belok Kanan)");
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
  lastBlinkTime = millis();
  emergencyBlinkState = false;
  Serial.println("Emergency Mode Activated - Blinking Yellow");
}

void setPedestrianMode() {
  pedestrianMode = true;
  currentState = STATE_12;
  turnOffAllLights();
  turnOnLights(PIN_LED_MERAH_UTARA, PIN_LED_MERAH_SELATAN, PIN_LED_MERAH_TIMUR, PIN_LED_MERAH_BARAT);
  Serial.println("Pedestrian Mode Activated");
}

void sendDataToCloud() {
  // Kirim status lampu
  String status = "State " + String(currentState + 1);
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

String createJSONData() {
  // Buat JSON data dengan informasi detail
  String json = "{";
  json += "\"timestamp\":\"" + String(millis()) + "\",";
  json += "\"state\":" + String(currentState) + ",";
  json += "\"duration\":" + String(getStateDuration()) + ",";
  json += "\"direction\":\"";
  
  switch (currentState) {
    case STATE_1:
    case STATE_2:
      json += "utara_selatan";
      break;
    case STATE_4:
    case STATE_5:
      json += "timur_barat";
      break;
    case STATE_7:
      json += "utara_individual";
      break;
    case STATE_8:
      json += "timur_individual";
      break;
    case STATE_9:
      json += "selatan_individual";
      break;
    case STATE_10:
      json += "barat_individual";
      break;
    case STATE_11:
      json += "emergency";
      break;
    case STATE_12:
      json += "pedestrian";
      break;
    default:
      json += "transisi";
      break;
  }
  
  json += "\",\"status\":\"";
  
  switch (currentState) {
    case STATE_1:
    case STATE_4:
    case STATE_7:
    case STATE_8:
    case STATE_9:
    case STATE_10:
      json += "hijau";
      break;
    case STATE_2:
    case STATE_5:
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
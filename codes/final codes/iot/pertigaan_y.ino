/*
 * Proyek IoT Lampu Lalu Lintas - Pertigaan (3-way Intersection)
 * ESP32 dengan Adafruit IO Integration
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

// Emergency mode variables
bool emergencyMode = false;
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

void setup() {
  Serial.begin(115200);
  Serial.println("=== Proyek IoT Lampu Lalu Lintas Pertigaan ===");
  
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
  
  // Koneksi WiFi
  connectToWiFi();
  
  // Koneksi MQTT
  connectToMQTT();
  
  Serial.println("Setup selesai. Memulai sistem lampu lalu lintas pertigaan...");
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
  if (emergencyMode) {
    if (currentTime - lastBlinkTime >= BLINK_INTERVAL) {
      emergencyBlinkState = !emergencyBlinkState;
      if (emergencyBlinkState) {
        // Turn on all yellow LEDs
        turnOnLights(PIN_LED_KUNING_UTAMA, PIN_LED_KUNING_CABANG1, PIN_LED_KUNING_CABANG2);
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
  
  Serial.println("Data terkirim ke Adafruit IO: " + status);
}

void sendUptimeToCloud() {
  unsigned long uptime = millis() / 1000; // Konversi ke detik
  if (!uptimeFeed.publish(uptime)) {
    Serial.println("Gagal mengirim uptime ke Adafruit IO");
  } else {
    Serial.println("Uptime terkirim: " + String(uptime) + " detik");
  }
} 
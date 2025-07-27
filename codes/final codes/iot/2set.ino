/*
 * Proyek IoT Lampu Lalu Lintas - Jalan Lurus (2-way Traffic)
 * ESP32 dengan Adafruit IO Integration
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

// Setup WiFi client dan MQTT
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Setup MQTT feeds
Adafruit_MQTT_Publish statusFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/traffic-status");
Adafruit_MQTT_Publish stateFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/traffic-state");
Adafruit_MQTT_Publish uptimeFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/traffic-uptime");

void setup() {
  Serial.begin(115200);
  Serial.println("=== Proyek IoT Lampu Lalu Lintas Jalan Lurus ===");
  
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
  
  // Koneksi WiFi
  connectToWiFi();
  
  // Koneksi MQTT
  connectToMQTT();
  
  Serial.println("Setup selesai. Memulai sistem lampu lalu lintas jalan lurus...");
}

void loop() {
  // Periksa koneksi MQTT dan reconnect jika perlu
  if (!mqtt.connected()) {
    connectToMQTT();
  }
  mqtt.processPackets(100);
  
  // Logika state machine non-blocking
  unsigned long currentTime = millis();
  
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
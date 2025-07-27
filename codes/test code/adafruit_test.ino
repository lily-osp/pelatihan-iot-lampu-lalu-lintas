/*
 * Adafruit IO Test - ESP32
 * Program untuk menguji koneksi dan komunikasi dengan Adafruit IO
 * 
 * Komponen:
 * - ESP32 Dev Kit
 * 
 * Tujuan:
 * - Menguji koneksi WiFi (prasyarat)
 * - Menguji koneksi ke Adafruit IO MQTT broker
 * - Menguji publish dan subscribe ke feeds
 * - Memverifikasi kredensial Adafruit IO
 * - Test berbagai tipe data (string, number, JSON)
 */

#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <ArduinoJson.h>

// Konfigurasi WiFi
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Konfigurasi Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "YOUR_AIO_USERNAME"
#define AIO_KEY         "YOUR_AIO_KEY"

// Setup WiFi client dan MQTT
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Setup MQTT feeds untuk testing
Adafruit_MQTT_Publish stringFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/test-string");
Adafruit_MQTT_Publish numberFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/test-number");
Adafruit_MQTT_Publish jsonFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/test-json");
Adafruit_MQTT_Publish statusFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/test-status");

Adafruit_MQTT_Subscribe controlFeed = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/test-control");

// Variables untuk testing
unsigned long messageCount = 0;
unsigned long lastPublishTime = 0;
const unsigned long PUBLISH_INTERVAL = 10000; // 10 detik

void setup() {
  Serial.begin(115200);
  Serial.println("=== Adafruit IO Test - ESP32 ===");
  
  // Test 1: Koneksi WiFi
  if (!testWiFiConnection()) {
    Serial.println("✗ WiFi gagal. Test Adafruit IO dibatalkan.");
    return;
  }
  
  // Test 2: Koneksi Adafruit IO
  if (!testAdafruitIOConnection()) {
    Serial.println("✗ Adafruit IO gagal. Test dibatalkan.");
    return;
  }
  
  // Test 3: Subscribe ke feed
  testSubscribe();
  
  Serial.println("=== Setup Test Selesai ===");
  Serial.println("Memulai monitoring dan publishing...");
}

void loop() {
  // Periksa koneksi MQTT dan reconnect jika perlu
  if (!mqtt.connected()) {
    Serial.println("MQTT terputus, mencoba reconnect...");
    connectToMQTT();
  }
  mqtt.processPackets(100);
  
  // Publish test data setiap interval
  unsigned long currentTime = millis();
  if (currentTime - lastPublishTime >= PUBLISH_INTERVAL) {
    publishTestData();
    lastPublishTime = currentTime;
  }
}

bool testWiFiConnection() {
  Serial.println("\n=== Test 1: Koneksi WiFi ===");
  Serial.print("Menghubungkan ke WiFi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("✓ WiFi terhubung!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println();
    Serial.println("✗ Gagal terhubung ke WiFi!");
    Serial.println("Periksa SSID dan password WiFi Anda.");
    return false;
  }
}

bool testAdafruitIOConnection() {
  Serial.println("\n=== Test 2: Koneksi Adafruit IO ===");
  Serial.print("Username: ");
  Serial.println(AIO_USERNAME);
  Serial.print("Key: ");
  Serial.println(AIO_KEY);
  Serial.println();
  
  Serial.println("Menghubungkan ke Adafruit IO MQTT broker...");
  
  int8_t ret;
  int attempts = 0;
  while ((ret = mqtt.connect()) != 0 && attempts < 5) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Mencoba koneksi ulang dalam 5 detik...");
    mqtt.disconnect();
    delay(5000);
    attempts++;
  }
  
  if (ret == 0) {
    Serial.println("✓ Adafruit IO MQTT broker terhubung!");
    return true;
  } else {
    Serial.println("✗ Gagal terhubung ke Adafruit IO!");
    Serial.println("Periksa kredensial Adafruit IO Anda:");
    Serial.println("- Username harus benar");
    Serial.println("- Key harus valid dan aktif");
    Serial.println("- Pastikan akun Adafruit IO Anda aktif");
    return false;
  }
}

void testSubscribe() {
  Serial.println("\n=== Test 3: Subscribe ke Feed ===");
  
  // Setup callback untuk control feed
  controlFeed.setCallback(controlCallback);
  
  // Subscribe ke control feed
  mqtt.subscribe(&controlFeed);
  Serial.println("✓ Berhasil subscribe ke test-control feed");
  Serial.println("Feed URL: " + String(AIO_USERNAME) + "/feeds/test-control");
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
  
  Serial.println("✓ Adafruit IO terhubung!");
  mqtt.subscribe(&controlFeed);
}

void publishTestData() {
  messageCount++;
  Serial.println("\n=== Publishing Test Data ===");
  
  // Test 1: String data
  String stringMessage = "Hello from ESP32 - Message #" + String(messageCount);
  if (stringFeed.publish(stringMessage.c_str())) {
    Serial.println("✓ String data terkirim: " + stringMessage);
  } else {
    Serial.println("✗ Gagal mengirim string data");
  }
  
  // Test 2: Number data
  int randomNumber = random(1, 100);
  if (numberFeed.publish(randomNumber)) {
    Serial.println("✓ Number data terkirim: " + String(randomNumber));
  } else {
    Serial.println("✗ Gagal mengirim number data");
  }
  
  // Test 3: JSON data
  StaticJsonDocument<200> doc;
  doc["device"] = "ESP32";
  doc["message_id"] = messageCount;
  doc["timestamp"] = millis();
  doc["temperature"] = random(20, 30);
  doc["humidity"] = random(40, 80);
  doc["status"] = "online";
  
  String jsonString;
  serializeJson(doc, jsonString);
  
  if (jsonFeed.publish(jsonString.c_str())) {
    Serial.println("✓ JSON data terkirim: " + jsonString);
  } else {
    Serial.println("✗ Gagal mengirim JSON data");
  }
  
  // Test 4: Status data
  String statusMessage = "ESP32 Online - Uptime: " + String(millis() / 1000) + "s";
  if (statusFeed.publish(statusMessage.c_str())) {
    Serial.println("✓ Status data terkirim: " + statusMessage);
  } else {
    Serial.println("✗ Gagal mengirim status data");
  }
  
  Serial.println("Total messages sent: " + String(messageCount));
}

// Callback function untuk menerima data dari subscribe
void controlCallback(char *data, uint16_t len) {
  Serial.println("\n=== Received Control Message ===");
  Serial.print("Data: ");
  Serial.println(data);
  Serial.print("Length: ");
  Serial.println(len);
  
  // Parse JSON control message
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, data);
  
  if (!error) {
    if (doc.containsKey("command")) {
      String command = doc["command"];
      Serial.println("Command received: " + command);
      
      if (command == "restart") {
        Serial.println("Restart command received!");
        // Implement restart logic here
      } else if (command == "status") {
        Serial.println("Status request received!");
        // Send status back
        String statusResponse = "{\"device\":\"ESP32\",\"status\":\"online\",\"uptime\":" + String(millis() / 1000) + "}";
        statusFeed.publish(statusResponse.c_str());
      }
    }
  } else {
    Serial.println("Failed to parse JSON control message");
  }
} 
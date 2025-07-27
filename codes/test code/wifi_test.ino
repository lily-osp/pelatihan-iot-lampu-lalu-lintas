/*
 * WiFi Test - ESP32
 * Program untuk menguji koneksi WiFi dan Adafruit IO
 * 
 * Komponen:
 * - ESP32 Dev Kit
 * 
 * Tujuan:
 * - Menguji koneksi WiFi
 * - Menguji koneksi ke Adafruit IO
 * - Memverifikasi kredensial
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

// Setup WiFi client dan MQTT
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Setup MQTT feeds untuk testing
Adafruit_MQTT_Publish testFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/test-feed");
Adafruit_MQTT_Subscribe testSubscribe = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/test-feed");

void setup() {
  Serial.begin(115200);
  Serial.println("=== WiFi & Adafruit IO Test ===");
  
  // Test 1: Koneksi WiFi
  testWiFiConnection();
  
  // Test 2: Koneksi Adafruit IO
  testAdafruitIOConnection();
  
  Serial.println("=== Test Selesai ===");
}

void loop() {
  // Periksa koneksi MQTT dan reconnect jika perlu
  if (!mqtt.connected()) {
    Serial.println("MQTT terputus, mencoba reconnect...");
    connectToMQTT();
  }
  mqtt.processPackets(100);
  
  // Kirim test message setiap 10 detik
  static unsigned long lastSend = 0;
  if (millis() - lastSend >= 10000) {
    sendTestMessage();
    lastSend = millis();
  }
}

void testWiFiConnection() {
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
    Serial.print("Signal Strength (RSSI): ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println();
    Serial.println("✗ Gagal terhubung ke WiFi!");
    Serial.println("Periksa SSID dan password WiFi Anda.");
  }
}

void testAdafruitIOConnection() {
  Serial.println("\n=== Test 2: Koneksi Adafruit IO ===");
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("✗ WiFi tidak terhubung. Tidak dapat test Adafruit IO.");
    return;
  }
  
  Serial.println("Menghubungkan ke Adafruit IO...");
  
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
    Serial.println("✓ Adafruit IO terhubung!");
    
    // Test publish
    if (testFeed.publish("Hello from ESP32!")) {
      Serial.println("✓ Berhasil publish test message");
    } else {
      Serial.println("✗ Gagal publish test message");
    }
    
    // Subscribe ke feed
    mqtt.subscribe(&testSubscribe);
    Serial.println("✓ Berhasil subscribe ke test feed");
    
  } else {
    Serial.println("✗ Gagal terhubung ke Adafruit IO!");
    Serial.println("Periksa kredensial Adafruit IO Anda:");
    Serial.print("Username: ");
    Serial.println(AIO_USERNAME);
    Serial.print("Key: ");
    Serial.println(AIO_KEY);
  }
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
  mqtt.subscribe(&testSubscribe);
}

void sendTestMessage() {
  String message = "Test message dari ESP32 - " + String(millis() / 1000) + "s";
  if (testFeed.publish(message.c_str())) {
    Serial.println("✓ Test message terkirim: " + message);
  } else {
    Serial.println("✗ Gagal mengirim test message");
  }
} 
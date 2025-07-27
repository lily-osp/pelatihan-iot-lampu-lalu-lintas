/*
 * WiFi Test - ESP32
 * Program untuk menguji koneksi WiFi
 * 
 * Komponen:
 * - ESP32 Dev Kit
 * 
 * Tujuan:
 * - Menguji koneksi WiFi
 * - Memverifikasi kredensial WiFi
 * - Menampilkan informasi koneksi WiFi
 */

#include <WiFi.h>

// Konfigurasi WiFi
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

void setup() {
  Serial.begin(115200);
  Serial.println("=== WiFi Test - ESP32 ===");
  
  // Test koneksi WiFi
  testWiFiConnection();
  
  Serial.println("=== Test WiFi Selesai ===");
}

void loop() {
  // Monitor status WiFi setiap 30 detik
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck >= 30000) {
    monitorWiFiStatus();
    lastCheck = millis();
  }
}

void testWiFiConnection() {
  Serial.println("\n=== Test Koneksi WiFi ===");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Password: ");
  Serial.println(password);
  Serial.println();
  
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
    Serial.println();
    Serial.println("=== Informasi Koneksi WiFi ===");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Gateway: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("Subnet Mask: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("DNS Server: ");
    Serial.println(WiFi.dnsIP());
    Serial.print("Signal Strength (RSSI): ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    Serial.print("WiFi Channel: ");
    Serial.println(WiFi.channel());
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
  } else {
    Serial.println();
    Serial.println("✗ Gagal terhubung ke WiFi!");
    Serial.println("Periksa SSID dan password WiFi Anda.");
    Serial.println("Pastikan WiFi router Anda aktif dan dalam jangkauan.");
  }
}

void monitorWiFiStatus() {
  Serial.println("\n=== Monitor Status WiFi ===");
  Serial.print("Status: ");
  
  switch (WiFi.status()) {
    case WL_CONNECTED:
      Serial.println("✓ Terhubung");
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());
      Serial.print("RSSI: ");
      Serial.print(WiFi.RSSI());
      Serial.println(" dBm");
      break;
    case WL_DISCONNECTED:
      Serial.println("✗ Terputus");
      Serial.println("Mencoba reconnect...");
      WiFi.reconnect();
      break;
    case WL_CONNECT_FAILED:
      Serial.println("✗ Gagal koneksi");
      break;
    case WL_NO_SSID_AVAIL:
      Serial.println("✗ SSID tidak ditemukan");
      break;
    case WL_WRONG_PASSWORD:
      Serial.println("✗ Password salah");
      break;
    default:
      Serial.println("✗ Status tidak diketahui");
      break;
  }
} 
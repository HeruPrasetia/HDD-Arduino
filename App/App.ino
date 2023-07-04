#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <SD.h>
#include <TimeLib.h> 

//const char* ssid = "TP-LinkHome";
//const char* password = "HomeITOffice78";
const char* ssid = "GRATIS";
const char* password = "Bissmillah123";
const char* apSSID = "HDD External";
const char* apPassword = "naylatools";
const int chipSelect = D8;

ESP8266WebServer server(80);
File uploadFile;

void handleCORS() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
  server.send(204);
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  
  if (!SD.begin(chipSelect)) {
    Serial.println("Kartu SD tidak terdeteksi.");
    return;
  }
  
  Serial.println("Kartu SD terdeteksi.");

  // Menghubungkan ke jaringan WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Terhubung ke jaringan WiFi...");
  }
  Serial.println("Terhubung ke jaringan WiFi.");

  // Mengecek apakah terhubung ke jaringan WiFi
  if (WiFi.status() == WL_CONNECTED) {
    // Mengatur mode WiFi sebagai Station (terhubung ke router)
    WiFi.mode(WIFI_STA);
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    // Mengatur mode WiFi sebagai Access Point (AP)
    WiFi.mode(WIFI_AP);
    WiFi.softAP(apSSID, apPassword);
    Serial.print("Access Point IP Address: ");
    Serial.println(WiFi.softAPIP());
  }

  server.on("/", handleRoot);
  server.on("/api", HTTP_OPTIONS, handleCORS);
  server.on("/api", handleApi);
  server.on("/file", handleDownload);

  server.begin();
  Serial.println("Web server dimulai.");
}

void loop() {
  server.handleClient();
}

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "GRATIS";
const char* password = "Bissmillah123";
const char* apSSID = "HDD External";
const char* apPassword = "naylatools";

ESP8266WebServer server(80);

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Selamat datang di web server Wemos D1 Mini</h1>";
  html += "<p>Silakan klik <a href='/client'>di sini</a> untuk mengakses web client.</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleClient() {
  // Kode untuk mengakses web server lain sebagai client
}

void setup() {
  Serial.begin(9600);

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
  server.on("/client", handleClient);

  server.begin();
  Serial.println("Web server dimulai.");
}

void loop() {
  server.handleClient();
}

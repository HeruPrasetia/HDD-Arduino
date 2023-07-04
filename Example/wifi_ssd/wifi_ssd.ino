#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SD.h>

const char* ssid = "GRATIS";
const char* password = "Bissmillah123";
const int chipSelect = D8;

ESP8266WebServer server(80);
File uploadFile;

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Selamat datang di server Wemos D1 Mini</h1>";
  html += "<form enctype='multipart/form-data' method='POST' action='/upload'>";
  html += "<input type='file' name='fileToUpload'>";
  html += "<input type='submit' value='Unggah'>";
  html += "</form>";
  html += "<h2>Daftar file di kartu SD:</h2>";
  html += getFileList();
  html += "</body></html>";
  server.send(200, "text/html", html);
}

String getFileList() {
  String fileList = "";
  File root = SD.open("/");
  while (File entry = root.openNextFile()) {
    fileList += "<p>" + String(entry.name()) + "</p>";
    entry.close();
  }
  return fileList;
}

void handleUpload() {
  HTTPUpload& upload = server.upload();
  Serial.print(upload.status);
  if (upload.status == 1) {
    String filename = "/" + upload.filename;
    Serial.print("Menerima file: ");
    Serial.println(filename);
    if (SD.exists(filename)) {
      SD.remove(filename);
    }
    uploadFile = SD.open(filename, FILE_WRITE);
    if (uploadFile) {
      Serial.println("Membuka file...");
      server.send(200, "text/html", "<h1>File berhasil ditransfer ke kartu SD.</h1>");
    } else {
      Serial.println("Gagal membuka file.");
      server.send(200, "text/html", "<h1>File berhasil ditransfer ke kartu SD.</h1>");
    }
  } else if (upload.status == 2) {
    if (uploadFile) {
      uploadFile.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == 3) {
    if (uploadFile) {
      uploadFile.close();
      Serial.println("File berhasil ditransfer ke kartu SD.");
      server.send(200, "text/html", "<h1>File berhasil ditransfer ke kartu SD.</h1>");
    }
  }else{
    server.send(200, "text/html", "<h1>"+String(upload.status)+"</h1>");
  }
}

void setup() {
  Serial.begin(9600);

  // Inisialisasi komunikasi SPI
  SPI.begin();

  // Periksa apakah kartu SD terdeteksi
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

  server.on("/", HTTP_GET, handleRoot);
  server.on("/upload", HTTP_POST, handleUpload);

  server.begin();
  Serial.println("Server dimulai.");
}

void loop() {
  server.handleClient();
}

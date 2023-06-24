#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SD.h>

const char* ssid = "NamaSSID";
const char* password = "PasswordWiFi";
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
  while (true) {
    File entry = root.openNextFile();
    if (!entry) {
      break;
    }
    fileList += "<p>" + String(entry.name()) + "</p>";
    entry.close();
  }
  return fileList;
}

void handleUpload() {
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = "/"+upload.filename;
    Serial.print("Menerima file: ");
    Serial.println(filename);
    if (SD.exists(filename)) {
      SD.remove(filename);
    }
    uploadFile = SD.open(filename, FILE_WRITE);
    if (uploadFile) {
      Serial.println("Membuka file...");
    } else {
      Serial.println("Gagal membuka file.");
    }
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (uploadFile) {
      uploadFile.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (uploadFile) {
      uploadFile.close();
      Serial.println("File berhasil ditransfer ke kartu SD.");
    }
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

  server.on("/", handleRoot);
  server.on("/upload", HTTP_POST, handleUpload);

  server.begin();
  Serial.println("Server dimulai.");
}

void loop() {
  server.handleClient();
}

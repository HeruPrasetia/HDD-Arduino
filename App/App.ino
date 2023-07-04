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
  File htmlFile = SD.open("/system/index.html");
  if (htmlFile) {
    String htmlContent = "";
    while (htmlFile.available()) {
      htmlContent += htmlFile.readStringUntil('\n');
    }
    htmlFile.close();

    File jsFile1 = SD.open("/system/static/js/main.931e79f9.js");
    if (jsFile1) {
      String jsContent1 = "";
      while (jsFile1.available()) {
        jsContent1 += jsFile1.readStringUntil('\n');
      }
      jsFile1.close();

      // Ganti tanda penanda khusus dengan konten file JavaScript
      htmlContent.replace("{{SCRIPT1}}", jsContent1);

      // Lanjutkan dengan file JavaScript lainnya (jika ada)
      // ...

      server.send(200, "text/html", htmlContent);
    } else {
      server.send(404, "text/plain", "JavaScript file not found");
    }

     // Membaca konten file CSS
    File cssFile = SD.open("/system/css/main.003f8338.css");
    if (cssFile) {
      String cssContent = "";
      while (cssFile.available()) {
        cssContent += cssFile.readStringUntil('\n');
      }
      cssFile.close();

      // Menambahkan tag <link> untuk file CSS
      htmlContent.replace("{{CSS}}", "<style>" + cssContent + "</style>");

      // Lanjutkan dengan file JavaScript dan operasi lainnya (jika ada)
      // ...

      server.send(200, "text/html", htmlContent);
    } else {
      server.send(404, "text/plain", "CSS file not found");
    }
  } else {
    server.send(404, "text/plain", "HTML file not found");
  }
}

String getContentType(const String& filename) {
  if (filename.endsWith(".html") || filename.endsWith(".htm")) {
    return "text/html";
  } else if (filename.endsWith(".css")) {
    return "text/css";
  } else if (filename.endsWith(".js")) {
    return "application/javascript";
  } else if (filename.endsWith(".txt")) {
    return "text/plain";
  } else if (filename.endsWith(".png")) {
    return "image/png";
  } else if (filename.endsWith(".jpg") || filename.endsWith(".jpeg")) {
    return "image/jpeg";
  } else if (filename.endsWith(".gif")) {
    return "image/gif";
  } else if (filename.endsWith(".pdf")) {
    return "application/pdf";
  } else {
    return "application/octet-stream";
  }
}


void handleGetRequest() {
  String path = server.uri();
  
  // Menghapus karakter awal '/' dari path
  path.remove(0, 1);
  
  // Memisahkan path menjadi komponen direktori dan nama file
  int lastSlash = path.lastIndexOf('/');
  String directory = path.substring(0, lastSlash);
  String filename = path.substring(lastSlash + 1);
  
  // Membuka file sesuai dengan path yang diminta
  File file = SD.open("/" + directory + "/" + filename);
  
  if (file) {
    // Mendapatkan tipe konten berdasarkan ekstensi file
    String contentType = getContentType(filename);
    
    if (contentType == "text/plain" || contentType == "text/html" || contentType == "text/css" || contentType == "application/javascript") {
      // Membaca isi file sebagai teks
      String fileContent = "";
      while (file.available()) {
        fileContent += file.readStringUntil('\n');
      }
      file.close();
      
      // Mengirimkan konten sebagai teks
      server.send(200, contentType, fileContent);
    } else {
      // Mengirimkan file sebagai unduhan
      server.sendHeader("Content-Disposition", "attachment; filename=" + filename);
      server.send(200, contentType, "");
      while (file.available()) {
        server.sendContent(String(file.read()));
      }
      file.close();
      server.client().stop();
    }
  } else {
    server.send(404, "text/plain", "File not found");
  }
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
    String filename = "/" + upload.filename;
    Serial.print("Menerima file: ");
    Serial.println(filename);
    if (SD.exists(filename)) {
      SD.remove(filename);
    }
    uploadFile = SD.open(filename, FILE_WRITE);
    if (uploadFile) {
      server.send(200, "text/html", "<h1>Membuka file</h1>");
      Serial.println("Membuka file...");
    } else {
      server.send(200, "text/html", "<h1>Gagal membuka file</h1>");
      Serial.println("Gagal membuka file.");
    }
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (uploadFile) {
      uploadFile.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (uploadFile) {
      uploadFile.close();
      server.send(200, "text/html", "<h1>File berhasil diunggah</h1>");
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

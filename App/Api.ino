void handleRoot() {
  File htmlFile = SD.open("/system/index.html");
  if (htmlFile) {
    String htmlContent = "";
    while (htmlFile.available()) {
      htmlContent += htmlFile.readStringUntil('\n');
    }
    htmlFile.close();
    server.send(200, "text/html", htmlContent);
  } else {
    server.send(404, "text/html", "<h4>Maaf OS Tidak dapat berjalan</h4>");
  }
}


void handleApi() {
  const char* act = server.arg("act").c_str();
  
  if (strcmp(act, "list_files") == 0) {
    String path = server.arg("path");
    path.replace("-", "/");
    getListFiles(path);
  } else {
    server.send(200, "application/json", "{status:'gagal', pesan:'Tidak ada perintah'}");
  }
}


void getListFiles(const String& path) {
  File directory = SD.open(path);

  if (!directory.isDirectory()) {
    server.send(500, "application/json", "{'status':'gagal', 'pesan':'Path is not a directory'}");
    return;
  }

  String fileList = "{\"status\":\"sukses\", \"data\":[";

  while (true) {
    File entry = directory.openNextFile();

    if (!entry) {
      break;
    }

    String name = entry.name();
    size_t size = entry.size();
    time_t lastModified = entry.getLastWrite();

    // Mendapatkan informasi tahun, bulan, dan tanggal dari tanggal terakhir modifikasi
    struct tm *tmLastModified = localtime(&lastModified);
    char formattedDate[11]; // Buffer untuk menyimpan tanggal yang diformat

    // Mengformat tanggal menjadi "YYYY-mm-dd"
    sprintf(formattedDate, "%04d-%02d-%02d", tmLastModified->tm_year + 1900, tmLastModified->tm_mon + 1, tmLastModified->tm_mday);

    String extension = ""; // Menyimpan jenis file (file extension)
    int dotIndex = name.lastIndexOf('.');
    if (dotIndex >= 0 && dotIndex < name.length() - 1) {
      extension = name.substring(dotIndex + 1);
    }

    // Menambahkan jumlah file jika jenis file adalah folder (direktori)
//    int fileCount = 0;
//    if (entry.isDirectory()) {
//      File subdir = SD.open(name);
//      while (subdir.openNextFile()) {
//        fileCount++;
//      }
//      subdir.close();
//    }

    fileList += "{\"Name\":\"" + name + "\",\"Size\":" + String(size) + ",\"Date\":\"" + String(formattedDate) + "\",\"Extension\":\"" + extension + "\",\"FileCount\":0},";
    entry.close();
  }

  if (fileList.endsWith(",")) {
    fileList.remove(fileList.length() - 1);
  }

  fileList += "]}";

  server.send(200, "application/json", fileList);
}

void handleDownload() {
   String path = server.arg("path");
   path.replace("-", "/");
  
    if (path.isEmpty()) {
    server.send(400, "text/plain", "Invalid request");
    return;
  }

  File file = SD.open(path, FILE_READ);
  if (!file) {
    server.send(404, "text/plain", "File not found");
    return;
  }

  size_t fileSize = file.size();
  server.sendHeader("Content-Type", "application/octet-stream");
  server.sendHeader("Content-Disposition", "attachment; filename=" + String(file.name()));
  server.sendHeader("Content-Length", String(fileSize));
  server.sendHeader("Connection", "close");
  server.send(200);

  const int bufferSize = 1024;
  uint8_t buffer[bufferSize];
  size_t bytesSent = 0;

  while (bytesSent < fileSize) {
    size_t bytesRead = file.readBytes(reinterpret_cast<char*>(buffer), bufferSize);
    server.sendContent(reinterpret_cast<const char*>(buffer), bytesRead);
    bytesSent += bytesRead;
  }

  file.close();
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

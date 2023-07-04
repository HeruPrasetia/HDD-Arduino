
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

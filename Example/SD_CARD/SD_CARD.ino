#include <SPI.h>
#include <SD.h>

const int chipSelect = D8; // Pin CS pada Wemos D1 Mini Micro SD Shield

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

  // Buat file baru dengan nama "data.txt" pada kartu SD
  File dataFile = SD.open("data.txt", FILE_WRITE);

  // Periksa apakah file berhasil dibuka
  if (dataFile) {
    // Tulis data ke file
    dataFile.println("Contoh data yang ditulis ke kartu SD.");
    dataFile.close();
    Serial.println("Data berhasil ditulis ke kartu SD.");
  } else {
    Serial.println("Gagal membuka file.");
  }
}

void loop() {
  // Tidak ada yang perlu dilakukan di loop
}

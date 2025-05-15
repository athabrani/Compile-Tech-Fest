#include <LiquidCrystal_I2C.h>  // Library untuk LCD I2C

// Inisialisasi LCD I2C dengan alamat 0x20 (sebaiknya periksa kembali, umum 0x27 atau 0x3F)
LiquidCrystal_I2C lcd(32, 16, 2);  // (alamat I2C, jumlah kolom, jumlah baris)

// Deklarasi pin
const int buzzerPin = 10;         // Pin buzzer
const int ledRed = 12;            // Pin LED merah (indikasi kering)
const int ledGreen = 11;          // Pin LED hijau (indikasi pompa aktif)
const int soilSensorPin = A0;     // Pin sensor kelembaban tanah (analog)
const int waterPump = 13;         // Pin pompa air (output)

void setup() {
  pinMode(buzzerPin, OUTPUT);     // Set pin buzzer sebagai output
  pinMode(ledRed, OUTPUT);        // Set pin LED merah sebagai output
  pinMode(ledGreen, OUTPUT);      // Set pin LED hijau sebagai output
  pinMode(waterPump, OUTPUT);     // Set pin pompa sebagai output

  Serial.begin(9600);             // Memulai komunikasi serial dengan baudrate 9600

  lcd.init();                     // Inisialisasi LCD
  lcd.backlight();                // Menyalakan lampu latar LCD

  // Menampilkan pesan awal di LCD
  lcd.setCursor(0, 0);            
  lcd.print("Automated Plant");   // Baris pertama
  lcd.setCursor(0, 1);
  lcd.print("Watering System");   // Baris kedua
  delay(2500);                    // Tahan tampilan selama 2.5 detik

  lcd.clear();                    // Bersihkan tampilan LCD
  lcd.setCursor(0, 0);
  lcd.print("Moisture =");        // Label untuk kelembaban
  lcd.setCursor(0, 1);
  lcd.print("Water Pump =");      // Label untuk status pompa
}

void loop() {
  int sensorValue = analogRead(soilSensorPin);   // Baca nilai analog dari sensor
  int moisture = map(sensorValue, 0, 876, 0, 99); // Konversi nilai sensor ke skala 0–99

  Serial.print("Moisture: ");     // Cetak nilai kelembaban ke Serial Monitor
  Serial.println(moisture);

  // Tampilkan nilai kelembaban pada LCD
  lcd.setCursor(11, 0);           // Posisi setelah "Moisture ="
  lcd.print(moisture);            // Cetak nilai kelembaban
  lcd.print("%");                 // Tambahkan simbol persen

  lcd.setCursor(13, 1);           // Posisi setelah "Water Pump ="
  if (moisture < 50) {            // Jika kelembaban < 50% maka nyalakan pompa
    digitalWrite(waterPump, HIGH); // Aktifkan pompa
    digitalWrite(ledGreen, HIGH);  // Nyalakan LED hijau
    digitalWrite(ledRed, LOW);     // Matikan LED merah
    lcd.print("ON ");              // Tampilkan status ON di LCD
    playSound();                   // Aktifkan bunyi buzzer
  } else {
    digitalWrite(waterPump, LOW);  // Matikan pompa
    digitalWrite(ledGreen, LOW);   // Matikan LED hijau
    digitalWrite(ledRed, HIGH);    // Nyalakan LED merah
    lcd.print("OFF");              // Tampilkan status OFF di LCD
  }
}

void playSound() {
  tone(buzzerPin, 1000, 100);     // Bunyi buzzer dengan frekuensi 1000Hz selama 100ms
  delay(1000);                    // Delay 1 detik untuk jeda
  noTone(buzzerPin);              // Matikan bunyi buzzer
}

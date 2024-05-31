#include <SoftwareSerial.h>

#define rainSensor 13 // Yağmur sensörü
#define zil 8         // Zil
#define RX 7          // Bluetooth RX pin
#define TX 6          // Bluetooth TX pin

bool rainStatus = false; // Yağmur durumu
bool lastRainStatus = false; // Son yağmur durumu
bool zilState = false;   // Zil durumu

SoftwareSerial bluetooth(RX, TX); // Bluetooth modülünün bağlantıları

void setup() {
  pinMode(rainSensor, INPUT);
  pinMode(zil, OUTPUT);
  bluetooth.begin(9600);
  Serial.begin(9600);
  Serial.println("Hazır");
}

void loop() {
  // Sensörden yağmur durumunu al
  rainStatus = digitalRead(rainSensor);

  // Eğer yağmur başladıysa
  if (rainStatus == HIGH && lastRainStatus == LOW) {
    zilState = true; // Zili aç
    bluetooth.println("Yağmur yağıyor!"); // Bluetooth ile mesaj gönder
  }

  // Eğer yağmur durduysa
  if (rainStatus == LOW && lastRainStatus == HIGH) {
    zilState = false; // Zili kapat
  }

  // Eğer zil açıksa ve Bluetooth'tan kapat komutu geldiyse
  if (zilState && bluetooth.available()) {
    String veri = bluetooth.readString(); // Gelen komutu oku
    veri.trim(); // Başta ve sonda boşlukları kaldır

    if (veri.equalsIgnoreCase("kapat")) { // Kapat komutu geldiyse
      zilState = false; // Zili kapat
    }
  }

  // Zil durumuna göre zili kontrol et
  digitalWrite(zil, zilState ? HIGH : LOW);
  
  // Yağmur durumunu kaydet
  lastRainStatus = rainStatus;

  delay(1000); // 1 saniye bekle
}

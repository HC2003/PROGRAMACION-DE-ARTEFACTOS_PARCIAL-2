// Ejercicio 4: Control de acceso con RFID RC522  |  Placa: ESP32 (DevKit WiFi + Bluetooth)
// Librería: MFRC522 (miguelbalboa). Alimentar el RC522 SOLO con 3.3V.
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN   5
#define RST_PIN  22
#define SCK_PIN  18
#define MOSI_PIN 23
#define MISO_PIN 19
#define LED_VERDE 25
#define LED_ROJO  26

MFRC522 rfid(SS_PIN, RST_PIN);

// UIDs autorizados: léelos primero por el monitor serial y pégalos aquí
const String AUTORIZADOS[] = {"A1B2C3D4", "11223344"};
const int N_AUTORIZADOS = sizeof(AUTORIZADOS) / sizeof(AUTORIZADOS[0]);

String leerUID() {
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uid += "0";            // relleno a 2 dígitos
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();
  return uid;
}

bool esAutorizado(String uid) {
  for (int i = 0; i < N_AUTORIZADOS; i++) if (uid == AUTORIZADOS[i]) return true;
  return false;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);          // pines SPI explícitos
  rfid.PCD_Init();
  Serial.println("Acerque una tarjeta...");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;  // espera tarjeta

  String uid = leerUID();
  bool ok = esAutorizado(uid);
  Serial.println("UID: " + uid + (ok ? " -> ACCESO PERMITIDO" : " -> ACCESO DENEGADO"));

  digitalWrite(ok ? LED_VERDE : LED_ROJO, HIGH);           // verde = permitido, rojo = denegado
  delay(1500);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_ROJO, LOW);

  rfid.PICC_HaltA();                                       // detiene la lectura de esa tarjeta
  rfid.PCD_StopCrypto1();
}

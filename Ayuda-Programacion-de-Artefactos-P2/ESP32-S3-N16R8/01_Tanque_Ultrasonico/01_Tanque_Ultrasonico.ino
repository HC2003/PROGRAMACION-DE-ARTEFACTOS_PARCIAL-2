// Ejercicio 1: Nivel de tanque con HC-SR04 + Adafruit IO (MQTT)  |  Placa: ESP32-S3 N16R8
// Feeds a crear en Adafruit IO: nivel-tanque, estado-tanque
#include <WiFi.h>
#include <PubSubClient.h>

const char* WIFI_SSID = "TU_WIFI";
const char* WIFI_PASS = "TU_CLAVE";
const char* AIO_USER  = "TU_USUARIO_AIO";
const char* AIO_KEY   = "TU_AIO_KEY";

#define TRIG_PIN  5
#define ECHO_PIN  6   // ECHO entrega 5V: usa divisor de voltaje 1k/2k
#define LED_LLENO 7

// Calibración: distancia (cm) del sensor al agua
const float DIST_VACIO = 30.0;  // tanque vacío
const float DIST_LLENO = 5.0;   // tanque lleno

WiFiClient wifi;
PubSubClient mqtt(wifi);
float nivelAnterior;
unsigned long ultimoEnvio = 0;

float medirDistancia() {
  digitalWrite(TRIG_PIN, LOW);  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);   // pulso de disparo
  digitalWrite(TRIG_PIN, LOW);
  long us = pulseIn(ECHO_PIN, HIGH, 30000);              // tiempo del eco (timeout 30 ms)
  if (us == 0) return -1;                                // sin lectura
  return us * 0.0343 / 2.0;                              // cm
}

float leerNivel() {  // porcentaje 0-100
  float d = medirDistancia();
  if (d < 0) return nivelAnterior;
  return constrain((DIST_VACIO - d) * 100.0 / (DIST_VACIO - DIST_LLENO), 0, 100);
}

void publicar(const char* feed, String valor) {
  String topic = String(AIO_USER) + "/feeds/" + feed;    // formato de Adafruit IO
  mqtt.publish(topic.c_str(), valor.c_str());
}

void conectarMQTT() {
  while (!mqtt.connected()) {
    if (!mqtt.connect("esp32-tanque", AIO_USER, AIO_KEY)) delay(2000);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_LLENO, OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  mqtt.setServer("io.adafruit.com", 1883);
  nivelAnterior = 0;
  nivelAnterior = leerNivel();
}

void loop() {
  if (!mqtt.connected()) conectarMQTT();
  mqtt.loop();

  float nivel = leerNivel();
  digitalWrite(LED_LLENO, nivel >= 95);                  // LED encendido = tanque lleno

  if (millis() - ultimoEnvio >= 10000) {                 // límite gratis de Adafruit IO: 30 msg/min
    float tasa = (nivel - nivelAnterior) / 10.0;         // % por segundo
    String estado;
    if (nivel >= 95) estado = "LLENO";
    else if (tasa > 0.05) estado = "LLENANDO (faltan ~" + String((100 - nivel) / tasa / 60.0, 1) + " min)";
    else if (tasa < -0.05) estado = "VACIANDO";
    else estado = nivel < 10 ? "VACIO" : "ESTABLE";

    publicar("nivel-tanque", String(nivel, 1));
    publicar("estado-tanque", estado);
    Serial.println(String(nivel, 1) + "% - " + estado);
    nivelAnterior = nivel;
    ultimoEnvio = millis();
  }
  delay(200);
}

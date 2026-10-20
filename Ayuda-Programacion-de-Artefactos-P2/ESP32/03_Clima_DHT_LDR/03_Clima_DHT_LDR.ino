// Ejercicio 3: Estación de clima (DHT + LDR) publicada por MQTT en Adafruit IO  |  Placa: ESP32 (DevKit WiFi + Bluetooth)
// Feeds a crear: temperatura, humedad, luz, clima
// Librerías: "DHT sensor library" (Adafruit) y PubSubClient
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

const char* WIFI_SSID = "TU_WIFI";
const char* WIFI_PASS = "TU_CLAVE";
const char* AIO_USER  = "TU_USUARIO_AIO";
const char* AIO_KEY   = "TU_AIO_KEY";

#define DHT_PIN  4
#define DHT_TIPO DHT11            // cambia a DHT22 si tu sensor es azul/blanco de 4 pines
#define LDR_PIN  34          // GPIO34 = ADC1, solo entrada (funciona con WiFi)
// Circuito LDR: 3.3V -> LDR -> pin ADC -> resistencia 10k -> GND (más luz = valor más alto)

DHT dht(DHT_PIN, DHT_TIPO);
WiFiClient wifi;
PubSubClient mqtt(wifi);

void publicar(const char* feed, String valor) {
  String topic = String(AIO_USER) + "/feeds/" + feed;
  mqtt.publish(topic.c_str(), valor.c_str());
}

void conectarMQTT() {
  while (!mqtt.connected()) {
    if (!mqtt.connect("esp32-clima", AIO_USER, AIO_KEY)) delay(2000);
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  mqtt.setServer("io.adafruit.com", 1883);
}

void loop() {
  if (!mqtt.connected()) conectarMQTT();
  mqtt.loop();

  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (isnan(t) || isnan(h)) { delay(2000); return; }       // lectura fallida
  int luz = map(analogRead(LDR_PIN), 0, 4095, 0, 100);     // ADC 12 bits -> % de luz

  // Lógica del clima (ajusta los umbrales según tu entorno)
  String clima;
  if (h >= 80 && luz < 40)       clima = "LLUVIA PROBABLE";
  else if (luz < 40 && h >= 60)  clima = "NUBLADO";
  else if (luz >= 70 && t >= 30) clima = "SOLEADO Y CALUROSO";
  else if (luz >= 70)            clima = "SOLEADO";
  else                           clima = "TEMPLADO";

  publicar("temperatura", String(t, 1));
  publicar("humedad", String(h, 1));
  publicar("luz", String(luz));
  publicar("clima", clima);
  Serial.printf("T=%.1fC H=%.1f%% Luz=%d%% -> %s\n", t, h, luz, clima.c_str());
  delay(15000);                                            // 4 feeds cada 15 s = 16 msg/min
}

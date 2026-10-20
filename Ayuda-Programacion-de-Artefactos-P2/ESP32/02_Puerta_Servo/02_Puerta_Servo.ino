// Ejercicio 2: Puerta con servomotor. LED rojo = cerrada, LED verde = abierta  |  Placa: ESP32 (DevKit WiFi + Bluetooth)
// Librería: ESP32Servo. Botón entre el pin y GND (INPUT_PULLUP, sin resistencia).
#include <ESP32Servo.h>

#define SERVO_PIN 18
#define LED_ROJO  25
#define LED_VERDE 26
#define BTN_PIN   14

Servo servo;
bool abierta = false;

void moverPuerta(bool abrir) {
  servo.write(abrir ? 90 : 0);        // 90° abierta, 0° cerrada
  digitalWrite(LED_VERDE, abrir);     // verde solo si está abierta
  digitalWrite(LED_ROJO, !abrir);     // rojo solo si está cerrada
  abierta = abrir;
  Serial.println(abrir ? "Puerta ABIERTA" : "Puerta CERRADA");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
  servo.setPeriodHertz(50);           // servos estándar trabajan a 50 Hz
  servo.attach(SERVO_PIN, 500, 2400); // rango del pulso en microsegundos
  moverPuerta(false);                 // inicia cerrada
}

void loop() {
  if (digitalRead(BTN_PIN) == LOW) {  // botón presionado
    moverPuerta(!abierta);            // alterna estado
    delay(300);                       // antirrebote
    while (digitalRead(BTN_PIN) == LOW);
  }
}

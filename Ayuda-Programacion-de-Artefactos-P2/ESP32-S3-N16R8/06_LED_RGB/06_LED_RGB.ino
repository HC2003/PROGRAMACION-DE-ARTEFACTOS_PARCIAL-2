// Ejercicio 6: LED RGB con PWM (colores fijos + arcoíris)  |  Placa: ESP32-S3 N16R8
// Requiere el core ESP32 de Arduino 3.x (usa ledcAttach). Una resistencia de 220 ohm por color.
#define PIN_R 4
#define PIN_G 5
#define PIN_B 6
const bool ANODO_COMUN = false;   // true si el pin largo va a 3.3V (la lógica se invierte)

void color(uint8_t r, uint8_t g, uint8_t b) {
  if (ANODO_COMUN) { r = 255 - r; g = 255 - g; b = 255 - b; }
  ledcWrite(PIN_R, r);            // duty de 0 a 255 (8 bits)
  ledcWrite(PIN_G, g);
  ledcWrite(PIN_B, b);
}

// Rueda de color: p de 0 a 255 recorre rojo -> verde -> azul -> rojo
void rueda(uint8_t p) {
  if (p < 85)       color(255 - p * 3, p * 3, 0);
  else if (p < 170) { p -= 85;  color(0, 255 - p * 3, p * 3); }
  else              { p -= 170; color(p * 3, 0, 255 - p * 3); }
}

void setup() {
  ledcAttach(PIN_R, 5000, 8);     // pin, frecuencia 5 kHz, resolución 8 bits
  ledcAttach(PIN_G, 5000, 8);
  ledcAttach(PIN_B, 5000, 8);
}

void loop() {
  const uint8_t C[7][3] = {{255,0,0},{0,255,0},{0,0,255},{255,255,0},{0,255,255},{255,0,255},{255,255,255}};
  for (int i = 0; i < 7; i++) { color(C[i][0], C[i][1], C[i][2]); delay(700); }  // colores fijos
  for (int p = 0; p < 256; p++) { rueda(p); delay(15); }                        // transición suave
}

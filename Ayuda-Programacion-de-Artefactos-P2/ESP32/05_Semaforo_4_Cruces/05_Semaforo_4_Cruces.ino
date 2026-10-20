// Ejercicio 5: Semáforo de 4 vías (Norte, Sur, Este, Oeste)  |  Placa: ESP32 (DevKit WiFi + Bluetooth)
// Norte-Sur y Este-Oeste se alternan: nunca hay verde en ambos ejes a la vez.
// Cada LED lleva su resistencia de 220 ohm.

enum Color { ROJO = 0, AMARILLO = 1, VERDE = 2 };

// Filas: Norte, Sur, Este, Oeste | Columnas: rojo, amarillo, verde
const int PIN[4][3] = {{13,14,27},{26,25,33},{32,16,17},{18,19,21}};

void ponerSemaforo(int s, Color c) {
  for (int i = 0; i < 3; i++) digitalWrite(PIN[s][i], i == c);  // enciende solo el color pedido
}

// Norte y Sur cambian juntos; Este y Oeste cambian juntos
void estado(Color ns, Color eo) {
  ponerSemaforo(0, ns); ponerSemaforo(1, ns);
  ponerSemaforo(2, eo); ponerSemaforo(3, eo);
}

void setup() {
  for (int s = 0; s < 4; s++)
    for (int c = 0; c < 3; c++) pinMode(PIN[s][c], OUTPUT);
  estado(ROJO, ROJO);
}

void loop() {
  estado(VERDE, ROJO);    delay(5000);   // pasa Norte-Sur
  estado(AMARILLO, ROJO); delay(2000);
  estado(ROJO, ROJO);     delay(1000);   // despeje: todos en rojo
  estado(ROJO, VERDE);    delay(5000);   // pasa Este-Oeste
  estado(ROJO, AMARILLO); delay(2000);
  estado(ROJO, ROJO);     delay(1000);
}

# Ayuda Programación de Artefactos P2

Ejercicios y guía de repaso para el **Parcial 2** de Programación de Artefactos (Arduino IDE).
Cada ejercicio existe en dos versiones con la misma lógica y distintos pines:

| Carpeta | Placa |
|---|---|
| `ESP32-S3-N16R8/` | ESP32-S3 N16R8 (placa principal) |
| `ESP32/` | ESP32 normal (WiFi + Bluetooth) |

## Ejercicios (6 por placa = 12 sketches)

| # | Ejercicio | Qué practica |
|---|---|---|
| 01 | Tanque con ultrasonido | HC-SR04, nivel en %, estado "llenando/lleno", Adafruit IO por MQTT, LED al llenarse |
| 02 | Puerta con servo | Servo, botón, LED rojo (cerrada) / verde (abierta) |
| 03 | Clima DHT + LDR | Temperatura, humedad, luz, lógica de lluvia, MQTT |
| 04 | Lector RFID RC522 | SPI, UIDs autorizados, LED verde/rojo |
| 05 | Semáforo de 4 vías | 12 LEDs, ejes Norte-Sur / Este-Oeste sin choque |
| 06 | LED RGB | PWM con `ledc`, colores fijos y arcoíris |

## Documentación
Lee **[docs/DOCUMENTACION.md](docs/DOCUMENTACION.md)**: ánodo/cátodo, resistencias, sensores, MQTT, configuración de Arduino IDE, mapas de pines de ambas placas y errores comunes.

## Antes de subir
- Core **esp32 (Espressif) 3.x** en el Gestor de Placas.
- Librerías: `PubSubClient`, `DHT sensor library`, `Adafruit Unified Sensor`, `ESP32Servo`, `MFRC522`.
- Cambia `WIFI_SSID`, `WIFI_PASS`, `AIO_USER` y `AIO_KEY` en los ejercicios 01 y 03, y crea los feeds en Adafruit IO.
- Verifica los pines de cada sketch con tu cableado.

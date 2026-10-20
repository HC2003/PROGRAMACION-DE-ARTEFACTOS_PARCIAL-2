# Documentación de repaso: Programación de Artefactos (Parcial 2)

## 1. LEDs: ánodo, cátodo y resistencia

- **Ánodo (+)**: pata **larga**. Va hacia el pin (o al voltaje).
- **Cátodo (−)**: pata **corta**, lado plano del encapsulado. Va a **GND**.
- **¿Se puede conectar un LED sin resistencia?** No. El LED no limita su corriente y se quema (o daña el pin de la placa).
- Fórmula: **R = (Vpin − Vled) / I**. Con 3.3V y ~10 mA:

| Color | Vled aprox. | R calculada | Valor práctico |
|---|---|---|---|
| Rojo / Amarillo | 1.8 – 2.1 V | ~120 – 150 Ω | **220 Ω** |
| Verde | 2.0 – 3.0 V | ~30 – 130 Ω | **220 Ω** |
| Azul / Blanco | 3.0 – 3.2 V | ~10 – 30 Ω | **100 – 220 Ω** (se verá menos brillante) |

- Regla segura: **220 Ω** funciona para casi todo (con 330 Ω o 1 kΩ se ve más tenue).
- Corriente recomendada por pin del ESP32: **~10 mA** (máximo absoluto 40 mA; no abusar).
- Un LED, una resistencia. Con varios LEDs en paralelo, **una resistencia por LED**.
- La resistencia puede ir antes o después del LED; el efecto es el mismo.

### LED RGB
- 4 patas: R, G, B y una común. La pata más larga es la **común**.
- **Cátodo común**: común a GND. `255` = encendido.
- **Ánodo común**: común a 3.3V. Lógica invertida (`0` = encendido).
- Una resistencia (220 Ω) **por cada color**, nunca una sola para los tres.
- El LED RGB integrado de muchas placas S3 (GPIO 48) es **WS2812 (direccionable)**: no se maneja con `ledcWrite`, sino con `neopixelWrite()` o la librería Adafruit NeoPixel.

## 2. Sensores: ¿llevan resistencia?

| Componente | ¿Resistencia? | Detalle |
|---|---|---|
| **HC-SR04** | No, pero sí un **divisor** en ECHO | Se alimenta con 5V y ECHO sale a 5V. Divisor: ECHO → 1 kΩ → pin ESP32 → 2 kΩ → GND (da ~3.3V). |
| **DHT11 / DHT22** | **Pull-up 4.7k–10k** entre DATA y VCC | Los módulos de 3 pines ya la traen; el sensor suelto de 4 pines no. Alimentación 3.3V. |
| **LDR (fotorresistencia)** | **Sí**, divisor con **10 kΩ** | 3.3V → LDR → pin ADC → 10 kΩ → GND. Sin la fija no hay variación de voltaje legible. |
| **Servomotor** | No | Señal al pin, **5V externo** si es posible, **GND común** con la placa. |
| **Pulsador** | No (usar `INPUT_PULLUP`) | Un lado al pin, otro a GND. Lee `LOW` al presionar. Externo: pull-up/down de 10 kΩ. |
| **RFID RC522** | No | Alimentar **solo a 3.3V** (5V lo daña). Usa SPI. |
| **Buzzer activo** | No (opcional 100–220 Ω) | `digitalWrite` lo activa. |
| **Relé (módulo)** | No | Alimentación 5V, señal 3.3V suele bastar. |

## 3. Voltajes y electricidad básica

- ESP32 y ESP32-S3 trabajan a **3.3V lógicos**. Los pines **NO toleran 5V**.
- **Ley de Ohm**: V = I · R. **Potencia**: P = V · I.
- **Divisor de voltaje**: Vout = Vin · R2 / (R1 + R2).
- **GND siempre en común** entre placa, sensores y fuentes externas.
- Nunca alimentar motores/servos desde el pin 3.3V de la placa.
- Pines con 3V3 y 5V (VIN): el 5V sale del USB.

## 4. ADC, PWM y comunicación

- **ADC**: 12 bits, `analogRead()` devuelve **0 – 4095** (0 – 3.3V).
- **PWM**: en core 3.x se usa `ledcAttach(pin, frecuencia, bits)` y `ledcWrite(pin, duty)`. 8 bits = 0–255.
- **Servo**: PWM de 50 Hz, pulso 500–2400 µs → 0°–180°. Librería **ESP32Servo**.
- **I2C**: 2 hilos (SDA, SCL). **SPI**: 4 hilos (SCK, MOSI, MISO, SS). **UART**: TX/RX.
- **`pulseIn`**: mide el tiempo de un pulso en µs. Distancia (cm) = tiempo · 0.0343 / 2.

## 5. MQTT y Adafruit IO

- **MQTT**: protocolo ligero de **publicar / suscribirse** sobre TCP.
  - **Broker**: servidor intermediario (aquí `io.adafruit.com`).
  - **Topic**: canal de mensajes. **Publish**: enviar. **Subscribe**: recibir.
  - **QoS**: 0 (una vez), 1 (al menos una), 2 (exactamente una).
- **Adafruit IO**: servidor `io.adafruit.com`, puerto **1883** (8883 con TLS).
  - Usuario: tu *username*. Contraseña: tu **AIO Key**.
  - Topic de un feed: `USUARIO/feeds/KEY_DEL_FEED` (la *key* en minúsculas y con guiones).
  - Plan gratuito: **30 mensajes por minuto**; si se pasa, bloquea temporalmente.
- Los **feeds** deben existir en el dashboard antes de publicar, y el nombre debe coincidir.
- Para recibir datos (por ejemplo un botón del dashboard) se usa `mqtt.subscribe(topic)` y una función `callback`.

## 6. Configuración en Arduino IDE

### ESP32-S3 N16R8
| Opción | Valor |
|---|---|
| Board | **ESP32S3 Dev Module** |
| Flash Size | **16MB (128Mb)** |
| PSRAM | **OPI PSRAM** (octal; si se deja otro valor la placa se reinicia en bucle) |
| Partition Scheme | 16MB Flash (3MB APP/9.9MB FATFS) |
| USB CDC On Boot | **Enabled** si usas el puerto USB nativo para el monitor serial |
| Puerto | Usa el puerto USB-C marcado **UART** (o COM) para subir sin complicaciones |

### ESP32 normal
| Opción | Valor |
|---|---|
| Board | **ESP32 Dev Module** |
| Upload Speed | 115200 si falla la subida |
| Puerto | El que aparezca al conectar (requiere driver CP210x o CH340) |

### Librerías (Gestor de librerías)
`PubSubClient` (Nick O'Leary), `DHT sensor library` + `Adafruit Unified Sensor`, `ESP32Servo`, `MFRC522` (miguelbalboa). Core: **esp32 de Espressif 3.x**.

---

## 7. Mapa de pines: ESP32-S3 N16R8

> Varía un poco entre fabricantes; confirma con la serigrafía de tu placa.

| GPIO | Uso / función | ¿Usar? |
|---|---|---|
| 0 | BOOT (strapping) | Evitar |
| 1 – 10 | Libres. **ADC1** (funciona con WiFi), también táctiles | **Sí** (ideal para sensores analógicos) |
| 3 | Strapping | Evitar |
| 8 / 9 | **I2C por defecto**: SDA = 8, SCL = 9 | Sí |
| 10, 11, 12, 13 | **SPI por defecto**: SS = 10, MOSI = 11, SCK = 12, MISO = 13 | Sí |
| 11 – 18 | **ADC2** (no usar `analogRead` si WiFi está activo) | Sí, solo digital |
| 19, 20 | USB nativo (D−, D+) | Evitar si usas USB |
| 21, 38 – 42, 47 | Libres | Sí |
| 26 – 32 | Flash SPI interna | **NO** |
| 33 – 37 | **PSRAM octal** (N16R8) | **NO** |
| 43 / 44 | UART0 TX / RX (monitor serial) | Evitar |
| 45, 46 | Strapping | Evitar |
| 48 | LED RGB integrado (WS2812) en muchas placas (algunas usan 38) | Reservado |

- Pines de alimentación: **3V3, 5V (VIN), GND**.
- Dos puertos USB-C: **UART** (convertidor serial) y **USB** (nativo).
- Es una placa con **WiFi y Bluetooth 5 (BLE)**; no tiene Bluetooth clásico.

## 8. Mapa de pines: ESP32 normal (DevKit)

| GPIO | Uso / función | ¿Usar? |
|---|---|---|
| 0 | BOOT (strapping), ADC2 | Evitar |
| 1 / 3 | UART0 TX / RX (USB serial) | Evitar |
| 2 | LED integrado (strapping), ADC2 | Con cuidado |
| 4, 13, 14, 16, 17, 25, 26, 27, 32, 33 | Libres para entrada/salida | **Sí** |
| 5 | **SS** de SPI (VSPI), strapping | Con cuidado |
| 6 – 11 | Flash SPI interna | **NO** |
| 12 | Strapping (si está en HIGH al arrancar, no inicia) | Evitar |
| 15 | Strapping (mensajes al arrancar) | Con cuidado |
| 18 / 19 / 23 | **SPI (VSPI)**: SCK = 18, MISO = 19, MOSI = 23 | Sí |
| 21 / 22 | **I2C**: SDA = 21, SCL = 22 | Sí |
| 25, 26 | **DAC** (salida analógica 8 bits) | Sí |
| 32 – 39 | **ADC1** (funciona con WiFi) | **Sí** (sensores analógicos) |
| 34, 35, 36 (VP), 39 (VN) | **Solo entrada**, sin pull-up interno | Solo leer |
| 0, 2, 4, 12 – 15, 25 – 27 | **ADC2**: no funciona con WiFi activo | Solo digital |

- Pines de alimentación: **3V3, 5V (VIN), GND**.
- Tiene **WiFi y Bluetooth clásico + BLE**.

---

## 9. Errores comunes y soluciones

### Placa y subida de código
| Problema | Causa / solución |
|---|---|
| `Failed to connect to ESP32... Wrong boot mode` | Mantén presionado **BOOT**, pulsa y suelta **EN/RST**, suelta BOOT cuando inicie la subida. En ESP32 normal prueba un capacitor de 10 µF entre EN y GND. |
| No aparece el puerto COM | Cable USB **solo de carga** (usa uno de datos), o falta el driver **CP210x / CH340**. En S3, prueba el otro puerto USB-C. |
| El S3 se reinicia en bucle (`rst:0x3 ... PSRAM`) | PSRAM mal configurada. Pon **OPI PSRAM** y Flash **16MB**. |
| El monitor serial queda vacío (S3) | Activa **USB CDC On Boot** si usas el puerto USB nativo, o cambia al puerto UART. |
| Texto basura en el monitor | Velocidad distinta: `Serial.begin(115200)` y el monitor en **115200**. |
| `Brownout detector was triggered` | Poca corriente: cable malo, puerto USB débil, servo o WiFi consumiendo mucho. Usa fuente de 5V externa para el servo. |
| `ledcSetup / ledcAttachPin not declared` | Usas core 3.x con código del 2.x. Usa `ledcAttach(pin, freq, bits)`. |

### WiFi y MQTT
| Problema | Causa / solución |
|---|---|
| No conecta a WiFi | El ESP32 solo usa **2.4 GHz** (no 5 GHz). Revisa SSID y clave (distingue mayúsculas). Evita redes con portal cautivo. |
| MQTT no conecta (`rc=-2` / `rc=5`) | `-2`: sin red o servidor mal escrito. `5`/`4`: usuario o **AIO Key** incorrectos. |
| Conecta pero no llegan datos al dashboard | El **feed key** no coincide o no existe. Topic exacto: `usuario/feeds/key-del-feed`. |
| Adafruit IO deja de recibir | Superaste los **30 msg/min**. Aumenta el intervalo de envío. |
| Se desconecta de MQTT cada rato | Falta `mqtt.loop()` en el `loop()`, o hay `delay()` muy largos. Reconecta si `!mqtt.connected()`. |
| `analogRead` da 0 o valores raros con WiFi (ESP32) | Estás usando un pin **ADC2**. Pasa el sensor a un pin **ADC1** (32–39 en ESP32; GPIO 1–10 en S3). |

### Sensores y componentes
| Componente | Problema | Solución |
|---|---|---|
| HC-SR04 | Siempre da 0 o -1 | Revisa 5V en VCC, TRIG/ECHO no invertidos y GND común. Objeto a más de 2 cm y menos de 4 m. |
| HC-SR04 | Lecturas saltan | Superficie inclinada o blanda. Promedia varias lecturas. |
| DHT | Devuelve `nan` | Pull-up faltante, `DHTTYPE` incorrecto (DHT11 vs DHT22) o leer más rápido de 1 s (DHT11) / 2 s (DHT22). |
| LDR | Valor no cambia | Falta la resistencia de 10 kΩ en divisor, o está en pin ADC2 con WiFi. |
| Servo | Tiembla o se reinicia la placa | Alimentación insuficiente. Usa 5V externo con GND común y un capacitor de 100–470 µF. |
| Servo | No se mueve | Pin con PWM no soportado o falta `ESP32Servo` (la librería `Servo` normal no funciona). |
| RC522 | `Communication failure` / no lee | Alimentado a 5V o cables SPI cruzados. Debe ir a **3.3V**. Revisa SS, RST, SCK, MOSI, MISO. |
| LED | No enciende | Está al revés (ánodo/cátodo), sin GND, o pin fuera de uso. |
| LED RGB | Colores invertidos | Es de **ánodo común**: pon `ANODO_COMUN = true`. |
| Pulsador | Lee solo HIGH o rebota | Usa `INPUT_PULLUP` y detecta `LOW`. Agrega antirrebote. |
| Pin GPIO | La placa no arranca con algo conectado | Hay un componente en un pin **strapping** (0, 2, 5, 12, 15 en ESP32; 0, 3, 45, 46 en S3). Desconéctalo al subir. |

### Mantras antes del parcial
1. ¿**GND en común**? 2. ¿Pin **permitido** (no flash/PSRAM/strapping)? 3. ¿**3.3V** y no 5V en sensores sensibles? 4. ¿Resistencia en el **LED**? 5. ¿Placa y **PSRAM** bien elegidas? 6. ¿Librería instalada?

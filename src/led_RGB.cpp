#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <Adafruit_NeoPixel.h>

// programa completamente funcional para tira de led RGB WS2812B usando FreeRTOS
// crea dos tareas: una para animar un cometa y otra para reportar por serial el estado cada segundo

// --- Configuración de la Tira de LED ---
const int LED_PIN = 4;        // Pin GPIO al que está conectada la línea de datos (puedes ajustarlo)
const int NUM_LEDS = 60;      // Número total de LEDs en tu tira
const int BRIGHTNESS = 50;    // Brillo (0-255)

// --- Objeto NeoPixel ---
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800); // si es el WS2818B usa NEO_KHZ400 !!!

// --- TAREA 1: Animación del Cometa ---
void ledAnimationTask(void *pvParameters) {
  uint16_t head = 0; // Posición inicial del cometa
  uint32_t cometColor = strip.Color(0, 0, 255); // Color Azul

  for (;;) {
    // 1. Borrar todos los LEDs
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0)); // Apaga todos
    }

    // 2. Dibujar el cometa
    // La "cabeza" del cometa (azul brillante)
    strip.setPixelColor(head, cometColor);

    // La "cola" del cometa (degradado)
    for (int i = 1; i < 5; i++) {
      int tailPos = (head - i + NUM_LEDS) % NUM_LEDS;
      // Reduce el brillo para crear un degradado en la cola
      uint8_t dim = map(i, 1, 4, BRIGHTNESS, 0); 
      strip.setPixelColor(tailPos, dim, dim, dim); // La cola es blanca (RGB iguales)
    }

    // 3. Mostrar el frame
    strip.show();

    // 4. Mover la cabeza y esperar
    head = (head + 1) % NUM_LEDS; // Avanza al siguiente LED
    
    // Pausa de la tarea para controlar la velocidad de la animación
    vTaskDelay(pdMS_TO_TICKS(30)); // Velocidad rápida (30ms por frame)
  }
}

// --- TAREA 2: Reporte Serial ---
void serialReportTask(void *pvParameters) {
  Serial.println(">>> FreeRTOS: Animación y Monitoreo activos <<<");
  uint32_t cycles = 0;
  for (;;) {
    Serial.printf("Ciclo de vida: %lu segundos. Animación en curso.\n", cycles);
    cycles++;
    vTaskDelay(pdMS_TO_TICKS(1000)); // Reporta cada segundo
  }
}

// ----------------------------------------------------
// SETUP y LOOP
// ----------------------------------------------------
void rgb_setup() {
  Serial.begin(115200);
  delay(100);

  // Inicializar la tira de LEDs
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show(); // Apagar todos los LEDs al inicio

  // Crear la Tarea de Animación (Prioridad 1)
  xTaskCreate(
    ledAnimationTask,
    "Comet_Anim",
    2048,
    NULL,
    1,
    NULL
  );

  // Crear la Tarea de Reporte Serial (Prioridad 1)
  xTaskCreate(
    serialReportTask,
    "Serial_Mon",
    2048,
    NULL,
    1,
    NULL
  );
}

void rgb_loop() {
  // El loop principal simplemente duerme, todo lo importante se hace en las tareas.
  vTaskDelay(pdMS_TO_TICKS(5000));
}
// ...existing code...
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

const int LED_PIN = LED_BUILTIN;

// Pines específicos del LED RGB en el ESP32-C3
const int LED_R = 3;
const int LED_G = 4;
const int LED_B = 5;

// Nueva función auxiliar para controlar el color blanco
void setWhiteLEDState(bool state)
{
  // Si 'state' es true (encender), el nivel es HIGH. Si es false (apagar), es LOW.
  int level = state ? HIGH : LOW;

  // Controla los tres pines con una sola llamada a esta función
  digitalWrite(LED_R, level);
  digitalWrite(LED_G, level);
  digitalWrite(LED_B, level);
}

void blinkTask(void *pvParameters)
{

  //  pinMode(LED_PIN, OUTPUT);

  
  //  Configurar los 3 pines como salida SOLO UNA VEZ
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  for (;;)
  {
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(500));
    digitalWrite(LED_PIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void serialTask(void *pvParameters)
{
  Serial.println("Serial task started");
  for (;;)
  {
    Serial.printf("Uptime: %lu ms\n", millis());
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void free_setup()
{
  Serial.begin(115200);
  delay(100);
  // Crear tareas FreeRTOS
  xTaskCreate(blinkTask, "Blink", 2048, NULL, 1, NULL);
  xTaskCreate(serialTask, "Serial", 2048, NULL, 1, NULL);
}

void free_loop()
{
  // El loop queda prácticamente libre; usar vTaskDelay para no consumir CPU
  vTaskDelay(pdMS_TO_TICKS(1000));
}
// ...existing code...
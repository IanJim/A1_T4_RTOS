#include <Arduino.h>


// Prototipos de las tareas
void tarea1(void *pvParameters);
void tarea2(void *pvParameters);

void segundo_setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("Iniciando FreeRTOS...");
  
  // Crear tarea 1
  xTaskCreate(
    tarea1,        // Función de la tarea
    "Tarea1",      // Nombre de la tarea
    2048,          // Tamaño del stack
    NULL,          // Parámetros
    1,             // Prioridad
    NULL           // Handle de la tarea
  );
  
  // Crear tarea 2
  xTaskCreate(
    tarea2,
    "Tarea2",
    2048,
    NULL,
    1,
    NULL
  );
}

void segundo_loop() {
  // El loop no se usa con FreeRTOS
  delay(10000);
}

// Tarea 1: Parpadea LED cada 1 segundo
void tarea1(void *pvParameters) {
  while (1) {
    Serial.println("Tarea 1 ejecutándose");
    digitalWrite(LED_BUILTIN, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(LED_BUILTIN, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// Tarea 2: Imprime cada 2 segundos
void tarea2(void *pvParameters) {
  while (1) {
    Serial.println("Tarea 2 ejecutándose");
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

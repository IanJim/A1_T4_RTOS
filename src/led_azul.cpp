#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Definimos el pin del LED Azul (GPIO 5 en el ESP32-C3 DevKitM-1)
const int LED_AZUL_PIN = 5;

// Variable global que ambas tareas pueden leer
volatile bool led_esta_encendido = false;

// ----------------------------------------------------
// TAREA 1: Controla el LED (Parpadeo)
// ----------------------------------------------------
void controlLedTask(void *pvParameters) {
    pinMode(LED_AZUL_PIN, OUTPUT);

    for (;;) {
        // 1. Encender el LED
        digitalWrite(LED_AZUL_PIN, HIGH); // originalmente estaba en HIGH
        led_esta_encendido = true; // Actualiza la variable global
        vTaskDelay(pdMS_TO_TICKS(1200)); // Espera 200 ms

        // 2. Apagar el LED
        digitalWrite(LED_AZUL_PIN, LOW);
        led_esta_encendido = false; // Actualiza la variable global
        vTaskDelay(pdMS_TO_TICKS(100)); // Espera 800 ms
    }
}

// ----------------------------------------------------
// TAREA 2: Reporta el estado (Serial)
// ----------------------------------------------------
void serialReporteTask(void *pvParameters) {
    Serial.println(">>> Monitor de Tareas de FreeRTOS iniciado <<<");

    for (;;) {
        // La tarea 2 lee la variable que la Tarea 1 modifica
        if (led_esta_encendido) {
            Serial.println("LED ESTADO: Encendido (HIGH)");
        } else {
            Serial.println("LED ESTADO: Apagado (LOW)");
        }
        
        // Espera exactamente 1 segundo
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// ----------------------------------------------------
// SETUP y LOOP
// ----------------------------------------------------
void ledAzul_setup() {
    Serial.begin(115200);
    delay(100);

    // Crear la Tarea de Control de LED (Prioridad 1)
    xTaskCreate(
        controlLedTask,
        "LED_Control",
        2048,
        NULL,
        1,
        NULL
    );

    // Crear la Tarea de Reporte Serial (Prioridad 1)
    xTaskCreate(
        serialReporteTask,
        "Serial_Reporte",
        2048,
        NULL,
        1,
        NULL
    );
}

void ledAzul_loop() {
    // El loop principal simplemente duerme para no consumir CPU
    vTaskDelay(pdMS_TO_TICKS(5000));
}
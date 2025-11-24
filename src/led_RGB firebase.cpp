// src/Practica.cpp
#include "practica.h"
#include <FirebaseESP32.h>

// Variable global que rastrea el estado actual de la animación
AnimationMode currentMode = MODE_RGB_DEFAULT;
// Variable global para rastrear el comando anterior (para el toggle)
String lastCommandReceived = ""; 

// --- Configuración de LEDs ---
#define LED_COUNT 60 // Ajusta al número de LEDs de tu tira
#define LED_PIN 4    // Ajusta al pin de tu ESP32
// (Aquí irían las variables y la inicialización de la librería FastLED/WS2818B)
// CRGB leds[LED_COUNT]; // Ejemplo si usas FastLED

extern FirebaseData firebaseData; // Declara la variable para el otro archivo

// --- Implementación de Funciones ---

void animacion_Setup() {
    // (Inicialización de la librería de LEDs aquí)
    // FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT); // Ejemplo
    // FastLED.show();
    Serial.println("Lógica de animación inicializada.");
}

// Función principal del bucle que ejecuta la animación basada en 'currentMode'
void animacion_Loop() {
    switch (currentMode) {
        case MODE_RGB_DEFAULT:
            drawRGBDefault(); 
            break;
        case MODE_B_REVERSA:
            drawBReversa();
            break;
        case MODE_I_INTERMITENTES:
            drawIIntermitentes();
            break;
        case MODE_L_IZQUIERDA:
            drawLIzquierda();
            break;
        case MODE_R_DERECHA:
            drawRDerecha();
            break;
        case MODE_S_ALTO:
            drawSAlto();
            break;
    }
    // FastLED.show(); // Mostrar los cambios si usas FastLED
}

// Función que lee el comando de la nube y actualiza el estado (toggle)
void firebase_check_command() {
    // La ruta debe coincidir con la de main.cpp: /control/comando
    if (Firebase.getString(firebaseData, "/control/comando")) {
        
        String receivedCommand = firebaseData.stringData();
        
        // 1. Quitar espacios y convertir a mayúsculas para evitar errores
        receivedCommand.trim();
        receivedCommand.toUpperCase(); 

        // 2. Lógica de TOGGLE (Desactivar)
        if (receivedCommand == lastCommandReceived) {
            Serial.print("Comando recibido: ");
            Serial.print(receivedCommand);
            Serial.println(" -> Desactivando (TOGGLE)");
            currentMode = MODE_RGB_DEFAULT; // Vuelve al estado por defecto
            lastCommandReceived = "";       // Limpia el comando anterior
            return;
        }

        // 3. Lógica de Activación (Selección de Modo)
        if (receivedCommand == "B") {
            currentMode = MODE_B_REVERSA;
        } else if (receivedCommand == "I") {
            currentMode = MODE_I_INTERMITENTES;
        } else if (receivedCommand == "L") {
            currentMode = MODE_L_IZQUIERDA;
        } else if (receivedCommand == "R") {
            currentMode = MODE_R_DERECHA;
        } else if (receivedCommand == "S") {
            currentMode = MODE_S_ALTO;
        } else {
            // Si recibe algo que no es un comando válido, vuelve al estado por defecto
            currentMode = MODE_RGB_DEFAULT;
        }

        // 4. Actualizar el comando anterior
        if (currentMode != MODE_RGB_DEFAULT) {
            lastCommandReceived = receivedCommand;
        } else {
            lastCommandReceived = "";
        }
        
        Serial.print("Modo cambiado a: ");
        Serial.println(receivedCommand);

    } else {
        // Manejo de error de Firebase
        // Serial.printf("Error al leer Firebase: %s\n", firebaseData.errorReason().c_str());
    }
}

// --- Implementación de Animaciones (Solo Estructura) ---

void drawRGBDefault() {
    // Aquí va la lógica de tu animación RGB anterior (el cometa).
    // Serial.println("Ejecutando Cometa RGB...");
    delay(10); 
}

void drawBReversa() {
    // Parpadear primeros y últimos 15 LEDs en blanco.
    // Serial.println("Ejecutando B Reversa...");
    delay(50); 
}

void drawIIntermitentes() {
    // Parpadear primeros y últimos 15 LEDs en ambar.
    // Serial.println("Ejecutando I Intermitentes...");
    delay(50); 
}

void drawLIzquierda() {
    // Animación direccional izquierda (usando dos LEDs).
    // Serial.println("Ejecutando L Izquierda...");
    delay(20); 
}

void drawRDerecha() {
    // Animación direccional derecha (usando dos LEDs).
    // Serial.println("Ejecutando R Derecha...");
    delay(20); 
}

void drawSAlto() {
    // Encender primeros y últimos 15 LEDs en rojo fijo.
    // Serial.println("Ejecutando S Alto...");
    delay(1000); 
}
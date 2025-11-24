// src/Practica.cpp
#include "practica.h"
#include <FirebaseESP32.h>
#include <FastLED.h> // <-- ¡Asegúrate de que esta línea esté aquí!

// Variable global que rastrea el estado actual de la animación
AnimationMode currentMode = MODE_RGB_DEFAULT; // MODE_RGB_DEFAULT
// Variable global para rastrear el comando anterior (para el toggle)
String lastCommandReceived = ""; 

// --- Configuración de LEDs ---
#define LED_COUNT 60 // Ajusta al número de LEDs de tu tira
#define LED_PIN 4    // Ajusta al pin de tu ESP32
// (Aquí irían las variables y la inicialización de la librería FastLED/WS2818B)
 CRGB leds[LED_COUNT]; // Ejemplo si usas FastLED

 // Para animaciones direccionales (L e R)
static int current_pos = 0;
static const int ANIMATION_DELAY = 50;

// Para animaciones de parpadeo (B e I)
static unsigned long previous_blink_millis = 0;
static bool blink_state = false;
static const int BLINK_INTERVAL = 250;


extern FirebaseData firebaseData; // Declara la variable para el otro archivo

// --- Implementación de Funciones ---

void animacion_Setup() {
    // (Inicialización de la librería de LEDs aquí)
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT); // Ejemplo
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

/*void resetAnimationState() {
    current_pos = 0;
    previous_blink_millis = 0;
    blink_state = false;
    FastLED.clear();
    FastLED.show();
}
    */


// Función que lee el comando de la nube y actualiza el estado (toggle)
void firebase_check_command() {
    // La ruta debe coincidir con la de main.cpp: /control/comando
    if (Firebase.getString(firebaseData, "control/comando")) {
        
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

        //resetAnimationState(); // Reinicia el estado de la animación

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
    static uint8_t hue = 0;
    fill_rainbow(leds, LED_COUNT, hue++, 5);
    FastLED.show();
    delay(10); 
}

void drawBReversa() {
    // Parpadear primeros y últimos 15 LEDs en blanco.
    // Serial.println("Ejecutando B Reversa...");
    unsigned long current_millis = millis();

    // Lógica para alternar el estado ON/OFF cada BLINK_INTERVAL (250ms)
    if (current_millis - previous_blink_millis >= BLINK_INTERVAL) {
        previous_blink_millis = current_millis;
        blink_state = !blink_state; // Cambia de ON a OFF o viceversa
    }
    
    FastLED.clear(); 

    if (blink_state) {
        // Estado ON: Encender los 15 LEDs de cada extremo en BLANCO
        for (int i = 0; i < 15; i++) {
            leds[i] = CRGB::White; 
        }
        for (int i = LED_COUNT - 15; i < LED_COUNT; i++) {
            leds[i] = CRGB::White;
        }
    }
    // Estado OFF: No se hace nada (FastLED.clear() ya apagó todo)
    
    FastLED.show();
    delay(50); 
}

void drawIIntermitentes() {
    // Parpadear primeros y últimos 15 LEDs en ambar.
    // Serial.println("Ejecutando I Intermitentes...");
    unsigned long current_millis = millis();
    CRGB amber = CRGB(255, 191, 0); // Define el color Ámbar

    // Utiliza la misma lógica de parpadeo que drawBReversa()
    if (current_millis - previous_blink_millis >= BLINK_INTERVAL) {
        previous_blink_millis = current_millis;
        blink_state = !blink_state; 
    }
    
    FastLED.clear(); 

    if (blink_state) {
        // Estado ON: Encender los 15 LEDs de cada extremo en ÁMBAR
        for (int i = 0; i < 15; i++) {
            leds[i] = amber; 
        }
        for (int i = LED_COUNT - 15; i < LED_COUNT; i++) {
            leds[i] = amber;
        }
    }
    
    FastLED.show();
    delay(50); 
}

void drawLIzquierda() {
    // Animación direccional izquierda (usando dos LEDs).
    // Serial.println("Ejecutando L Izquierda...");
    static unsigned long last_update = 0;
    
    if (millis() - last_update >= ANIMATION_DELAY) {
        last_update = millis();
        FastLED.clear();

        // Mueve la posición de izquierda a derecha (el índice se DECREMENTA)
        current_pos--; 
        
        // Reinicia la posición cuando llega al inicio (Loop Infinito)
        if (current_pos < -1) { 
            current_pos = LED_COUNT - 1; 
        }

        // Dibuja los dos LEDs en movimiento (Amarillo/Ámbar para direccional)
        CRGB color = CRGB::Yellow; 

        // LED principal (posición actual)
        if (current_pos >= 0 && current_pos < LED_COUNT) {
            leds[current_pos] = color;
        }
        // LED de cola (posición actual + 1)
        if (current_pos + 1 >= 0 && current_pos + 1 < LED_COUNT) {
            leds[current_pos + 1] = color.nscale8(100); // Un poco más tenue
        }
        
        FastLED.show();
    }
    delay(20); 
}

void drawRDerecha() {
    // Animación direccional derecha (usando dos LEDs).
    // Serial.println("Ejecutando R Derecha...");
    static unsigned long last_update = 0;
    
    if (millis() - last_update >= ANIMATION_DELAY) {
        last_update = millis();
        FastLED.clear();

        // Mueve la posición de izquierda a derecha (el índice se INCREMENTA)
        current_pos++; 
        
        // Reinicia la posición cuando llega al final (Loop Infinito)
        if (current_pos >= LED_COUNT) {
            current_pos = 0; 
        }

        // Dibuja los dos LEDs en movimiento (Amarillo/Ámbar para direccional)
        CRGB color = CRGB::Yellow; 

        // LED principal (posición actual)
        if (current_pos >= 0 && current_pos < LED_COUNT) {
            leds[current_pos] = color;
        }
        // LED de cola (posición actual - 1)
        if (current_pos - 1 >= 0 && current_pos - 1 < LED_COUNT) {
            leds[current_pos - 1] = color.nscale8(100); // Un poco más tenue
        }
        
        FastLED.show();
    }
    delay(20); 
}

void drawSAlto() {
    // Encender primeros y últimos 15 LEDs en rojo fijo.
    // Serial.println("Ejecutando S Alto...");
    // 1. Apagar toda la tira para empezar limpio
    FastLED.clear(); 
    
    // 2. Encender los PRIMEROS 15 LEDs (índices 0 a 14) en ROJO
    for (int i = 0; i < 15; i++) {
        leds[i] = CRGB::Red; 
    }

    // 3. Encender los ÚLTIMOS 15 LEDs 
    // (Ej: Si LED_COUNT es 60, va de 45 a 59)
    for (int i = LED_COUNT - 15; i < LED_COUNT; i++) {
        leds[i] = CRGB::Red;
    }
    
    // 4. Mostrar los cambios en la tira
    FastLED.show();


    delay(1000); 
}
// Practica.h

#ifndef PRACTICA_H
#define PRACTICA_H
#include <Arduino.h>
#include <FirebaseESP32.h>

// Declaración de las funciones para que 'main.cpp' las reconozca
void practicaSetup();
void practicaLoop();

void segundo_setup();
void segundo_loop();

void free_setup();
void free_loop();

void ledAzul_setup();
void ledAzul_loop();

void rgb_setup();
void rgb_loop();

// aqui empieza la practica de firebase con led RGB

// Definición de los estados/comandos de animación
enum AnimationMode {
    MODE_RGB_DEFAULT, // Estado por defecto (Cometa RGB)
    MODE_B_REVERSA,   // B
    MODE_I_INTERMITENTES, // I
    MODE_L_IZQUIERDA, // L
    MODE_R_DERECHA,   // R
    MODE_S_ALTO       // S
};


extern FirebaseData firebaseData; 
extern AnimationMode currentMode;

// Declaración de las funciones de inicialización y bucle principal
void animacion_Setup();
void animacion_Loop();

// Declaración de las funciones de lectura de Firebase
void firebase_check_command();

// Declaración de las funciones de animación específicas (solo el dibujo)
void drawRGBDefault();
void drawBReversa();
void drawIIntermitentes();
void drawLIzquierda();
void drawRDerecha();
void drawSAlto();



#endif
#include <Arduino.h>


void practicaSetup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH); // deja la luz encendida
    Serial.println("LED encendido");
}

void practicaLoop() {
    delay(1000); // evita uso intensivo de CPU
}
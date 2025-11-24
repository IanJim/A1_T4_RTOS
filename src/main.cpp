#include <Arduino.h>
#include <FirebaseESP32.h> // Librería que instalaste
#include "secrets.h"       // Credenciales
#include "practica.h"

// Objetos globales para Firebase
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

// La URL de la base de datos donde leeremos el comando:
const char *FIREBASE_COMMAND_PATH = "/control/comando";

void setup()
{
  // practicaSetup();
  // segundo_setup();
  // free_setup();
  // ledAzul_setup();
  // rgb_setup();
  //  aqui se inicia la practica de firebase con led RGB
  Serial.begin(115200);

  // 1. Inicialización de la animación (LEDs y Pines)
  animacion_Setup();

  // aqui se comento por mientras funciona en modo prototipo
/*
  // 2. Conexión Wi-Fi
  Serial.print("Conectando a WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi Conectado.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // 3. Configuración y Conexión a Firebase
  config.database_url = FIREBASE_HOST;
  config.api_key = FIREBASE_API_KEY; // <-- Usa la API Key

  // 4. AUTENTICACIÓN: Usamos el método de Token/UID (funciona sin el "database_secret")
  // Esto le da una identidad a tu ESP32.
  auth.token.uid = "MySecretAuth"; // Puedes usar un string cualquiera
  // auth.token.database_secret = FIREBASE_AUTH;
  // config.signer.tokens.database_secret = FIREBASE_AUTH;

  // 5. CONEXIÓN INICIAL
  Firebase.begin(&config, &auth);

  // 6. Reconexión: Asegura que si se cae, se levante
  Firebase.reconnectWiFi(true); // Reconexión automática
  */
}

void loop()
{
  // practicaLoop();
  // segundo_loop();
  // free_loop();
  // ledAzul_loop();
  // rgb_loop();
  // 1. Verificar si hay cambios en Firebase y actualizar el estado

  // aqui empieza la practica de firebase con led RGB

  if (Firebase.ready())
  {
    firebase_check_command();
  }

  // 2. Ejecutar la animación actual
  animacion_Loop();
}

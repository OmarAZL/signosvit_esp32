#include "modules/utils.h"

// Variables para la gestión de estados (Non-blocking)
unsigned long soundTimer = 0;
unsigned long soundDuration = 0;
int currentFrequency = 0;

bool isBuzzerActive = false;
bool isLooping = false;
bool isToneOn = false; // Rastrea si el buzzer está emitiendo sonido físicamente

namespace Utils {
    void init() {
        pinMode(BUZZER_PIN, OUTPUT);
        Serial.println("Sistema listo - Beeps intermitentes configurados.");
    }

    // --- MODULAR FUNCTION: Detiene todo y resetea los estados ---
    void stopTone() {
        noTone(BUZZER_PIN);
        isBuzzerActive = false;
        isLooping = false;
        isToneOn = false;
        Serial.println(">>> Buzzer silenciado manualmente.");
    }

    // --- MODULAR FUNCTION: Inicia un tono con opción de bucle ---
    void startTone(int frequency, int duration, bool loopMode) {
        // GUARDA: Si el buzzer ya está activo, ignoramos la nueva petición
        if (isBuzzerActive) {
            return; 
        }
        currentFrequency = frequency;
        soundDuration = duration;
        isLooping = loopMode;
        isBuzzerActive = true;
        
        // Iniciar la primera fase de sonido (Encendido)
        tone(BUZZER_PIN, frequency);
        isToneOn = true;
        soundTimer = millis();
        
        Serial.print("Buzzer activado - Frecuencia: ");
        Serial.print(frequency);
        Serial.println(isLooping ? " (Modo: BUCLE)" : " (Modo: UNA VEZ)");
    }

    // --- MODULAR FUNCTION: El motor lógico (Debe ejecutarse en el loop) ---
    void updateBuzzer() {
        if (!isBuzzerActive) return;

        // Verificar si la fase actual (sonido o silencio) ha terminado
        if (millis() - soundTimer >= soundDuration) {
            soundTimer = millis(); // Reiniciar el cronómetro para la siguiente fase

            if (isLooping) {
                if (isToneOn) {
                    noTone(BUZZER_PIN); // Cambiar a fase de silencio
                    isToneOn = false;
                    Serial.println("Fase: Silencio...");
                } else {
                    tone(BUZZER_PIN, currentFrequency); // Cambiar a fase de sonido
                    isToneOn = true;
                    Serial.println("Fase: Sonando...");
                }
            } else {
                // Si no es bucle, se detiene tras cumplir la primera duración
                stopTone();
            }
        }
    }

    // --- función nueva: reproduce un tono puntual sin alterar el estado
    // del buzzer que usa las funciones startTone/stopTone. Esto es útil
    // para emitir beeps de latido cardiaco mientras una alarma ya esté sonando.
    void pulseTone(int frequency, int duration) {
        // tone(pin, freq, duration) maneja el apagado automáticamente;
        // no tocamos ninguna variable global para no interferir.
        tone(BUZZER_PIN, frequency, duration);
    }

}
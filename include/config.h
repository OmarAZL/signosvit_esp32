#define SCREEN_WIDTH 128 // Ancho de la pantalla OLED
#define SCREEN_HEIGHT 64 // Alto de la pantalla OLED
#define SCREEN_ADDRESS 0x3C // Dirección I2C de la pantalla OLED

#define DS18B20_PIN 4 // Pin del sensor de temperatura DS18B20
#define DS18B20_RESOLUTION 12 // Resolución del sensor de temperatura DS18B20

#define AD8232_OUT_PIN 34      // Pin analógico de salida ECG
#define AD8232_LOPLUS_PIN 32   // Pin LO+
#define AD8232_LOMINUS_PIN 33  // Pin LO-

#define GY906_ADDRESS 0x5A

#define BUZZER_PIN 25

// constantes útiles para el bus I2C
#ifndef I2C_SPEED_STANDARD
#define I2C_SPEED_STANDARD 100000UL  // 100 kHz
#endif
#ifndef I2C_SPEED_FAST
#define I2C_SPEED_FAST     400000UL  // 400 kHz (el MAX30105 puede usarla)
#endif
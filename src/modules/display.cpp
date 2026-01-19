#include "modules/display.h"

static const unsigned char PROGMEM image_wifi_not_connected_bits[] = {0x21,0xf0,0x00,0x16,0x0c,0x00,0x08,0x03,0x00,0x25,0xf0,0x80,0x42,0x0c,0x40,0x89,0x02,0x20,0x10,0xa1,0x00,0x23,0x58,0x80,0x04,0x24,0x00,0x08,0x52,0x00,0x01,0xa8,0x00,0x02,0x04,0x00,0x00,0x42,0x00,0x00,0xa1,0x00,0x00,0x40,0x80,0x00,0x00,0x00};
static const unsigned char PROGMEM image_wifi_bits[] = {0x01,0xf0,0x00,0x06,0x0c,0x00,0x18,0x03,0x00,0x21,0xf0,0x80,0x46,0x0c,0x40,0x88,0x02,0x20,0x10,0xe1,0x00,0x23,0x18,0x80,0x04,0x04,0x00,0x08,0x42,0x00,0x01,0xb0,0x00,0x02,0x08,0x00,0x00,0x40,0x00,0x00,0xa0,0x00,0x00,0x40,0x00,0x00,0x00,0x00};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

namespace Display {
    bool init() {
        if(display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
            showMessage("Pantalla iniciada");
            return true;
        }
        return false;
    }

    void clear() {
        display.clearDisplay();
    }

    void updateDisplay() {
        display.display();
    }

    void showMessage(const String& message) {
        display.clearDisplay();
        display.setTextSize(1); // Normal size
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print(message);
        display.display(); // Update the display with the new message
    }

    void showData(float &temp1, float &temp2, unsigned long &timestamp, bool &isActiveMQTT) {
        display.clearDisplay();

        if(isActiveMQTT) {
            display.drawBitmap(107, 2, image_wifi_bits, 19, 16, 1);
        } else {
            display.drawBitmap(107, 2, image_wifi_not_connected_bits, 19, 16, 1);
        }

        display.setTextColor(1);
        display.setTextWrap(false);
        display.setCursor(2, 2);
        display.print("Temperatura 1:");

        display.setCursor(2, 24);
        display.print("Temperatura 2:");

        display.setCursor(32, 11);
        display.print(String(temp1, 2) + "C");

        display.setCursor(32, 33);
        display.print(String(temp2, 2) + "C");

        unsigned long now = timestamp / 1000;
        unsigned long seconds = now % 60;
        int minutes = (now / 60) % 60;
        int hours   = (now / 3600) % 24;

        display.setCursor(79, 55);
        // Mostrar hh:mm:ss con ceros
        if (hours < 10) display.print("0");
        display.print(hours);
        display.print(":");

        if (minutes < 10) display.print("0");
        display.print(minutes);
        display.print(":");

        if (seconds < 10) display.print("0");
        display.print(seconds);

        display.display();
    }
}

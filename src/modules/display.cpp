#include "modules/display.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

namespace Display {
    bool init() {
        return display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
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

}
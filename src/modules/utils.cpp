#include "modules/utils.h"

namespace Utils {
    AsyncEventButton btn(BUTTON_PIN, LOW);
    
    void onClickEvent() {
        Serial.println("Hola, botone presionado");
    }

    void init() {
        btn.begin();
        btn.onClick(onClickEvent);
        btn.enable();
    }

}
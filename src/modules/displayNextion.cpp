#include "modules/displayNextion.h"

uint16_t ecgGraph;   
EasyNex myNex(Serial2);

namespace Nextion {
    void init() {
        myNex.begin();
    }

    void start() {
        myNex.writeStr("page page1");
    }

    void write(float &temp1, float &temp2, int &spo2, int &bpm, int &ecg, bool &electrodes) {
        if(electrodes) {
            // el AD8232 está alimentado a 3.3 V y el ESP32 usa ADC de 12 bits
            // (0..4095). la pantalla Nextion espera 0..3300 para 0‑3.3 V.
            ecgGraph = map(ecg, 0, 4095, 0, 3300);
        } else {
            ecgGraph = 0;
        }
        //Serial.print("p");
        //Serial.println(ecgGraph);
        myNex.writeStr("t0.txt", String(temp1, 2));
        myNex.writeStr("t1.txt", String(temp2, 2));
        if(spo2 < 1000) {
            myNex.writeStr("t2.txt", String(spo2));
        }
        myNex.writeStr("t3.txt", String(bpm));
        myNex.writeStr("t4.txt", String(millis()));
        myNex.writeNum("va0.val", ecgGraph);

        myNex.writeNum("va1.val", electrodes ? 1 : 0);
        //Serial.print("e");
        //Serial.println(electrodes);
    }

    // Nota: en la pantalla se configura un temporizador que lee va0 y
    // actualiza el componente de waveform. el puerto serie sólo envía
    // los valores, el refresco lo maneja el propio Nextion.
}

/*
Codigo para el timer de la pantalla Nextion, necesitas dos variables va0, va1

// 1. Calculamos el valor para la gráfica
sys0=va0.val*128/3300
add 5,0,sys0
// 2. Condicional para mostrar el valor o el error
if(va1.val==0)
{
  // Si va1 es false (0), mostramos el mensaje de error
  t5.txt="Desconectado"
}else
{
  // Si va1 es true (1), convertimos y mostramos el valor de va0
  covx va0.val,t5.txt,0,0
}


*/
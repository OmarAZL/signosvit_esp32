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
            ecgGraph = map(ecg, 0, 1023, 0, 3300);
        } else {
            ecgGraph = 1600;
        }
        Serial.println("p"+ecgGraph);
        myNex.writeStr("t0.txt", String(temp1, 2));
        myNex.writeStr("t1.txt", String(temp2, 2));
        if(spo2 < 1000) {
            myNex.writeStr("t2.txt", String(spo2));
        }
        myNex.writeStr("t3.txt", String(bpm));
        myNex.writeStr("t4.txt", String(millis()));
        myNex.writeNum("va0.val", ecgGraph);
        myNex.writeNum("va1.val", electrodes ? 1 : 0);
    }
}

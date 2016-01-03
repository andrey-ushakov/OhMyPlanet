#include "GestureRecognition.h"

GestureRecognition gr;

void setup() {
    Serial.begin(115200);
    gr.setup();
}


void loop() {
    gr.run();
}

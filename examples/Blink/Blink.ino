#include <Flow.h>

void setup() {
  Flow.begin();
  Flow.eval(F("x = digitalWrite(0, 0.5 + squareWave(200)/2)"));
}

void loop() {
  Flow.loop();
}

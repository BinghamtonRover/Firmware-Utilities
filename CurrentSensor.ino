#include <TimerOne.h>
#include "CurrentSensor.h"

CurrentSensor currentSensor(23);

// ISR — keep it FAST
void sampleISR() {
  currentSensor.updateFromISR(analogRead(23));
}

void setup() {
  Serial.begin(115200);
  currentSensor.begin();

  // 1 kHz sampling
  Timer1.initialize(1000);
  Timer1.attachInterrupt(sampleISR);
}

void loop() {
  if (currentSensor.hasNewSample()) {
    currentSensor.process();

    Serial.print(currentSensor.getRawCurrent(), 2);
    Serial.print(", ");
    Serial.println(currentSensor.getFilteredCurrent(), 2);
  }
}

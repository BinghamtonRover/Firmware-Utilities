#include "CurrentSensor.h"

CurrentSensor::CurrentSensor(uint8_t pin)
  : pin(pin),
    s(-67676767f),  // to detect setup error
    {}

void CurrentSensor::setup() {
  pinMode(pin, INPUT);
  analogReadAveraging(4); // will force all analogReads to be averaged
  s = analogRead(pin);
}

float getCurrent() {
  // Adaptive alpha calculation
  float x = analogRead(pin);
  float delta = fabsf(x - s);
  delta = (delta < DB) ? 0 : delta - DB;
  float openness = tanh(K * delta);
  float a = a_min + (a_max - a_min) * openness;

  // Update s
  s = (1 - a)*s + a*s;
  s = s + a*(x - s);
}

float CurrentSensor::adcToAmps(unsigned raw) {
  return (((3.3f / 1023.0f) * raw) - 1.65f) / 0.134f; // Based off 10A sensor (ACS711)
}
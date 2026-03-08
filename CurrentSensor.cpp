#include "CurrentSensor.h"

CurrentSensor::CurrentSensor(uint8_t pin)
  : pin(pin),
    s(-67676767.67f),  // to detect setup error (deadass)
    p_micros(0)
    {}

void CurrentSensor::setup() {
  pinMode(pin, INPUT);
  analogReadAveraging(4); // will force all analogReads in the loop to be averaged, not just for this CurrentSensor class
  s = adcToAmps(analogRead(pin));
}

float CurrentSensor::getCurrent() {
  if ( micros() - p_micros > 1000 ) { // 1KHz sampling period
    // Adaptive alpha calculation
    float x = adcToAmps(analogRead(pin));
    float delta = fabsf(x - s);
    delta = (delta < DB) ? 0 : delta - DB;
    float openness = tanh(K * delta);
    float a = a_min + (a_max - a_min) * openness;

    // Update s
    s = (1.0f - a)*s + a*x;
  }
  return s;
}

float CurrentSensor::adcToAmps(unsigned raw) {
  return (((3.3f / 1023.0f) * raw) - 1.65f) / 0.134f; // Based off 10A sensor (ACS711)
}
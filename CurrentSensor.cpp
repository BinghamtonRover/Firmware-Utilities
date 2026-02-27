#include "CurrentSensor.h"
#include <math.h>

static const float DB = 0.5f;          // Deadband (A)
static const float u90 = 0.5f;
static const float K = 1.4722f / u90;

static const float alpha_min = 0.5f;
static const float alpha_max = 0.5f;

CurrentSensor::CurrentSensor(uint8_t adcPin)
  : _adcPin(adcPin),
    _adcRaw(0),
    _newSample(false),
    _inited(false),
    _s_hat(0.0f),
    _x(0.0f) {}


void CurrentSensor::begin() {
  pinMode(_adcPin, INPUT);
  analogReadAveraging(4);
}

void CurrentSensor::updateFromISR(int adcValue) {
  _adcRaw = adcValue;
  _newSample = true;
}

bool CurrentSensor::hasNewSample() {
  return _newSample;
}

void CurrentSensor::process() {
  if (!_newSample) return;

  noInterrupts();
  int raw = _adcRaw;
  _newSample = false;
  interrupts();

  // ADC → Current
  _x = adcToCurrent(raw);

  // Initialize filter
  if (!_inited) {
    _s_hat = _x;
    _inited = true;
    return;
  }

  // Innovation
  float delta = _x - _s_hat;
  float a = fabsf(delta);
  float alpha = alpha_min;

  if (a > DB) {
    float sign = (delta >= 0.0f) ? 1.0f : -1.0f;
    float delta_db = sign * (a - DB);

    float f = tanh(K * fabsf(delta_db));
    float f01 = 0.5f * (f + 1.0f);

    alpha = alpha_min + (alpha_max - alpha_min) * f01;
  }

  // Filter update
  _s_hat = _s_hat + alpha * (_x - _s_hat);
}

float CurrentSensor::getRawCurrent() const {
  return _x;
}

float CurrentSensor::getFilteredCurrent() const {
  return _s_hat;
}

// ================== Helpers ==================

float CurrentSensor::adcToCurrent(int raw) {
  return (((3.3f / 1023.0f) * raw) - 1.65f) / 0.134f;
}

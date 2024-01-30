#include "BURT_timer.h"

BurtTimer::BurtTimer(int interval, VoidCallback callback) : 
  interval(interval),
  callback(callback)
  { }

void BurtTimer::reset() {
  nextTime = millis() + interval;
}

void BurtTimer::setup() {
  reset();
}

void BurtTimer::update() { 
  if (millis() < nextTime) return;
  callback();
  reset();
}

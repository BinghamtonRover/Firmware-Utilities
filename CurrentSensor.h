#pragma once

#include <Arduino.h>

class CurrentSensor {
public:
    CurrentSensor(uint8_t adcPin);

    // Initialization
    void begin();

    // Called from ISR
    void updateFromISR(int adcValue);

    // Called from main loop
    bool hasNewSample();
    void process();

    // Accessors
    float getRawCurrent() const;
    float getFilteredCurrent() const;

private:

    // Hardware
    uint8_t _adcPin;

    // ISR shared data
    volatile int _adcRaw;
    volatile bool _newSample;

    // Filter state
    bool _inited;
    float _s_hat;
    float _x;

    // Internal helpers
    float adcToCurrent(int raw);
};


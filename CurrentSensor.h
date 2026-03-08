#pragma once

#include <Arduino.h>
#include <math.h>
#include <stdint.h>

class CurrentSensor {
public:
  /**
   * @brief Construct a new CurrentSensor object
   * 
   * @param pin Pin number (must be analog)
   */
  CurrentSensor(uint8_t pin);

  /**
   * @brief Sets up CurrentSensor on pin; initialized first reading
   * 
   * @note All analogRead() calls will be averaged if setup() is called
   */
  void setup();

  /**
   * @brief Get a filtered current reading
   * 
   * @return float 
   */
  float getCurrent() const;

private:
  uint8_t pin;

  const float DB = 0.5f; // Amps
  const float u90 = 0.5f;
  const float K = 1.4722f / u90;
  const float a_min = 0.03f;
  const float a_max = 0.5f;

  bool inited;
  float s;

  /**
   * @brief Helper function to convert raw adc reading into current value (amps)
   * 
   * @param raw ADC reading
   * @note The values are currently based on the ACS711 10A current sensor; the values will be different for different current sensors
   * @return float 
   */
  float adcToAmps(unsigned raw);
};
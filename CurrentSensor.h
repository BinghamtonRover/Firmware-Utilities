#include <Arduino.h>

class CurrentSensor {
public:
  CurrentSensor(uint8_t adcPin);

  // Call once in setup()
  void begin();

  // Called from ISR
  void updateFromISR(int adcValue);

  // Called from loop()
  bool hasNewSample();
  void process();

  float getRawCurrent() const;
  float getFilteredCurrent() const;

private:
  // Hardware
  uint8_t _adcPin;

  // ISR-shared data
  volatile int _adcRaw;
  volatile bool _newSample;

  // Filter state
  bool _inited;
  float _s_hat;
  float _x;

  float adcToCurrent(int raw);
};

#endif

#include <stdint.h>

#define TEMPERATURE 37.7778  // in Celsius

#pragma pack(push, 1)  // do not pad the following struct
struct Data {
  private: 
    // Private so users are forced to use #getTemperature.
    uint16_t temperature;   // 5 digits of precision

  public:
    float angle;  // 7 digits of precision
    uint8_t currentMotor;  // value 1-4
    bool flag;  // 1 bit, but still takes up one byte

    // The constructor parameters can be in any order, but the initializer list
    // should be in the same order as the declarations above.
    Data(float angle, float temperature, int currentMotor, bool flag) : 
      temperature((uint16_t) (temperature*1000)),  // scale to 5 non-decimal digits
      angle(angle),
      currentMotor((uint8_t) currentMotor),
      flag(flag) { }

    // Scales the #temperature back into a float. 
    float getTemperature() { return (float) temperature / 1000; }
};
#pragma pack(pop)  // go back to default compiler settings

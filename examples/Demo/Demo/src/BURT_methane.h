#include <Arduino.h>  // needed for Arduino code, like Serial

class MethaneSensor {
  private: 
  	int pin;
  
	public:
  	MethaneSensor(int pin) : pin(pin) { }
		float read();
};

#include "BURT_methane.h"

float MethaneSensor::read() { 
	return analogRead(pin);  // insert real logic here
}

#include <BURT_can.h>
#include "data.h"  // the same data defined in "receive.ino"

#define SIGNAL_ID 1

Data data(PI, TEMPERATURE, 3, true);

void setup() {
	Serial.begin(9600);
	BurtCan::setup();
	Serial.println("Finished setup");
}

void loop() {
	BurtCan::send(SIGNAL_ID, data);
	Serial.println("Sent message");
	delay(1000);
}

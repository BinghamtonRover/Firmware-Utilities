#include <BURT_can.h>

#define SIGNAL_ID 1

uint8_t data[8];

void setup() {
	Serial.begin(9600);
	BurtCan::setup();
	BurtCan::packFloat(PI, data, 0);
	BurtCan::packFloat(2*PI, data, 4);
	Serial.println("Finished setup");
}

void loop() {
	BurtCan::send(SIGNAL_ID, data);
	Serial.println("Sent message");
	delay(1000);
}

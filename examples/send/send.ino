#include <BURT_can.h>

#define SIGNAL_ID 1

uint8_t data[] = {1, 2, 3, 4, 5, 6, 7, 8};

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

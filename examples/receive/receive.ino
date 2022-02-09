#include <BURT_can.h>

#define ROTATE_ID 5

void rotate(const CanMessage& message) { 
  Serial.println("Rotating motor 1 by: " + message.buf[0]);
  Serial.println("Rotating motor 2 by: " + message.buf[1]);
}

void setup() {
	Serial.begin(9600);
	BurtCan::setup();
	BurtCan::registerHandler(ROTATE_ID, rotate);
}

void loop() {
	BurtCan::update();
}

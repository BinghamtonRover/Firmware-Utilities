#include <BURT_can.h>

#define SIGNAL_ID 1

void handler(const CanMessage& message) { 
  float pi = BurtCan::unpackFloat(message.buf, 0);
  float tau = BurtCan::unpackFloat(message.buf, 4);

  Serial.print("Pi: ");
  Serial.print(pi);
  Serial.print(", Tau: ");
  Serial.println(tau);
}

void setup() {
	Serial.begin(9600);
	BurtCan::setup();
	BurtCan::registerHandler(SIGNAL_ID, handler);
	Serial.println("Finished setup");
}

void loop() {
	Serial.println("Checking for messages");
	BurtCan::update();
	delay(1000);
}

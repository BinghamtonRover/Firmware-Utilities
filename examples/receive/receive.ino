#include <BURT_can.h>
#include "data.h"  // the same data defined in "send.ino"

#define SIGNAL_ID 1

void handler(const CanMessage& message) { 
	Data data = BurtCan::bytesToStruct<Data>(message.buf);

  Serial.print("Angle: ");
  Serial.print(data.angle);
  Serial.print(", Temperature: ");
  Serial.print(data.getTemperature());
  Serial.print("Current Motor: ");
  Serial.print(data.currentMotor);
  Serial.print("Flag: ");
  Serial.println(data.flag);
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

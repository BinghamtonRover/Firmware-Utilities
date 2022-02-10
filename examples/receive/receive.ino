#include <BURT_can.h>

#define SIGNAL_ID 1

void handler(const CanMessage& message) { 
  Serial.print("Received signal with data: ");
  for (int index = 0; index < 8; index++) {
    Serial.print(message.buf[index]);
    Serial.print(" ");
  }
  Serial.print("\n");
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

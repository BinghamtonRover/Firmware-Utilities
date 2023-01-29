#include <BURT_can.h>

#include "user.pb.h"

void rawHandler(const CanMessage& message) {
  Serial.print("1. Received ");
  Serial.print(message.len);
  Serial.print(" bytes: ");
  for (int i = 0; i < message.len; i++) {
    Serial.print(message.buf[i], HEX);
  }
  Serial.println("");
}

void userHandler(const CanMessage& message) {
  User bob = BurtProto::decode<User>(message.buf, User_fields);
  Serial.print("2. Received user with ID: ");
  Serial.println(bob.id);
}

void setup() {
	Serial.begin(9600);
	BurtCan::setup();
	BurtCan::registerHandler(1, rawHandler);
  BurtCan::registerHandler(2, userHandler);
	Serial.println("Finished setup");
}

void loop() {
	BurtCan::update();
	delay(10);
}

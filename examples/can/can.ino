#include <BURT_can.h>

#include "user.pb.h"

#define USER_ID 0x01

User bob = { .id=123 };  // the data we want to send

/// A function to be called whenever a User is received.
void userHandler(const uint8_t* buffer, int length) {
	// Decode the user and print something about them.
  User bob = BurtProto::decode<User>(buffer, length, User_fields);
  Serial.print("2. Received user with ID: ");
  Serial.println(bob.id);
}

/// The CAN service.
BurtCan can(USER_ID, userHandler);

void setup() {
	Serial.begin(9600);
	can.setup();

	Serial.println("Finished setup");
}

void loop() {
	can.update();
	can.send(USER_ID, &bob, User_fields);

	Serial.print("Sent user with ID: ");
	Serial.println(bob.id);
	delay(100);
}

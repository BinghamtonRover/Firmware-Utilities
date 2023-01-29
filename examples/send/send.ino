#include <BURT_can.h>

#include "user.pb.h"

uint8_t data[8] = {0xd, 0xe, 0xa, 0xd, 0xb, 0xe, 0xe, 0xf};
uint8_t userBuf[8];
User bob = { .id=123 };

void print_buf(uint8_t* buf, int length) {
  for (int i = 0; i < length; i++) Serial.print(buf[i], HEX);
  Serial.print("\n");
}

void setup() {
	Serial.begin(9600);
	BurtCan::setup();
	Serial.println("Finished setup");

  int bytes = BurtProto::encode(userBuf, User_fields, &bob);
  Serial.print("Will be sending bytes: ");
  print_buf(userBuf, bytes);
}

void loop() {
  BurtCan::sendRaw(1, data);
	BurtCan::send(2, User_fields, &bob);

	Serial.println("Sent raw data on ID=1 and Bob on ID=2");
	delay(1000);
}

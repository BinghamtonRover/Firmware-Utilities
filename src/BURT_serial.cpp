#include "BURT_serial.h"

void BurtSerial::parseSerial() {
	int length = Serial.available();
	if (length == 0) return;
	uint8_t input[length];
	Serial.readBytes((char*) input, length);

	if (!isConnected) tryConnect(input);
	else handler(input, length);
}

void BurtSerial::tryConnect(uint8_t* input) {
	// Parse as an incoming Connect request
	Connect connect = BurtProto::decode<Connect>(input, Connect_fields);
	bool isValid = connect.sender == Device::Device_DASHBOARD
	  && connect.receiver == Device::Device_FIRMWARE;
	if (!isValid) return;

	// Send a Connect response
	Connect response;
	response.receiver = Device::Device_DASHBOARD;
	response.sender = Device::Device_FIRMWARE;
	uint8_t buffer[4];
	int newLength = BurtProto::encode(buffer, Connect_fields, &response);
	Serial.write(buffer, newLength);
  isConnected = true;
}

#include "BURT_serial.h"
#include "BURT_proto.h"

void BurtSerial::update() {
	int length = Serial.available();
	if (length == 0) return;
	uint8_t input[length];
	Serial.readBytes((char*) input, length);

	if (!isConnected) tryConnect(input, length);
	else handler(input, length);
}

void BurtSerial::tryConnect(uint8_t* input, int length) {
	// Parse as an incoming Connect request
	Connect connect = BurtProto::decode<Connect>(input, length, Connect_fields);
	bool isValid = connect.sender == Device::Device_DASHBOARD
		&& connect.receiver == Device::Device_FIRMWARE;
	if (!isValid) return;

	// Send a Connect response
	Connect response;
	response.receiver = Device::Device_DASHBOARD;
	response.sender = device;
	uint8_t buffer[8];
	int newLength = BurtProto::encode(buffer, Connect_fields, &response);
	Serial.write(buffer, newLength);
	isConnected = true;
}

/**
 * @brief Sends an encoded protobuf message over a serial connection.
 *
 * This function encodes the given message using the provided field structure. 
 * Then, it sends the encoded message over a serial connection 
 * if the connection has been established using the tryConnect method of BurtSerial.
 *
 * @param fields The Protobuf descriptor for this message. Use the generated MessageName_fields.
 * @param message Pointer to the message structure to be sent, must be a protobuf message.
 * @param length The maximum length of the encoded message. Use the generated MessageName_size.
 * @return Returns `true` if the entire message is sent successfully, `false` otherwise.
 */
bool BurtSerial::send(const pb_msgdesc_t* fields, const void* message, int length) {
	if (!isConnected) return false;

	uint8_t buffer[length];
	int encodedLength = BurtProto::encode(buffer, fields, message);
	
	int sentLength = Serial.write(buffer, encodedLength);
	return encodedLength == sentLength;
}

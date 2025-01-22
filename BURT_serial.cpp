#include "BURT_serial.h"
#include "BURT_proto.h"
#include "version.pb.h"
#include "wrapper.pb.h"

BurtSerial::BurtSerial(Device device, ProtoHandler onMessage, const pb_msgdesc_t* descriptor, int length, Version version, bool receipt = false) :
	device(device),
	onMessage(onMessage),
	descriptor(descriptor),
	length(length),
	version(version),
	receipt(receipt),
	{ }

// bool isResetCode(uint8_t* buffer, int length) {
// 	return length >= 4
// 		&& buffer[0] == 0
// 		&& buffer[1] == 0
// 		&& buffer[2] == 0
// 		&& buffer[3] == 0;
// }

void BurtSerial::update() {
	int length = Serial.available();
	if (length == 0) return;
	uint8_t input[length];
	int receivedLength = Serial.readBytes((char*) input, length);

	if (!isConnected){
		tryConnect(input, length);
	}

	// NO CHECK 
	WrappedMessage msg = BurtProto::decode<WrappedMessage>(input, length, WrappedMessage_fields);

	if(msg.Version.major != this->Version.major)
	{
		return;
		// Send back invalid version message?
	}

	switch(msg.MessageType)
	{
		case MessageType::MessageType_HEARTBEAT:
			BurtSerial::send(std::nullopt, MessageType::MessageType_HEARTBEAT)
			// check sender validity?
			break;
		case MessageType::MessageType_DISCONNECT:
			uint8_t response[4] = {0x01, 0x01, 0x01, 0x01};
			//Serial.write(response, 4);
			BurtSerial::send(response, MessageType::MessageType_DISCONNECT)
			onDisconnect();
			isConnected = false;
			break;
		case MessageType::MessageType_COMMAND:
			onMessage(input, length);
			// break;
		case MessageType_DATA:
			BurtSerial::send(input)
	}

	// } else if (isResetCode(input, receivedLength)) {
	// 	// This is our special "reset" code. Respond with 1111
	// 	uint8_t response[4] = {0x01, 0x01, 0x01, 0x01};
	// 	Serial.write(response, 4);
	// 	isConnected = false;
	// } else {
	// 	onMessage(input, length);
	// }
}

// CHANGE THIS
void BurtSerial::tryConnect(uint8_t* input, int length) {
	// Parse as an incoming Connect request
	Connect connect = BurtProto::decode<Connect>(input, length, Connect_fields);
	bool isValid = connect.receiver == Device::Device_FIRMWARE;
	if (!isValid) return;

	// Send a Connect response
	Connect response;
	response.receiver = connect.sender;
	response.sender = device;
	uint8_t buffer[8];
	int newLength = BurtProto::encode(buffer, Connect_fields, &response, Connect_size);
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
bool BurtSerial::send(const std::optional<void*> message, const MessageType& msgType = MessageType::MessageType_DATA) {

	// Check if message null -> then skip encode (example case: heartbeat)
	
	// Wrap it to wrapped message
	BurtProto::encode()

	// check if connected, if not, return false
	if (!isConnected) return false;

	uint8_t* buffer = new uint8_t[length];
	int encodedLength = BurtProto::encode(buffer, descriptor, message, length);
	
	int sentLength = Serial.write(buffer, encodedLength);
	delete[] buffer;
	return encodedLength == sentLength;
}

bool BurtSerial:sendLogMessage(BurtLog message){
	
	return true;
}

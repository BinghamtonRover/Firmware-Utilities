#include "BURT_serial.h"
#include "BURT_proto.h"
#include "version.pb.h"
#include "wrapper.pb.h"

BurtSerial::BurtSerial(Device device, ProtoHandler onMessage, const pb_msgdesc_t* descriptor, int length, Version version, VoidCallback onDisconnect, bool receipt) :
	device(device),
	onMessage(onMessage),
	descriptor(descriptor),
	length(length),
	version(version),
	onDisconnect(onDisconnect),
	receipt(receipt)
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

	//Decode message
	std::optional<WrappedMessage> msg_opt = BurtProto::decode<WrappedMessage>(input, length, WrappedMessage_fields);
	// Check if msg is goodie

	if (!msg_opt.has_value())
	{
		// SOUND ALARM!!!!!!!!!!!!!!!
	}

	WrappedMessage msg = msg_opt.value();

	if(msg.version.major != this->version.major)
	{
		return;
		// Send back invalid version message?
	}
	if(msg.version.minor != this->version.minor)
	{
		// Warn user of minor version mismatch
	}

	switch(msg.type)
	{
		case MessageType::MessageType_HEARTBEAT:
		{
<<<<<<< HEAD
			BurtSerial::send(reinterpret_cast<const void*>(onMessage), MessageType::MessageType_HEARTBEAT);
=======
			BurtSerial::send(nullptr, MessageType::MessageType_HEARTBEAT);
>>>>>>> c6175e6353f3388790cc52459e903a5aa403b07a
			// check sender validity?
			break;
		}
		case MessageType::MessageType_DISCONNECT:
		{
<<<<<<< HEAD
			BurtSerial::send(reinterpret_cast<const void*>(onDisconnect), receivedLength, MessageType::MessageType_DISCONNECT);
=======
			uint8_t response[4] = {0x01, 0x01, 0x01, 0x01};
			//Serial.write(response, 4);
			BurtSerial::send(response, MessageType::MessageType_DISCONNECT);
			onDisconnect();
>>>>>>> c6175e6353f3388790cc52459e903a5aa403b07a
			isConnected = false;
			break;
		}
		case MessageType::MessageType_COMMAND:
		{
<<<<<<< HEAD
			BurtSerial::send(reinterpret_cast<const void*>(onMessage), receivedLength, MessageType::MessageType_COMMAND);
=======
			onMessage(input, length);
>>>>>>> c6175e6353f3388790cc52459e903a5aa403b07a
			break;
		}
		case MessageType::MessageType_DATA:
		{
<<<<<<< HEAD
			BurtSerial::send(reinterpret_cast<const void*>(onMessage), receivedLength, MessageType::MessageType_COMMAND);
=======
			BurtSerial::send(msg.data);
>>>>>>> c6175e6353f3388790cc52459e903a5aa403b07a
			break;
		}
		case MessageType::MessageType_LOG_MESSAGE:
		{
<<<<<<< HEAD
			BurtSerial::sendLogMessage(reinterpret_cast<const void*>(onMessage), receivedLength);
=======
			BurtSerial::sendLogMessage(msg.data);
>>>>>>> c6175e6353f3388790cc52459e903a5aa403b07a
			break;
		}
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
	std::optional<Connect> connect_opt = BurtProto::decode<Connect>(input, length, Connect_fields);
	
	// Check if connect_operation is not null
	if (!connect_opt.has_value())
	{
		// Send log message to indicate bad connection or try again with recursion?
		return;
	}
	
	Connect connect = connect_opt.value();
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
<<<<<<< HEAD
bool BurtSerial::send(const void* message, const int length, const MessageType& msgType) {
=======
bool BurtSerial::send(const void* message, const MessageType& msgType) {
>>>>>>> c6175e6353f3388790cc52459e903a5aa403b07a

	// Check if message null -> then skip encode (example case: heartbeat)

	// check if connected, if not, return false
	if (!isConnected) return false;

	uint8_t* buffer = new uint8_t[length];
	int encodedLength = BurtProto::encode(buffer, descriptor, message, length);
	
	int sentLength = Serial.write(buffer, encodedLength);
	delete[] buffer;
	return encodedLength == sentLength;
}

<<<<<<< HEAD
bool BurtSerial::sendLogMessage(const void* message, int length){
=======
bool BurtSerial::sendLogMessage(BurtLog message){
>>>>>>> c6175e6353f3388790cc52459e903a5aa403b07a
	
	return true;
}

#include "BURT_can.h"

/// A fallback handler that makes it clear to the user that they forgot to set a real handler.
void _defaultHandler(const uint8_t* data, int length) {
	Serial.println("Warning: You have not registered a callback function for CAN.");
	Serial.println("Please do so by calling BurtCan::registerHandler.");
	while (true);
}

/// A callback to run whenever a CAN message of interest arrives.
/// 
/// This **must** be a top-level function, as it is called by #canHandler. 
static ProtoHandler globalHandler = _defaultHandler;

/// A top-level function to handle incoming CAN messages. 
/// 
/// This function delegates to #globalHandler with the contents of the message. This **must** be a 
/// top-level function as required `FlexCan_T4.onReceive`.
void canHandler(const CanMessage& message) {
	globalHandler(message.buf, message.len);
}

BurtCan::BurtCan(uint32_t id, ProtoHandler onMessage) : id(id) {
	globalHandler = onMessage;
}

void BurtCan::setup() {
	// Sets the baud rate and default message policy. 
  can.begin();
  can.setBaudRate(CAN_BAUD_RATE);
  can.enableFIFO();
  can.enableFIFOInterrupt();
  can.onReceive(canHandler);
}

void BurtCan::update() { 
	can.events();
}

void BurtCan::sendRaw(uint32_t id, uint8_t data[8], int length) {
	if (length > 8) {
		Serial.println("Message is too long");
		return;
	}
	// Initializes a CAN frame with the given data and sends it.
	CanMessage frame = {};
	frame.id = id;
	frame.len = length;
	memset(frame.buf, 0, 8);
	memcpy(frame.buf, data, length);
	can.write(frame);
}

bool BurtCan::send(uint32_t id, const void* message, const pb_msgdesc_t* fields) {
	// Encodes a Protobuf message and then sends it using #sendRaw.
	uint8_t data[8];
	int length = BurtProto::encode(data, fields, message);
	if (length == -1) {
		Serial.println("Failed to encode message"); 
		return false;
	} else if (length > 8) {
		Serial.println("Encoded message is too long");
		return false;
	}

	sendRaw(id, data, length);
	return true;
}

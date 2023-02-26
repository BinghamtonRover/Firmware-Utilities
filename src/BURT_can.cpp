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

/// The number of the next available mailbox.
static int mailbox = MB0;

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
  can.enableMBInterrupts();
  can.setMBFilter(REJECT_ALL);

  // Creates a new mailbox set to handle [id] with [handler]. 
  FLEXCAN_MAILBOX mb = FLEXCAN_MAILBOX(mailbox);
  can.setMBFilter(mb, id);
  can.onReceive(mb, canHandler);
  mailbox += 1;
}

void BurtCan::update() { 
	can.events();
}

void BurtCan::sendRaw(uint32_t id, uint8_t data[8], int length) {
	// Initializes a CAN frame with the given data and sends it.
	CanMessage frame;
	frame.id = id;
	frame.len = length;
	memcpy(frame.buf, data, 8);
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

#include "BURT_can.h"

void _defaultHandler(const uint8_t* data, int length) {
	Serial.println("Warning: You have not registered a callback function for CAN.");
	Serial.println("Please do so by calling BurtCan::registerHandler.");
	while (true);
}

int BurtCan::mailbox = MB0;
Can BurtCan::can;
ProtoHandler BurtCan::protoHandler = _defaultHandler;
uint32_t BurtCan::id = 0;

void BurtCan::setup() {
	// Sets the baud rate and default message policy. 
  can.begin();
  can.setBaudRate(CAN_BAUD_RATE);
  can.enableMBInterrupts();
  can.setMBFilter(REJECT_ALL);
}

void BurtCan::registerHandler(uint32_t id, ProtoHandler handler) {
	// Creates a new mailbox set to handle [id] with [handler]. 
	BurtCan::id = id;
	BurtCan::protoHandler = handler;
	FLEXCAN_MAILBOX mb = FLEXCAN_MAILBOX(mailbox);
	can.setMBFilter(mb, id);
	can.onReceive(mb, &BurtCan::canHandler);
	mailbox += 1;
}

void BurtCan::canHandler(const CanMessage& message) {
	BurtCan::protoHandler(message.buf, message.len);
}

void BurtCan::sendRaw(uint32_t id, uint8_t data[8], int length) {
	CanMessage frame;
	frame.id = id;
	frame.len = length;
	memcpy(frame.buf, data, 8);
	can.write(frame);
}

bool BurtCan::send(uint32_t id, const pb_msgdesc_t* fields, const void* message) {
	uint8_t data[8] = {0, 0, 0, 0, 0, 0, 0, 0};
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

void BurtCan::update() { 
	can.events();
}

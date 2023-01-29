#include "BURT_can.h"

int BurtCan::mailbox = MB0;
Can BurtCan::can;

void BurtCan::setup() {
	// Sets the baud rate and default message policy. 
  can.begin();
  can.setBaudRate(CAN_BAUD_RATE);
  can.enableMBInterrupts();
  can.setMBFilter(REJECT_ALL);
}

void BurtCan::registerHandler(uint32_t id, const CanHandler handler) {
	// Creates a new mailbox set to handle [id] with [handler]. 
	FLEXCAN_MAILBOX mb = FLEXCAN_MAILBOX(mailbox);
	can.setMBFilter(mb, id);
	can.onReceive(mb, handler);
	mailbox += 1;
}

void BurtCan::sendRaw(uint32_t id, uint8_t data[8]) {
	CanMessage frame;
	frame.id = id;
	memcpy(frame.buf, data, 8);
	can.write(frame);
}

bool BurtCan::send(uint32_t id, const pb_msgdesc_t* fields, const void* message) {
	uint8_t data[8];
	bool status = BurtProto::encode(data, fields, message);
	if (!status) return false;

	sendRaw(id, data);
	return true;
}

void BurtCan::update() { 
	can.events();
}

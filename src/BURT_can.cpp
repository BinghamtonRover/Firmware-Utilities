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

void BurtCan::send(uint32_t id, uint8_t data[DATA_LENGTH]) {
	// Creates a new message and prefills it with [id] and [data].
	CanMessage message;
	message.id = id;
	Serial.println("Sending message...");
	for (int index = 0; index < DATA_LENGTH; index++) {
		Serial.print("  Current byte: ");
		Serial.println(data[index]);
		message.buf[index] = data[index];
	}
	can.write(message);
	Serial.println("Message sent.");
}

void BurtCan::registerHandler(uint32_t id, const CanHandler handler) {
	// Creates a new mailbox set to handle [id] with [handler]. 
	FLEXCAN_MAILBOX mb = FLEXCAN_MAILBOX(mailbox);
	can.setMBFilter(mb, id);
	can.onReceive(mb, handler);
	mailbox += 1;
}

void BurtCan::update() { 
	can.events();
}

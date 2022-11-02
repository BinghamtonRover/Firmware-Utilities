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

void BurtCan::packFloat(float num, uint8_t buffer[8], int index) { 
	if (index > 4) return 0;  // otherwise will write out of bounds
	// [ptr] is a pointer to an array of bytes, starting at the memory address of [num].
  uint8_t* ptr = (uint8_t*) &num;
  for (int i = index; i < index + 4; i++) {
    buffer[i] = *(ptr + i);
  }
}

float BurtCan::unpackFloat(uint8_t buffer[8], int index) {
	if (index > 4) return -1;  // otherwise will read out of bounds

	// First, allocate a new float (defaults to 0.0)
  float num;
  // Next, read its floating-point representation as an array of bytes, similar to [packFloat].
  uint8_t* ptr = (uint8_t*) &num;
  for (int i = 0; i < 4; i++) {
    *(ptr + i) = buffer[i];
  }
  return num;
}
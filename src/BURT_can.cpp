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

void BurtCan::packFloat(uint8_t *buffer, float data, int bytes, float start, float end){
	float range = end-start;
	uint32_t max = (1LL<<(bytes*8))-1;
	float rel = data-start;
	float percent = rel/range;
	if(percent>1) percent = 1.0;
	if(percent<0) percent = 0.0;
	uint32_t representation = (uint32_t)(percent*max);

	for(int i = 0; i<bytes; i++){
		buffer[i] = (representation&(0xff<<(8*i)))>>(8*i);
	}
}

float BurtCan::unpackFloat(uint8_t *buffer, int bytes, float start, float end){
	float range = end-start;
	uint32_t max = (1LL<<(bytes*8))-1;

	uint32_t sink = 0;

	for(int i = 0; i<bytes; i++){
		sink+=buffer[i]<<(8*i);
	}

	float percent = ((float)sink)/max;
	float rel = percent*range;
	return start+rel;
}
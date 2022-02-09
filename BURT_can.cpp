#include "BURT_can.h"

int BurtCan::mailbox = MB0;
Can BurtCan::can;

void BurtCan::setup() {
	// Configure the baud rate: 
  CANFD_timings_t config;
  config.clock = CLK_24MHz;
  config.baudrate = CAN_BAUD_RATE;
  config.baudrateFD = CAN_BAUD_RATE;
  config.propdelay = 190;
  config.bus_length = 1;
  config.sample = 70;

  // Configure all other settings
  can.begin();
  can.setBaudRate(config);
  can.enableMBInterrupts();
  can.setMBFilter(REJECT_ALL);
}

void BurtCan::send(uint32_t id, uint8_t data[DATA_LENGTH]) {
	CanMessage message;
	message.id = id;
	for (int index = 0; index < DATA_LENGTH; index++) {
		message.buf[index] = data[index];
	}
	can.write(message);
}

void BurtCan::registerHandler(uint32_t id, const CanHandler handler) {
	// Register the handler and ID to [mailbox] and update the count.
	FLEXCAN_MAILBOX mb = FLEXCAN_MAILBOX(mailbox);
	can.setMBFilter(mb, id);
	can.onReceive(mb, handler);
	mailbox += 1;
}

void BurtCan::update() { 
	can.events();
}


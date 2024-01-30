#include "BURT_can.h"

/// The number of the next available mailbox.
static int mailbox = MB0;

template <class CanType>
BurtCan<CanType>::BurtCan(uint32_t id, Device device, ProtoHandler onMessage, VoidCallback onDisconnect, bool useExtendedIds) : 
	id(id),
	device(device),
	onMessage(onMessage),
	onDisconnect(onDisconnect),
	useExtendedIds(useExtendedIds)
	{ }

template <class CanType>
void BurtCan<CanType>::handleCanFrame(const CanMessage& message) {
	onMessage(message.buf, message.len);
}

template <class CanType>
void BurtCan<CanType>::setup() {
	// Sets the baud rate and default message policy. 
  can.begin();
  can.setBaudRate(CAN_BAUD_RATE);
  can.enableMBInterrupts();
  can.setMBFilter(REJECT_ALL);

  // Creates a new mailbox set to handle [id] with [handler]. 
  FLEXCAN_MAILBOX mb = FLEXCAN_MAILBOX(mailbox);
  can.setMBFilter(mb, id);
  mailbox += 1;
}

template <class CanType>
void BurtCan<CanType>::update() { 
	int count = 0;
	while (true) {
		CanMessage message;
		int success = can.read(message);  // 0=no message, 1=message read
		if (success > 0) {
			count++;
			if (count == 10) {
				Serial.println("[BurtCan] Warning: More than 10 messages are being read in loop().");
				Serial.println("[BurtCan] Warning:   Consider calling can.update() more often, reduce the");
				Serial.println("[BurtCan] Warning:   amount of messages on the CAN bus, or consider increasing");
				Serial.println("[BurtCan] Warning:   this limit. Your messages are still being processed.");
			}
			onMessage(message.buf, message.len);
		} else {
			return;
		}
	}
}

template <class CanType>
void BurtCan<CanType>::sendRaw(uint32_t id, uint8_t data[8], int length) {
	if (length > 8) {
		Serial.println("Message is too long");
		return;
	}
	// Initializes a CAN frame with the given data and sends it.
	CanMessage frame = {};
	if (useExtendedIds) frame.flags.extended = 1;
	frame.id = id;
	frame.len = length;
	memset(frame.buf, 0, 8);
	memcpy(frame.buf, data, length);
	can.write(frame);
}

template <class CanType>
bool BurtCan<CanType>::send(uint32_t id, const void* message, const pb_msgdesc_t* fields) {
	// Encodes a Protobuf message and then sends it using #sendRaw.
	uint8_t data[8];
	int length = BurtProto::encode(data, fields, message);
	if (length == -1) {
		Serial.println("[BurtCan] Error: Failed to encode message"); 
		return false;
	} else if (length > 8) {
		Serial.println("[BurtCan] Error: Encoded message is too long");
		return false;
	}

	sendRaw(id, data, length);
	return true;
}

template <class CanType>
void BurtCan<CanType>::showDebugInfo() {
	Serial.println("[BurtCan] Debug: Showing debug info...");
	can.mailboxStatus();
}

// ----- DO NOT REMOVE -----
// The following tells C++ which concrete implementations of "CanType" we will need.
// Without this, the BurtCan class will compile but you cannot use it since it's never seen
// a "real" CanType (ie, Can1, Can2, Can3) in this file. We have to use all three CAN types here.
template class BurtCan<Can1>; 
template class BurtCan<Can2>; 
template class BurtCan<Can3>; 

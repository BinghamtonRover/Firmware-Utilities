#include "BURT_can.h"
#include "rover.h"

/// The number of the next available mailbox for standard frames.
static int standardMailbox = MB0;

/// The number of the next available mailbox for extended frames.
static int extendedMailbox = MB4;

template <class CanType>
BurtCan<CanType>::BurtCan(uint32_t id, CanHandler onMessage, bool useExtendedIds) {
	config = {
		.idStart = id,
		.onMessage = onMessage,
		.useExtendedIds = useExtendedIds
	};
}

template <class CanType>
BurtCan<CanType>::BurtCan(uint32_t idStart, uint32_t idEnd, CanHandler onMessage, bool useExtendedIds) {
	config = {
		.idStart = idStart,
		.idEnd = idEnd,
		.onMessage = onMessage,
		.useExtendedIds = useExtendedIds
	};
}

template <class CanType>
BurtCan<CanType>::BurtCan(BurtCanConfig config) : config(config) {}

template <class CanType>
void BurtCan<CanType>::handleCanFrame(const CanMessage& message) {
	if (config.isRoverNetwork && message.id == ROVER_ROVER_HEARTBEAT_FRAME_ID) {
		onHeartbeatMessage(message);
	} else {
		config.onMessage(message);
	}
}

template <class CanType>
void BurtCan<CanType>::onHeartbeatMessage(const CanMessage& message) {
	rover_rover_heartbeat_t heartbeat = {0};
	rover_rover_heartbeat_unpack(&heartbeat, message.buf, message.len);
	receivedHeartbeat = true;
	if (!isConnected) {
		isConnected = true;
		config.onConnect();
	}
	// TODO: Update an internal rover status
	sendBroadcastMessage();
}

template <class CanType>
void BurtCan<CanType>::sendBroadcastMessage() {
	rover_device_broadcast_t broadcastMessage = {0};
	broadcastMessage.device_value = static_cast<uint8_t>(config.device);
	broadcastMessage.fw_version_major = static_cast<uint8_t>(config.version.major);
	broadcastMessage.fw_version_minor = static_cast<uint8_t>(config.version.minor);

	CanMessage message;
	message.id = ROVER_DEVICE_BROADCAST_FRAME_ID;
	message.len = rover_device_broadcast_pack(message.buf, &broadcastMessage, message.len);
	sendMessage(message);
}

template <class CanType>
void BurtCan<CanType>::checkHeartbeats() {
	if (!receivedHeartbeat && isConnected) {
		config.onDisconnect();
		isConnected = false;
	}
	receivedHeartbeat = false;
}

template <class CanType>
void BurtCan<CanType>::setup() {
	heartbeatTimer.setup();
	// Sets the baud rate and default message policy.
	can.begin();
	can.setMBFilter(REJECT_ALL);
	if (config.isRoverNetwork) {
		can.setBaudRate(ROVER_NETWORK_BAUD_RATE);
		can.setClock(CLK_60MHz);
	} else {
		can.setBaudRate(CAN_BAUD_RATE);
	}

	FLEXCAN_MAILBOX mb = config.useExtendedIds
		? FLEXCAN_MAILBOX(extendedMailbox++)
		: FLEXCAN_MAILBOX(standardMailbox++);

	// It's unclear if `setMBFilterRange` can be called with a range of [x, x],
	// so we take care to call the correct function here.
	if (config.idEnd == 0 && !config.isRoverNetwork) {  // only one ID to listen to
		can.setMBFilter(mb, config.idStart);
	} else {  // listen to a range
		if (!config.isRoverNetwork) {
			can.setMBFilterRange(mb, config.idStart, config.idEnd);
		} else {
			FLEXCAN_MAILBOX hbMailbox = config.useExtendedIds
				? FLEXCAN_MAILBOX(extendedMailbox++)
				: FLEXCAN_MAILBOX(standardMailbox++);
			can.setMBFilter(hbMailbox, ROVER_ROVER_HEARTBEAT_FRAME_ID);
			can.setMBFilterRange(mb, config.idStart, config.idStart | 0x0F);
			can.enhanceFilter(mb);
			can.enhanceFilter(hbMailbox);
		}
	}
}

template <class CanType>
void BurtCan<CanType>::update() {
	if (config.isRoverNetwork) {
		heartbeatTimer.update();
	}
	int count = 0;
	while (true) {
		CanMessage message;
		int success = can.read(message);  // 0=no message, 1=message read
		if (success == 0) return;
		count++;
		handleCanFrame(message);
	}
}

template <class CanType>
bool BurtCan<CanType>::sendRaw(uint32_t id, uint8_t data[8], int length) {
	if (length > 8) {
		return false;
	}
	// Initializes a CAN frame with the given data and sends it.
	CanMessage frame = {};
	if (config.useExtendedIds) frame.flags.extended = 1;
	frame.id = id;
	frame.len = length;
	memset(frame.buf, 0, 8);
	memcpy(frame.buf, data, length);
	can.write(frame);

	return true;
}

template <class CanType>
bool BurtCan<CanType>::send(uint32_t id, const void* message, const pb_msgdesc_t* fields) {
	// Encodes a Protobuf message and then sends it using #sendRaw.
	uint8_t data[8];
	int length = BurtProto::encode(data, fields, message, 8);
	if (length == -1) {
		return false;
	} else if (length > 8) {
		return false;
	}

	sendRaw(id, data, length);
	return true;
}

template <class CanType>
void BurtCan<CanType>::sendMessage(const CanMessage& message) {
	can.write(message);
}

template <class CanType>
void BurtCan<CanType>::showDebugInfo() {
	// Mailbox Status prints to Serial
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

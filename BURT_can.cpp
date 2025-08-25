#include "BURT_can.h"
#include "rover.h"

/// The number of the next available mailbox for standard frames.
static int standardMailbox = MB0;

/// The number of the next available mailbox for extended frames.
static int extendedMailbox = MB4;

template <class CanType>
BurtCan<CanType>::BurtCan(uint32_t id, CanHandler onMessage, bool useExtendedIds) :
	idStart(id),
	idEnd(0),
	onMessage(onMessage),
	useExtendedIds(useExtendedIds)
	{ }

template <class CanType>
BurtCan<CanType>::BurtCan(uint32_t idStart, uint32_t idEnd, CanHandler onMessage, bool useExtendedIds) :
	idStart(idStart),
	idEnd(idEnd),
	onMessage(onMessage),
	useExtendedIds(useExtendedIds)
	{ }

template <class CanType>
BurtCan<CanType>::BurtCan(
	uint32_t nodeId,
	Device device,
	Version version,
	CanHandler onMessage,
	VoidCallback onDisconnect,
	VoidCallback onConnect = []() {},
	bool useExtendedIds) :
		idStart(nodeId),
		idEnd(nodeId | 0x0F),
		onMessage(onMessage),
		device(device),
		version(version),
		onDisconnect(onDisconnect),
		onConnect(onConnect),
		useExtendedIds(useExtendedIds) {
	heartbeatTimer = BurtTimer(HEARTBEAT_CHECK_MS, [this]() -> void {
		this->checkHeartbeats();
	});
}

template <class CanType>
void BurtCan<CanType>::handleCanFrame(const CanMessage& message) {
	if (message.id == ROVER_ROVER_HEARTBEAT_FRAME_ID) {
		onHeartbeatMessage(message);
	} else {
		onMessage(message);
	}
}

template <class CanType>
void BurtCan<CanType>::onHeartbeatMessage(const CanMessage& message) {
	rover_rover_heartbeat_t heartbeat;
	rover_rover_heartbeat_unpack(&heartbeat, message.buf, message.len);
	receivedHeartbeat = true;
	// TODO: Update an internal rover status
	sendBroadcastMessage();
}

template <class CanType>
void BurtCan<CanType>::sendBroadcastMessage() {
	rover_device_broadcast_t broadcastMessage = {
		.device_value = device,
		.fw_version_major = version.major,
		.fw_version_minor = version.minor
	};
	CanMessage message;
	message.id = ROVER_DEVICE_BROADCAST_FRAME_ID;
	message.len = rover_device_broadcast_pack(message.buf, &broadcastMessage, message.len);
	sendMessage(message);
}

template <class CanType>
void BurtCan<CanType>::checkHeartbeats() {
	if (!receivedHeartbeat) {
		if (isConnected) {
			onDisconnect();
			isConnected = false;
		}
	} else {
		if (!isConnected) {
			isConnected = true;
			onConnect();
		}
		sendBroadcastMessage();
	}
	receivedHeartbeat = false;
}

template <class CanType>
void BurtCan<CanType>::setup() {
	heartbeatTimer.setup();
	// Sets the baud rate and default message policy.
	can.begin();
	can.setBaudRate(CAN_BAUD_RATE);
	can.setMBFilter(REJECT_ALL);
	if (isRoverCan()) {
		can.setClock(CLK_60MHz);
	}

	FLEXCAN_MAILBOX mb = useExtendedIds
		? FLEXCAN_MAILBOX(extendedMailbox++)
		: FLEXCAN_MAILBOX(standardMailbox++);

	// It's unclear if `setMBFilterRange` can be called with a range of [x, x],
	// so we take care to call the correct function here.
	if (idEnd == 0) {  // only one ID to listen to
		can.setMBFilter(mb, idStart);
	} else {  // listen to a range
		if (!isRoverCan()) {
			can.setMBFilterRange(mb, idStart, idEnd);
		} else {
			FLEXCAN_MAILBOX hbMailbox = useExtendedIds
				? FLEXCAN_MAILBOX(extendedMailbox++)
				: FLEXCAN_MAILBOX(standardMailbox++);
			can.setMBFilter(hbMailbox, ROVER_ROVER_HEARTBEAT_FRAME_ID);
			can.setMBFilterRange(mb, idStart, idEnd);
			can.enhanceFilter(mb);
			can.enhanceFilter(hbMailbox);
		}
	}
}

template <class CanType>
void BurtCan<CanType>::update() {
	if (isRoverCan()) {
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
	if (useExtendedIds) frame.flags.extended = 1;
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

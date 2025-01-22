#pragma once

#include <Arduino.h>

#include "BURT_proto.h"
#include "version.pb.h"
#include "wrapper.pb.h"

#include "../logs.pb.h"

class BurtSerial {
	public: 
		bool isConnected = false;

		BurtSerial(
			Device device, 
			ProtoHandler onMessage, 
			const pb_msgdesc_t* descriptor, 
			int length, 
			Version version, 
			VoidCallback onDisconnect,
			bool receipt = false
			);
		void setup() { /* No setup needed */ }
		void update();
		bool send(const void* message, const MessageType& msgType = MessageType::MessageType_DATA);
		void decode();
		bool sendLogMessage(BurtLog message);

	private: 
		void tryConnect(uint8_t* input, int length);
		Device device;
		ProtoHandler onMessage;
		const pb_msgdesc_t* descriptor;
		int length;
		Version version;
		VoidCallback onDisconnect;
		bool receipt;
};

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
<<<<<<< HEAD
		bool send(const void* message, const int length, const MessageType& msgType = MessageType::MessageType_DATA);
		void decode();
		bool sendLogMessage(const void* message, int length);
=======
		bool send(const void* message, const MessageType& msgType = MessageType::MessageType_DATA);
		void decode();
		bool sendLogMessage(BurtLog message);
>>>>>>> c6175e6353f3388790cc52459e903a5aa403b07a

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

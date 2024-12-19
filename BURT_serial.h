#pragma once

#include <Arduino.h>

#include "BURT_proto.h"
#include "version.pb.h"

class BurtSerial {
	public: 
		bool isConnected = false;

		BurtSerial(Device device, ProtoHandler onMessage, const pb_msgdesc_t* descriptor, int length, Version version);
		void setup() { /* No setup needed */ }
		void update();
		bool send(const void* message);
		void decode();
		bool sendLogMessage(BurtLog message);
		bool receipt;

	private: 
		void tryConnect(uint8_t* input, int length);
		Device device;
		ProtoHandler onMessage;
		const pb_msgdesc_t* descriptor;
		int length;
		Version version;
};

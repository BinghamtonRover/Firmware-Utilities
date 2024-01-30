#pragma once

#include <Arduino.h>

#include "BURT_proto.h"

class BurtSerial {
	public: 
		bool isConnected = false;

		BurtSerial(Device device, ProtoHandler onMessage, VoidCallback onDisconnect);
		void setup() { /* No setup needed */ }
		void update();
		bool send(const pb_msgdesc_t* fields, const void* message, int length);

	private: 
		void tryConnect(uint8_t* input, int length);
		Device device;
		ProtoHandler onMessage;
		VoidCallback onDisconnect;
};

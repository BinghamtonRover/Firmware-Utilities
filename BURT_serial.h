#pragma once

#include <Arduino.h>

#include "BURT_proto.h"
#include "BURT_timer.h"

const int heartbeatInterval = 500;

class BurtSerial {
	public: 
		bool isConnected = false;

		BurtSerial(Device device, ProtoHandler onMessage, const pb_msgdesc_t* descriptor, 
		int length, VoidCallback onDisconnect);
		void setup();
		void update();
		bool send(const void* message);

	private: 
		void tryConnect(uint8_t* input, int length);
		Device device;
		ProtoHandler onMessage;
		const pb_msgdesc_t* descriptor;
		int length;
		
		// Heartbeat Functionality
		bool receivedHeartbeat = false;
		void heartbeatCheck();
		VoidCallback onDisconnect;
		BurtTimer heartbeatTimer;
};

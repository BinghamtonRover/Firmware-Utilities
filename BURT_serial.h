#pragma once

#include <Arduino.h>

#include "BURT_proto.h"
#include "core.pb.h"

class BurtSerial {
	public: 
		bool isConnected = false;

		BurtSerial(ProtoHandler handler, Device device) : handler(handler), device(device) {}
		void update();
		bool send(const pb_msgdesc_t* fields, const void* message);
	private: 
		void tryConnect(uint8_t* input, int length);
		ProtoHandler handler;
		Device device;
};

#include <Arduino.h>
#include <BURT_proto.h>

#include "core.pb.h"

class BurtSerial {
	public: 
		bool isConnected = false;

		BurtSerial(ProtoHandler handler) : handler(handler) {}
		void parseSerial();
	private: 
		void tryConnect(uint8_t* input, int length);
		ProtoHandler handler;
};

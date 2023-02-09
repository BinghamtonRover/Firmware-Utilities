#include "BURT_proto.h"

#include <Arduino.h>

int BurtProto::encode(uint8_t* buffer, const pb_msgdesc_t* fields, const void* message) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, 8);
	bool status = pb_encode(&stream, fields, message);
	if (!status) {
		Serial.print("Encoding failed: ");
		Serial.println(stream.errmsg);
		return -1;
	}
	
	return stream.bytes_written;
}

bool BurtProto::decodeRaw(const uint8_t* buffer, const pb_msgdesc_t* fields, void* message) {
	pb_istream_t stream = pb_istream_from_buffer(buffer, sizeof(buffer));
	return pb_decode(&stream, fields, message);
}

#include "BURT_proto.h"

#include <Arduino.h>

int BurtProto::encode(uint8_t* buffer, const pb_msgdesc_t* fields, const void* message) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, 8);
	// This *should* return false on failure, but it seems to return false positives
	pb_encode(&stream, fields, message);
	return stream.bytes_written;
}

bool BurtProto::decodeRaw(const uint8_t* buffer, int length, const pb_msgdesc_t* fields, void* message) {
	pb_istream_t stream = pb_istream_from_buffer(buffer, length);
	return pb_decode(&stream, fields, message);
}

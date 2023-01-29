#include "BURT_proto.h"

int BurtProto::encode(uint8_t* buffer, const pb_msgdesc_t* fields, const void* message) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	bool status = pb_encode(&stream, fields, message);
	if (!status) return -1;
	else return stream.bytes_written;
}

bool BurtProto::decodeRaw(const uint8_t* buffer, const pb_msgdesc_t* fields, void* message) {
	pb_istream_t stream = pb_istream_from_buffer(buffer, sizeof(buffer));
	return pb_decode(&stream, fields, message);
}

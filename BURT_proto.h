#pragma once

#include "nanopb/pb.h"
#include "nanopb/pb_common.h"
#include "nanopb/pb_encode.h"
#include "nanopb/pb_decode.h"

#include "core.pb.h"
#include "types.h"


class BurtProto {
	public:
		static int encode(uint8_t* buffer, const pb_msgdesc_t* fields, const void* message, int length);
		static bool decodeRaw(const uint8_t* buffer, int length, const pb_msgdesc_t* fields, void* message);

		template<typename T>
		static T decode(const uint8_t* buffer, int length, const pb_msgdesc_t* fields) {
			T result;
			decodeRaw(buffer, length, fields, &result);
			return result;
		}
};

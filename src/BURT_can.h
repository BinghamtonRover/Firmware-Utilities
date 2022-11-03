/*! @file BURT_can.h */

#ifndef burt_can_h
#define burt_can_h

#include <Arduino.h>
#include <FlexCAN_T4.h>
#include <functional>

#define CAN_BAUD_RATE 500000
#define DATA_LENGTH 8

/// The CAN service.
/// 
/// For internal use only. Uses the `FlexCAN_T4` library.
typedef FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can;

// A message in the CAN protocol format.
// 
// See https://en.wikipedia.org/wiki/CAN_bus#Data_transmission for details.
typedef CAN_message_t CanMessage;

/// A handler function that accepts a #CanMessage.
/// 
/// Use these with BurtCan::registerHandler to listen for messages. 
typedef void (*CanHandler)(const CanMessage& msg);

/// A service to send and receive messages via the CAN bus protocol.
/// 
/// The CAN protocol allows microcontrollers to efficiently communicate. On the rover, 
/// CAN is used to coordinate the subsystems to the onboard computer. When the computer
/// receives a command from the operator, it translates it to a CAN message with a specific
/// ID. The ID designates the purpose of the message as well as which function is to handle it.
/// 
/// Internally, this service delegates to the [`FlexCAN_T4`](https://github.com/tonton81/FlexCAN_T4) library.
/// 
/// Usage: 
/// - Use #setup to initialize the CAN service
/// - Use #update to check for new messages
/// - Use #send to send a struct to a given ID
/// - Use #registerHandler to designate a function to handle messages with a given ID
/// - Use #bytesToStruct to convert a CAN payload into structured data.
/// 
/// The structs you use must be at most 8 bytes to conform to the size of the CAN payload. 
/// This means you'll want to choose the smallest representations for your data. For example,
/// if you have a float for which you don't need high accuracy, you can store it as a `uint16_t`
/// for 5 digits of precision. You may want to make that field private and introduce a public
/// function that scales the number back into a float. 
/// 
/// The order of fields determines the size of the struct due to **padding**. To tell the compiler
/// to pack your data as efficiently as possible, use `#pragma pack(push, 1)` before your struct
/// declaration and `pragma pack(pop)` afterwards. 
/// 
/// See https://en.wikipedia.org/wiki/CAN_bus for more details.
class BurtCan {
	private: 
		static int mailbox;  ///< The number of the next available mailbox.
		static Can can;  ///< The `FlexCAN_T4` instance.

	public: 
		/// Initializes settings in CAN. 
		/// 
		/// This configures CAN to reject all messages by default. To process a message, register a 
		/// function to handle the message in #registerHandler.
		static void setup();

		/// Checks for incoming messages.
		/// 
		/// Call this in the loop function to stay up-to-date on message delivery.
		static void update();

		/// Sends a message via the CAN bus protocol.
		/// 
		/// The ID determines the purpose of the message and who will receive it. See #structToBytes
		/// for details on how the struct instance is converted to a byte array. 
		/// 
		/// WARNING: DO NOT move to `.cpp`. Templates can only be declared in the header file. 
		template <class T>
		static void send(uint32_t id, T data) {
			CanMessage message;
			message.id = id;
			uint8_t* bytes = structToBytes<T>(data);
			for (int index = 0; index < DATA_LENGTH; index++) {
				message.buf[index] = bytes[index];
			}
			can.write(message);
		}

		/// Registers a handler function for the given ID.
		/// 
		/// When a message is received, the `FlexCAN_T4` library will parse its ID and send it 
		/// to a mailbox that accepts that ID. By calling this function, you create a mailbox
		/// that is configured to handle all messages with the given ID and call the handler.
		static void registerHandler(uint32_t id, CanHandler handler);

		/// Returns an array of 8 bytes representing the 8-byte struct `T`.
		/// 
		/// This function allows you to declare your data as a struct instead of manually filling
		/// a byte buffer. Simply pass it to this function and parse it with #bytesToStruct. 
		/// 
		/// More technically, a struct is simply a contiguous block of memory with enough "slots"
		/// for all its fields. A pointer to a struct instance is simply a pointer to the first
		/// field. Similarly, a byte array is also a contiguous block of memory with enough "slots"
		/// for all its bytes, and is also represented by the pointer to the first element. This
		/// function simply takes the pointer to the beginning of the struct's memory block and 
		/// tells C++ to treat it as the memory block of a byte array. 
		/// 
		/// This function asserts that the struct is indeed 8 bytes long with `static_assert`. 
		/// WARNING: DO NOT move to `.cpp`. Templates can only be declared in the header file. 
		template <class T> 
		static inline uint8_t* structToBytes(T& data) {
			static_assert(sizeof(T) <= 8, "T cannot exceed 8 bytes");
			// Simply assert that the struct is actually a byte array.
			return (uint8_t*) &data;
		}

		/// Returns a struct instance `T` represented by the bytes in `buffer`. 
		/// 
		/// This function allows you to declare your data as a struct instead of manually filling
		/// a byte buffer. Simply pass it to #structToBytes and parse it with this function. 
		/// 
		/// More technically, a struct is simply a contiguous block of memory with enough "slots"
		/// for all its fields. A pointer to a struct instance is simply a pointer to the first
		/// field. Similarly, a byte array is also a contiguous block of memory with enough "slots"
		/// for all its bytes, and is also represented by the pointer to the first element. This
		/// function simply takes the pointer to the beginning of the array's memory block and 
		/// tells C++ to treat it as the memory block of a struct instance. 
		/// 
		/// This function asserts that the struct is indeed 8 bytes long with `static_assert`. 
		/// WARNING: DO NOT move to `.cpp`. Templates can only be declared in the header file. 
		template <class T> 
		static inline T bytesToStruct(const uint8_t buffer[8]) {
			static_assert(sizeof(T) <= 8, "T cannot exceed 8 bytes");
			// Inner *: Treat buffer as a T*
			// Outer *: Dereference the T at that address
			return *( (T*) buffer );
		}
};

#endif

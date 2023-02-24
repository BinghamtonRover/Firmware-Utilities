/*! @file BURT_can.h */

#ifndef burt_can_h
#define burt_can_h

#include <Arduino.h>
#include <FlexCAN_T4/FlexCAN_T4.h>

#include "BURT_proto.h"

#define CAN_BAUD_RATE 500000
#define DATA_LENGTH 8

/// The CAN service using the Teensy pins at CAN1.
using Can1 = FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16>;

/// The CAN service using the Teensy pins at CAN2. 
using Can2 = FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16>;

/// The CAN service using the Teensy pins at CAN3. 
using Can3 = FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16>;

// A message in the CAN protocol format.
// 
// See https://en.wikipedia.org/wiki/CAN_bus#Data_transmission for details.
using CanMessage = CAN_message_t;

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
/// - Create an instance and pass in the ID and callback to handle incoming messages
/// - Use #setup to initialize the CAN service
/// - Use #update to check for new messages
/// - Use #send to send a message with a given ID
/// 
/// See https://en.wikipedia.org/wiki/CAN_bus for more details.
class BurtCan {
	private: 
		/// The underlying `FlexCAN_T4` instance.
		Can3 can;  

		/// The ID of the message this instance will respond to.
		uint32_t id;

	public: 
		/// Creates a CAN handler that calls #onMessage for messages with the given ID.
		BurtCan(uint32_t id, ProtoHandler onMessage);

		/// Initializes the CAN hardware to handle messages with #id.
		/// 
		/// When a message is received, the `FlexCAN_T4` library will parse its ID and send it 
		/// to a mailbox that accepts that ID. By calling this function, you create a mailbox
		/// that is configured to handle all messages with the given ID and call the handler.
		void setup();

		/// Call this function in `loop` to check for incoming messages.
		void update();

		/// Sends a byte array via the CAN bus protocol.
		void sendRaw(uint32_t id, uint8_t data[8], int length = 8);

		/// Sends a Protobuf message via the CAN bus protocol.
		bool send(uint32_t id, const void* message, const pb_msgdesc_t* fields);
};

#endif

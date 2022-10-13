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
/// - Use #send to send some data to a given ID
/// - Use #registerHandler to designate a function to handle messages with a given ID
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
		/// The ID determines the purpose of the message and who will receive it. The data 
		/// buffer is restricted to #DATA_LENGTH.
		static void send(uint32_t id, uint8_t data[DATA_LENGTH]);

		/// Registers a handler function for the given ID.
		/// 
		/// When a message is received, the `FlexCAN_T4` library will parse its ID and send it 
		/// to a mailbox that accepts that ID. By calling this function, you create a mailbox
		/// that is configured to handle all messages with the given ID and call the handler.
		static void registerHandler(uint32_t id, CanHandler handler);

		/// Will fill a data buffer based on a given float.
		/// 
		/// The buffer pointer refers to where the data packing will begin. For example, when sending with CAN,
		/// you usually will create a uint8_t array of size 8 to represent the packet. So if you want to start packing the float `x`
		/// at index 2 of the arr, you would use `packFloat(&arr[2],x,2,0,100)`. 
		/// The above command would pack the float into 2 bytes (so index 2 and 3), assuming that the minimum `x` could be was 0 and the max is 100.
		/// The min and max are important to scale the float to gain maximum precision. Note all these numbers down, as they will be used to unpack the float later.
		///
		/// Some important limits to know: start<=data<=end, 1<=bytes<=4
		/// Also, it should be self-explanatory, but buffer must have at least `bytes` positions left to be filled.
		static void packFloat(uint8_t *buffer, float data, int bytes, float start, float end);

		/// The counterpart of `packFloat`. 
		///
		///
		static float unpackFloat(uint8_t *buffer, int bytes, float start, float end);
};

#endif

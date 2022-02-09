#ifndef burt_can_h
#define burt_can_h

#include <FlexCAN_T4.h>
#include <functional>

#define CAN_BAUD_RATE 500'000
#define DATA_LENGTH 64

typedef FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_16> Can;
typedef CANFD_message_t CanMessage;
typedef void (*CanHandler)(const CanMessage& msg);

class BurtCan {
	private: 
		static int mailbox;
		static Can can;

	public: 
		/* Initializes settings in CAN */
		static void setup();

		/* Checks for incoming messages. */
		static void update();

		/* Sends a message via CAN */
		static void send(uint32_t id, uint8_t data[DATA_LENGTH]);

		/* Registers a handler function for the given ID. */
		static void registerHandler(uint32_t id, CanHandler handler);
};

#endif

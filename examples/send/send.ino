#include <BURT_can.h>

#define SIGNAL_ID 1

uint8_t data[] = {1, 2, 3, 4, 5, 6, 0, 0};

BurtCan::packFloat(&data[6],3.14,2,0,6.28); // Pack 3.14 into the last 2 bytes of data. The min and max control how 3.14 will be packed.
// To unpack this, we would use unpackFloat(&data[6],2,0,6.28) to get the float of 3.14 back when receiving.


void setup() {
	Serial.begin(9600);
	BurtCan::setup();
	Serial.println("Finished setup");
}

void loop() {
	BurtCan::send(SIGNAL_ID, data);
	Serial.println("Sent message");
	delay(1000);
}

#include "BURT_spi.h"

BurtSPI::BurtSPI(const uint8_t (&csAddrPins)[4],
                 uint8_t addressEnable,
                 uint8_t outEnable,
                 unsigned delayUs) :
    addressEnable(addressEnable),
    outEnable(outEnable),
    delayUs(delayUs) { 
    for (int i = 0; i < 4; i++) {
        csAddress[i] = csAddrPins[i];
    }
}

void BurtSPI::setupSPI() {
    pinMode(addressEnable, OUTPUT);
    pinMode(outEnable,     OUTPUT);

    // EN signals are ACTIVE LOW; deassert them (HIGH) at init
    digitalWrite(addressEnable, HIGH);
    digitalWrite(outEnable, HIGH);

    // Initialize address pins to a known safe state (LOW)
    pinMode(csAddress[0], OUTPUT); digitalWrite(csAddress[0], LOW);
    pinMode(csAddress[1], OUTPUT); digitalWrite(csAddress[1], LOW);
    pinMode(csAddress[2], OUTPUT); digitalWrite(csAddress[2], LOW);
    pinMode(csAddress[3], OUTPUT); digitalWrite(csAddress[3], LOW);

    SPI.begin();
    idle = true;
#if BURT_DEBUG
    Serial.println("SPI Bus Initialized");
#endif
}

bool BurtSPI::prepareTransaction(uint8_t addr) {
    if (addr >= 16) { return false; }
  
    // Keep outputs disabled until caller explicitly enables them (active LOW)
    digitalWrite(outEnable, HIGH);

    addr &= 0x0F;

    if (!idle) return false;
  
    // Pulse the addressEnable low to allow changing the address (addressEnable is active LOW)
    digitalWrite(addressEnable, LOW); // Enable addr selection (active LOW)

    digitalWrite(csAddress[0], ((addr >> 0) & 0b0001) ? HIGH : LOW);
    digitalWrite(csAddress[1], ((addr >> 1) & 0b0001) ? HIGH : LOW);
    digitalWrite(csAddress[2], ((addr >> 2) & 0b0001) ? HIGH : LOW);
    digitalWrite(csAddress[3], ((addr >> 3) & 0b0001) ? HIGH : LOW);

    // Deassert addressEnable to latch the address (rising edge when active-low EN is released)
    digitalWrite(addressEnable, HIGH); // Latch addr

    // Small settle time to allow decoder outputs to become valid
    if (delayUs) delayMicroseconds(delayUs);
    idle = false;
    return true;
}

void BurtSPI::enableOutput() {
    // Active LOW: pull low to enable
    digitalWrite(outEnable, LOW);
}

void BurtSPI::disableOutput() {
    digitalWrite(outEnable, HIGH);
}

void BurtSPI::goToIdle() {
    // Ensure outputs are disabled (active LOW)
    disableOutput();
    if (delayUs) delayMicroseconds(delayUs);
    idle = true;
}


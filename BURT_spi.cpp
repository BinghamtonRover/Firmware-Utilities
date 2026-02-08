#include "BURT_spi.h"


BurtSPI::BurtSPI(const uint8_t (&cs_addr_pins)[4],
                 uint8_t addr_EN,
                 uint8_t out_EN,
                 unsigned delay_us) :
    addr_EN(addr_EN),
    out_EN(out_EN),
    delay_us(delay_us)
    { 
        for (int i = 0; i < 4; i++) {
            CS_addr[i] = cs_addr_pins[i];
        }
    }

void BurtSPI::setupSPI() {
    pinMode(addr_EN,    OUTPUT);
    pinMode(out_EN,     OUTPUT);

    // EN signals are ACTIVE LOW; deassert them (HIGH) at init
    digitalWrite(addr_EN, HIGH);
    digitalWrite(out_EN,  HIGH);

    // Initialize address pins to a known safe state (LOW)
    pinMode(CS_addr[0], OUTPUT); digitalWrite(CS_addr[0], LOW);
    pinMode(CS_addr[1], OUTPUT); digitalWrite(CS_addr[1], LOW);
    pinMode(CS_addr[2], OUTPUT); digitalWrite(CS_addr[2], LOW);
    pinMode(CS_addr[3], OUTPUT); digitalWrite(CS_addr[3], LOW);

    SPI.begin();
    idle = true;
    #if defined(BURT_DEBUG)
    Serial.println("SPI Bus Initialized");
    #endif
}

bool BurtSPI::prepareTransaction(uint8_t addr) {
    if (addr >= 16) { return false; }
    // Keep outputs disabled until caller explicitly enables them (active LOW)
    digitalWrite(out_EN, HIGH);

    addr &= 0x0F;
    if (!idle) return false;
    else {
        // Pulse the addr_EN low to allow changing the address (addr_EN is active LOW)
        digitalWrite(addr_EN, LOW); // Enable addr selection (active LOW)

        digitalWrite(CS_addr[0], (addr & 0x01) ? HIGH : LOW);
        digitalWrite(CS_addr[1], (addr & 0x02) ? HIGH : LOW);
        digitalWrite(CS_addr[2], (addr & 0x04) ? HIGH : LOW);
        digitalWrite(CS_addr[3], (addr & 0x08) ? HIGH : LOW);

        // Deassert addr_EN to latch the address (rising edge when active-low EN is released)
        digitalWrite(addr_EN, HIGH); // Latch addr

        // Small settle time to allow decoder outputs to become valid
        if (delay_us) delayMicroseconds(delay_us);
        idle = false;
        return true;
    }   
}

void BurtSPI::enableOutput() {
    // Active LOW: pull low to enable
    digitalWrite(out_EN, LOW);
}

void BurtSPI::disableOutput() {
    digitalWrite(out_EN, HIGH);
}

void BurtSPI::goToIdle() {
    // Ensure outputs are disabled (active LOW)
    disableOutput();
    if (delay_us) delayMicroseconds(delay_us);
    idle = true;
}


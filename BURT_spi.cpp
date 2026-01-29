#include "BURT_spi.h"


BurtSPI::BurtSPI(uint8_t CS_addr[4], uint8_t addr_EN, uint8_t out_EN, unsigned delay_us) :
    CS_addr(CS_addr),
    addr_EN(addr_EN),
    out_EN(out_EN),
    delay_us(delay_us)
    { }

BurtSPI::setupSPI() {
    pinMode(CS_addr[0], OUTPUT);
    pinMode(CS_addr[1], OUTPUT);
    pinMode(CS_addr[2], OUTPUT);
    pinMode(CS_addr[3], OUTPUT);
    pinMode(addr_EN,    OUTPUT);
    pinMode(out_EN,     OUTPUT);

    digitalWrite(addr_EN, HIGH);
    digitalWrite(out_EN,  HIGH);

    SPI.begin();
    idle = true;
    Serial.println("SPI Bus Initialized");
}

BurtSPI::prepareTransaction(uint8_t addr) {
    if (!idle) {
        Serial.println("SPI Bus not ready for transaction");
        return false;
    }
    else {
        idle = false;
        digitalWrite(addr_EN, LOW); // Enable addr selection

        delayMicroseconds(1);

        digitalWrite(CS_addr[0], (addr & 0b00000001));
        digitalWrite(CS_addr[1], (addr & 0b00000010));
        digitalWrite(CS_addr[2], (addr & 0b00000100));
        digitalWrite(CS_addr[3], (addr & 0b00001000));

        delayMicroseconds(1);

        digitalWrite(addr_EN, HIGH); // Latch addr

        delayMicroseconds(1);

        digitalWrite(out_EN, LOW); // Prepped for transaction
    }   
}


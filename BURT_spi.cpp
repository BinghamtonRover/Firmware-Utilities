#include "BURT_spi.h"

/// @brief For use with 74hc4515 4:16 Decoder IC for Chip Select
/// @param CS_addr array of 4 pins serving as addressing pins to the decoder
/// @param addr_EN enables the decoder to change its addr
/// @param out_EN enables the selected output (active LOW)
/// @param delay_us optional; small delay after successful transaction (keep below 10us)
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
    ready = true;
    Serial.println("SPI Bus Initialized");
}

BurtSPI::prepareTransaction(uint8_t addr) {
    if (!ready) {
        Serial.println("SPI Bus not ready for transaction");
        return false;
    }
    else {
        digitalWrite(addr_EN, LOW); // Enable addr selection

        delayMicroseconds(1);

        digitalWrite(CS_addr[0], (addr & 0b00000001));
        digitalWrite(CS_addr[1], (addr & 0b00000010));
        digitalWrite(CS_addr[2], (addr & 0b00000100));
        digitalWrite(CS_addr[3], (addr & 0b00001000));

        delayMicroseconds(1);

        digitalWrite(addr_EN, HIGH); // Latch addr

        delayMicroseconds(1);

        digitalWrite(out_EN, LOW);
    }   
}


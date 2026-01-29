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

bool BurtSPI::prepareTransaction(uint8_t addr) {
    digitalWrite(out_EN, HIGH);
    addr &= 0x0F;
    if (!idle) return false;
    else {
        idle = false;
        digitalWrite(addr_EN, LOW); // Enable addr selection

        digitalWrite(CS_addr[0], (addr & 0b00000001) ? HIGH : LOW);
        digitalWrite(CS_addr[1], (addr & 0b00000010) ? HIGH : LOW);
        digitalWrite(CS_addr[2], (addr & 0b00000100) ? HIGH : LOW);
        digitalWrite(CS_addr[3], (addr & 0b00001000) ? HIGH : LOW);

        digitalWrite(addr_EN, HIGH); // Latch addr

        return true;
    }   
}

void BurtSPI::goToIdle() {
    digitalWrite(out_EN, HIGH);
    idle = true;
    if (delay_us) delayMicroseconds(delay_us);
}


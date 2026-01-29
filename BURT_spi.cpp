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

    SPI.begin();
    Serial.println("SPI Bus Initialized");
}


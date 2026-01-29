#pragma once
#include <Arduino.h>

class BurtSPI {
    private:
        uint8_t CS_addr[4];
        uint8_t addr_EN;
        uint8_t out_EN;
    public:
        BurtSPI(uint8_t CS_addr[4], uint8_t addr_EN, uint8_t out_EN);
}
#pragma once
#include <Arduino.h>
#include <SPI.h>

class BurtSPI {
    private:
        uint8_t CS_addr[4];
        uint8_t addr_EN;
        uint8_t out_EN;

        bool ready = false;
    public:
        BurtSPI(uint8_t CS_addr[4], uint8_t addr_EN, uint8_t out_EN, unsigned delay_us);

        void setupSPI();
        bool prepareTransaction(uint8_t addr);
        bool goToIdle();
}

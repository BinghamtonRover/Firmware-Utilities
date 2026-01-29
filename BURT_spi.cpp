#include "BURT_spi.h"

BurtSPI::BurtSPI(uint8_t CS_addr[4], uint8_t addr_EN, uint8_t out_EN) :
    CS_addr(CS_addr),
    addr_EN(addr_EN),
    out_EN(out_EN)
    { }

BurtSPI::prepareTransaction(uint8_t addr) {
    if (!ready) {
        Serial.println("SPI Bus not ready for transaction");
        return false;
    }
}
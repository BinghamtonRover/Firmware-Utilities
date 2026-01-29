#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <stdint.h>

/* 
Call Order:
if (BurtSPI.prepareTransaction(device_addr)) {
    device.func_that_uses_SPI(out_EN);
} else 
{ /.* Raise error flag *./}
BurtSPI.goToIdle();

This will ensure that the bus is idle, then set up the correct virtual CS/addr pin
then call the external library's function with the out_EN pin so it can handle timing
then close the transaction and go back to idle
*/

/// @brief For use with 74hc4515 4:16 Decoder IC for Chip Select
/// @param CS_addr array of 4 pins serving as addressing pins to the decoder
/// @param addr_EN enables the decoder to change its addr
/// @param out_EN enables the selected output (active LOW)
/// @param delay_us small delay after successful transaction, pass 0 to disable (keep below 10us)
class BurtSPI {
    private:

        uint8_t CS_addr[4];
        uint8_t addr_EN;
        uint8_t out_EN;
        
        unsigned delay_us;

        bool idle = false;
    public:
        BurtSPI::BurtSPI(const uint8_t (&cs_addr_pins)[4],
                 uint8_t addr_EN,
                 uint8_t out_EN,
                 unsigned delay_us);
                 
        void setupSPI();
        bool prepareTransaction(uint8_t addr);
        void goToIdle();
};

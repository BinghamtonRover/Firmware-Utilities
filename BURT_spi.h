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
/// @param addr_EN enables the decoder address latch (ACTIVE LOW)
/// @param out_EN enables the selected output (ACTIVE LOW)
/// @param delay_us small delay after successful transaction, pass 0 to disable (keep below 10us)
class BurtSPI {
    private:

        uint8_t CS_addr[4];
        uint8_t addr_EN;
        uint8_t out_EN;
        
        unsigned delay_us;

        bool idle = false;
    public:
        BurtSPI(const uint8_t (&cs_addr_pins)[4],
                uint8_t addr_EN,
                uint8_t out_EN,
                unsigned delay_us);

        void setupSPI();
        bool prepareTransaction(uint8_t addr);

        /**
         * @note Prefer that external component libraries do not call `disableOutput()` 
         * and `enableOutput()` directly. Using `goToIdle()` and the prepare/transfer
         * workflow is safer. Directly calling `enableOutput()`/`disableOutput()` is 
         * provided for flexibility, but is NOT recommended unless you have verified 
         * that the external library does NOT also assert its own CS line directly (e.g.
         * `digitalWrite(CS, LOW)`). If external libs still use their own CS writes,
         * using `enableOutput()` can create races and unexpected device selects.
         */
        /** Enable the selected decoder output (active LOW) */
        void enableOutput();
        /** Disable the selected decoder output (active LOW) */
        void disableOutput();

        /** Release the bus and optionally apply a small settle delay */
        void goToIdle();
};

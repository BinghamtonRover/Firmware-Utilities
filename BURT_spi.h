#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <stdint.h>

/* 
Call Order:
```
if (BurtSPI.prepareTransaction(device_addr)) {
    device.func_that_uses_SPI(outEnable);
} else  {
 /.* Raise error flag *./
}
BurtSPI.goToIdle();
```

This will ensure that the bus is idle, then set up the correct virtual CS/addr pin
then call the external library's function with the outEnable pin so it can handle timing
then close the transaction and go back to idle
*/

/// @brief For use with 74hc4515 4:16 Decoder IC for Chip Select
/// @param csAddress array of 4 pins serving as addressing pins to the decoder
/// @param addressEnable enables the decoder address latch (ACTIVE LOW)
/// @param outEnable enables the selected output (ACTIVE LOW)
/// @param delayUs small delay after successful transaction, pass 0 to disable (keep below 10us)
class BurtSPI {
private:
    uint8_t csAddress[4];
    uint8_t addressEnable;
    uint8_t outEnable;
    
    unsigned delayUs;

    bool idle = false;
public:
    BurtSPI(const uint8_t (&csAddrPins)[4],
            uint8_t addressEnable,
            uint8_t outEnable,
            unsigned delayUs);

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

#include "BURT_timer.h"

void turnOffLed();

class BurtErrorLed {
    private: 
      int pinNum;
      BurtTimer timer;
      
    public: 
        BurtErrorLed(int pinNum);
        void setup();
        void update();
        void turnOn();
};
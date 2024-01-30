#include "types.h"

class BurtTimer {
  private: 
    int interval;
    VoidCallback callback;
    unsigned long nextTime;
    void reset();

  public: 
    BurtTimer(int interval, VoidCallback callback);
    void setup();
    void update();
};

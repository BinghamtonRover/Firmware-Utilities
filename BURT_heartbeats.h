#include "types.h"
#include "BURT_timer.h"

typedef struct {
  int heartbeatCheckInterval = 500;
} BurtHeartbeatConfig;

class BurtHeartbeats {
  private: 
    BurtHeartbeatConfig config;
    VoidCallback onDisconnect;
    BurtTimer checkTimer;
    bool gotHeartbeat = false;
    void checkForHeartbeats();

  public: 
    BurtHeartbeats(VoidCallback onDisconnect);
    BurtHeartbeats(VoidCallback onDisconnect, BurtHeartbeatConfig config);
    void setup();
    void update();
    void registerHeartbeat();
};

#include "BURT_heartbeats.h"

void BurtHeartbeats::setup() {
  checkTimer = BurtTimer(config.heartbeatCheckInterval, checkForHeartbeats);
}

void BurtHeartbeats::update() {
  checkTimer.update();
}

void BurtHeartbeats::registerHeartbeat() {
  gotHeartbeat = true;
}

void BurtHeartbeats::checkForHeartbeats() {
  if (!gotHeartbeat) onDisconnect();
}


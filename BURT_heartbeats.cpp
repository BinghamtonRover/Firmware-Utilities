#include "BURT_heartbeats.h"

BurtHeartbeats::BurtHeartbeats(VoidCallback onDisconnect) { }

BurtHeartbeats::BurtHeartbeats(VoidCallback onDisconnect, BurtHeartbeatConfig config) { }

void BurtHeartbeats::setup() {
  VoidCallback callback = [this]() { this->checkForHeartbeats(); };
  checkTimer = BurtTimer(config.heartbeatCheckInterval, callback);
  checkTimer.setup();
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


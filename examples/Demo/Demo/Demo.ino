#include <BURT_utils.h>

#include "src/BURT_methane.h"
#include "src/science.pb.h"

// See the CAN repository for details
#define SCIENCE_DATA_ID 0x27
#define SCIENCE_COMMAND_ID 0x83

#define METHANE_PIN 12

// The data to send. Will change over time.
ScienceData data;

MethaneSensor methaneSensor(METHANE_PIN);
BurtSerial serial(handleScienceCommand);
BurtCan can(SCIENCE_COMMAND_ID, handleScienceCommand);

void handleScienceCommand(const uint8_t* data, int length) {
  // The 3rd parameter is always MessageName_fields
  auto command = BurtProto::decode<ScienceCommand>(data, length, ScienceCommand_fields);
  if (command.dig) Serial.println("Digging!");
}

void sendData() { 
  data.methane++;
  if (data.methane >= 50) data.methane = 0; 
  // The 3rd parameter is always MessageName_fields
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);
}

void setup() {
  Serial.begin(9600);
  can.setup();
}

void loop() {
  can.update();
  serial.update();
  
  sendData();
  delay(10);
}

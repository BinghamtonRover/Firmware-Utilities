#pragma once

#include "BURT_can.h"
#include "BURT_proto.h"
#include "BURT_serial.h"
#include "BURT_timer.h"

// Utility methods for printing only when debug mode is defined,
// this will prevent any text from being written to Serial when
// running on the Rover
//
// In order to have messages print to Serial, add at the top of your .ino file:
// #define DEBUG_MODE
#ifdef DEBUG_MODE
#define DebugPrint(message) Serial.print(message)
#define DebugPrintln(message) Serial.println(message)
#define DebugPrintHEX(message) Serial.print(message, HEX)
#else
#define DebugPrint(message)
#define DebugPrintln(message)
#define DebugPrintHEX(message)
#endif

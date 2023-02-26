# Firmware Integration Sketch

This example is a demonstration on how to integrate hardware code, Protobuf, CAN, and Serial in your Arduino sketch, based on our [Firmware-Utilities](https://github.com/BinghamtonRover/Firmware-Utilities) library (this repository). The rest of this document will specify the structure and reasoning exactly, and the code can serve as a demonstration of following these instructions.

This example contains a sketch called `Demo`. Adapt the commands below to your firmware sketch's name. 

## Hardware code

Often, hardware-specific code can get complex and unwieldly, and is better relegated to separate C++ files. The same is true for overly complex logic. Try to reserve the `.ino` file for high-level logic, and relegate the low-level implementation to other C++ files. The [Arduino sketch specification](https://arduino.github.io/arduino-cli/0.20/sketch-specification/#additional-code-files) states that all code inside your sketch's `src` folder (eg, `Demo/src`) are compiled, so we’ll put them there and `#include` them later. For example, say you're writing code for the EA team – you might want a `MethaneSensor` class: 

```cpp
// Demo/src/methane.h
#include <Arduino.h>  // needed for Arduino code, like Serial

class MethaneSensor {
  private: 
    int pin;
  
  public:
    MethaneSensor(int pin) : pin(pin) { }
    float read();
};
```
```cpp
// Demo/src/methane.cpp
#include "methane.h"

float MethaneSensor::read() { 
  return analogRead(pin);  // insert real logic here
}
```
And then simply include it in your sketch: 
```cpp
// Demo/Demo.ino
#include "src/methane.h"

#define METHANE_PIN 12

MethaneSensor methaneSensor(METHANE_PIN);

void sendData() { 
  float methane = methaneSensor.read(); 
  // ...
}
```

The goal is to make the `.ino` file more readable, and no hardware-specific logic should be there. If you need a lot of hardware code, you can organize them in subdirectories under `src` and bundle them into one higher-level header file:

```cpp
// Demo/science.h
#include "src/methane/methane.h"
#include "src/humidity/humidity.h"
// ...
```

```cpp
// Demo/Demo.ino
#include "science.h"
```

## Including the Firmware Utilities

Clone the [Firmware Utilities](https://github.com/BinghamtonRover/Firmware-Utilities) repository into a new folder (not in your sketch), and run the `sync.sh` or `sync.bat` script to install it into your Arduino libraries folder.

### Using Protobuf files and the `BURT_proto` library

Integrating Protobuf into your code is a 3-step process: 

#### Including the `.proto` files

Protobuf defines data in its own language, in a file with the `.proto` extension. We keep our Protobuf files in a [separate repository](https://github.com/BinghamtonRover/Protobuf), so you'll first need to include that in your code. 

To keep our Protobuf files in sync, add them as a submodule: 
```bash
# Clones the Protobuf repository into a new Protobuf folder. They won't be compiled
git submodule add https://github.com/BinghamtonRover/Protobuf
```
If you clone this repository, make sure you run
```bash
git submodule update --init
```
To update your Protobuf files in the future, you can either `cd` into it and `git pull` as normal, or run this command: 
```bash
git submodule update --remote
```

#### Using the Protobuf files

First, be sure to follow the instructions in the Firmware-Utilities repository to install `protoc` and `nanopb`. Now you can generate the C++ code for your `.proto` files. These files will be stored in your sketch's `src` folder -- which compiles _everything_ -- so you only want to generate the files you need. Continuing the science example, say you want to generate `science.proto`: 

To generate Protobuf code: 
```bash
nanopb_generator -I Protobuf -D Demo/src science.proto
```

This generates a file `Demo/src/science.pb.h`. Now, in your sketch: 
```cpp
// Demo/Demo.ino
#include "src/science.pb.h"
```

You now have access to all the data defined in the Proto files. Whatever is declared as a `message` in there will be generated as a `struct` in the `.pb.h` files. When writing code, use the `.proto` files as a reference -- _not_ the structs.

You should not need to encode Protobuf messages directly -- the `BURT_can` and `BURT_serial` libraries handle this for you. But you will need to _decode_ messages yourself, since you need to know ahead of time which message type to use. 

The `decode<T>` function will be explained below, in the context of decoding a CAN message.

### Using the `BURT_can` library.

The Firmware-Utilities repository has details on how CAN bus works, so this section focuses on how to _use_ the library. 

#### Initialization

The `BURT_can` library defines a class, `BurtCan`, which can send and receive messages over CAN. To start, include the library, and initialize CAN on startup and update it on every loop. 

```cpp
// Demo/Demo.ino
#include <BURT_can.h>

void setup() {
  Serial.begin(9600);
  BurtCan::setup();
  // ...
}

void loop() {
  BurtCan::update();
  // ...
}
```

#### Sending messages

You can easily send a Protobuf message using the `send` function. Say you want to send some Protobuf-generated struct `ScienceData`: 

```cpp
#include "src/methane.h"
#include "src/science.pb.h"

// See the CAN repository for details
#define SCIENCE_DATA_ID 0x27  

#define METHANE_PIN 12

MethaneSensor methaneSensor(12);

void sendData() {
  ScienceData data;
  data.methane = methaneSensor.read();
  // The 2nd parameter is always MessageName_fields
  BurtCan::send(SCIENCE_DATA_ID, ScienceData_fields, &data);
}

void loop() {
  BurtCan::update();
  sendData();
  delay(10);
}
```

#### Receiving messages

You can define a callback to run when a new CAN frame of a specific ID is received. You have to decode it as the correct Protobuf message yourself, based on the ID. Here's an example of parsing a `ScienceCommand` from a CAN message with ID `0xC3`: 

```cpp
#include <BURT_proto.h>
#include "src/science.pb.h"

// See the CAN repository for details
#define SCIENCE_COMMAND_ID 0x83

// This is a [ProtoHandler] -- your function's parameters must match.
void handleScienceCommand(const uint8_t* data, int length) {
  // The 2nd parameter is always MessageName_fields
  auto command = BurtProto::decode<ScienceCommand>(data, length, ScienceCommand_fields);
  if (command.dig) Serial.println("Digging!");
}

void setup() {
  Serial.begin(9600);
  BurtCan::setup();
  BurtCan::registerHandler(SCIENCE_COMMAND_ID, handleScienceCommand);
  // more handlers here...
}
```

Now `handleScienceCommand` will run whenever a new CAN frame is received with ID `0x83`, decode that frame as a `ScienceCommand` instance, and check whether it should dig.

### Using the `BURT_serial` library

Similarly, we want the `ScienceCommand` handler to run whenever a command is sent over _Serial_, as well. To make this happen, simply register the handler using the `BURT_serial` library as well, and check for updates in `loop`: 

```cpp
#include <BURT_serial.h>

BurtSerial serial(handleCommand);

void loop() {
  BurtCan::update();
  sendData();
  serial.parseSerial();
  delay(10);  // don't want to overload Serial or CAN
}
```

## Summary
The above was a step-by-step process of writing a firmware sketch. The full code can be found in `Demo/Demo.ino`, and it implements the following features: 

- Reads methane levels from a sensor on pin 12
- Sends a `ScienceData` message over CAN with ID `0x27`
- Checks for a `ScienceCommand` on CAN ID `0xC3` and Serial

You can also simply include `BURT_utils.h` to get all three of the above libraries.

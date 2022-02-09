# Burt CAN Bus

Provides common CAN bus functionality for the Teensy controllers aboard the rover using the [FlexCAN_T4](https://github.com/tonton81/FlexCAN_T4) library. Make sure to install that before using it

## Setup

Arduino can only include libraries that are in one of the following locations: 

1. The same directory as the sketch including it
2. The user’s library directory
3. The global library directory

Since these libraries need to be included in a number of sketches, location 1 is not an option. Option 3 is cumbersome since it requires root privileges. So you want to store this repository in your **user-specific** Arduino library directory. Check your Arduino IDE for your sketchbook directory, and put this (and `FlexCAN_T4`) in the `libraries` folder inside there.

## Usage

First, include the library in your code: 

```cpp
#include <BURT_can.h>
```

Next, you need to initialize the library: 

```cpp
void setup() {
  // ...
  BurtCan::setup();
}
```

### Receiving messages

CAN works best via callbacks instead of checking for messages. To simplify message handling, CAN uses “mailboxes” that each pick up messages with a certain ID. The ID, in our case, is a combination of the device being targeted and the command it is being sent. Thus, whenever a CAN message is sent, exactly one handler should run to execute the command, without parsing the command.

Your handler function needs to be a `CanHandler`: `const CanMessage&` as input and `void` output. `CanMessage`s have their ID in `message.id` and data in `message.buf`.

```cpp
void rotate(const CanMessage& message) { 
  Serial.println("Rotating motor 1 by: " + message.buf[0]);
  Serial.println("Rotating motor 2 by: " + message.buf[1]);
}
```

Register your handler in `setup`: 

```cpp
#define ROTATE_ID 5

void setup() {
  // ...
  BurtCan::setup();
  BurtCan::registerHandler(ROTATE_ID, rotate);
}
```

Be sure to listen for incoming messages in `loop`:

```cpp
void loop() {
  // ...
  BurtCan::update();
}
```

### Sending messages

To send a message of bytes, use `send`:

```cpp
uint8_t[] arguments = {1, 2, 3}  // angles, in radians

void loop() {
  BurtCan::send(ROTATE_ID, arguments);
  delay(1000);
}
```


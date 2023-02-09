### Setup
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

See the sketches in the `examples` directory for examples.

### Receiving messages

CAN works best via callbacks instead of checking for messages. To simplify message handling, CAN uses “mailboxes” that each pick up messages with a certain ID. The ID, in our case, is a combination of the device being targeted and the command it is being sent. Thus, whenever a CAN message is received, exactly one handler should run to execute the command.

Your handler function needs to be a `CanHandler`: `const CanMessage&` as input and `void` output. `CanMessage`s have their ID in `message.id` and data in `message.buf`.

```cpp
void handler(const CanMessage& message) { 
  Serial.print("Received signal with data: ");
  for (int index = 0; index < 8; index++) {
    Serial.print(message.buf[index]);
    Serial.print(" ");
  }
  Serial.print("\n");
}
```

Register your handler in `setup`: 

```cpp
#define SIGNAL_ID 1

void setup() {
  // ...
  BurtCan::setup();
  BurtCan::registerHandler(SIGNAL_ID, handler);
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
uint8_t data[] = {1, 2, 3, 4, 5, 6, 7, 8};

void loop() {
  BurtCan::send(SIGNAL_ID, data);
  delay(1000);
}
```

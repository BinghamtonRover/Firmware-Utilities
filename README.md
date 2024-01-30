# Firmware Utilities

This repository contains all the common code you'll need to write firmware sketches for the rover. 

- **Protobuf**: All the rover's data and commands are serialized using Google's [Protocol Buffers](https://protobuf.dev/)
- **CAN bus**: The Teensy boards communicate with the Raspberry Pi using the [CAN bus protocol](https://www.csselectronics.com/pages/can-bus-simple-intro-tutorial)
- **Serial**: While the firmware are supposed to be controlled by the Raspberry Pi, they can also be unit tested with the dashboard by passing Protobuf messages over a USB Serial connection.

Each library is documented, required dependencies are bundled for your convenience, and there is a full example combining everything you'll need in the [Firmware-Template](https://github.com/BinghamtonRover/Firmware-Template) repository.

To install and use this library, run `sync.sh` (or `sync.bat` on Windows) with the path of your Arduino Sketchbook directory. You can find this by going into `File > Preferences` in your Arduino IDE. That script will create a [hard link](https://stackoverflow.com/q/185899/9392211) to this repository in your Arduino installation's [libraries](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries#manual-installation) directory. 

## Protobuf 

Just as JSON can turn a C++ struct or a Python class into a language-agnostic string that can be passed between programs, Protobuf also encodes and decodes data between languages. The advantages of Protobuf over JSON are: 

1. A much smaller format. Protobuf only encodes the fields it needs, whereas JSON encodes every field. Also, Protobuf is not designed to be human-readable, which means it doesn't have to waste space on characters like `{`, `"`, or `,`.
2. Protobuf data is defined in a special `.proto` file, and the Protobuf compiler, `protoc`, automatically generates classes or structs in the language of your choice, with encoding/decoding methods for you. 
3. Because code is generated for you, you never have to worry about keeping your data in sync between languages, or whether your encoding/decoding logic is sound. 

You'll need to install Google's Protobuf compiler [here](https://github.com/protocolbuffers/protobuf/releases/latest). For C++, you only need to download the `protoc` files for your platform -- the runtime libraries are bundled here for you. Once downloaded, extract the files and put them in your PATH.

The original Protobuf compiler is not fit for use within Arduino. Instead, we will use a different compiler called `nanopb`.

```bash
# Make sure you have Python before running this command
python -m pip install nanopb
```

All our Protobuf files are stored within the [Protobuf](https://github.com/BinghamtonRover/Protobuf) repository.

## CAN Bus

Protobuf defines a format for how to represent our data, but we still need a method of sending that data. The dashboard uses a UDP connection over Ethernet, but the Teensy boards are too small for that. Instead, we use the widely-popular Controller Area Network (CAN), specifically designed for use within vehicles. The basic idea of a CAN bus is that all devices are connected to the same network (along the “bus”), and each message is packaged with an ID that specifies the intended recipient and purpose. 

The link above is very user-friendly; a more technical document can be found [here](https://www.ti.com/lit/an/sloa101b/sloa101b.pdf). Unfortunately, CAN only allows 8 bytes per packet, which is too small to accommodate most useful Protobuf messages. We are hoping to switch to [CAN FD](https://www.csselectronics.com/pages/can-fd-flexible-data-rate-intro), which allows up to 64 bytes per packet, which is more than enough to fit all our Protobuf messages.

## Serial

Writing custom C++ code to test each aspect of the firmware can get tiresome, and it may difficult for non-software members to set up our libraries, repositories, and submodules. Instead, we’ll use the Dashboard to send commands to the Teensy boards directly over Serial. The `BURT_serial` library allows you to handle any incoming commands by specifying a callback function to handle it. You don’t have to be connected to the Dashboard to run your code – you can still use the `BURT_can` library to receive commands as normal. But you won’t be able to use the Serial Monitor when using this library, either. 

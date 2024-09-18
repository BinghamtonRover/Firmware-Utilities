#include <Arduino.h>
#include <functional>

using ProtoHandler = void (*)(const uint8_t* buffer, int length);

// using VoidCallback = void (*)();
using VoidCallback = std::function<void()>;

#include <Arduino.h>
#include <functional>

using ProtoHandler = void (*)(const uint8_t* buffer, int length);

using VoidCallback = std::function<void()>;

// ionOS v1.0 - Arduino IDE wrapper
// This sketch allows using ionOS with the Arduino IDE by forwarding to
// the main implementation in src/main.cpp (Kernel, drivers, apps, etc.).

#include <Arduino.h>

// Forward declarations to match the signatures in src/main.cpp.
// The Arduino build system will link these to the real implementations.
void setup();
void loop();

// Nothing else is required here.
// Place all core logic in src/main.cpp and the ionOS source tree,
// and keep this file as a thin wrapper so Arduino IDE compiles it correctly.

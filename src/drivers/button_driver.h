#ifndef IONOS_BUTTON_DRIVER_H
#define IONOS_BUTTON_DRIVER_H

#include <stdint.h>
#include <Arduino.h>
#include "../config/pinmap.h"

// ============================================================================
// ionOS v1.0 - BUTTON DRIVER
// Debounced input handling for 6-button layout
// UP, DOWN, LEFT, RIGHT, SELECT, BACK
// ============================================================================

enum ButtonState {
    BTN_STATE_RELEASED = 0,
    BTN_STATE_PRESSED = 1,
    BTN_STATE_HELD = 2
};

enum ButtonEvent {
    BTN_EVENT_NONE = 0,
    BTN_EVENT_SHORT_PRESS = 1,
    BTN_EVENT_LONG_PRESS = 2,
    BTN_EVENT_RELEASE = 3
};

struct Button {
    uint8_t pin;
    uint8_t id;
    ButtonState state;
    ButtonEvent last_event;
    uint32_t press_time;
    uint32_t last_stable_time;
    bool debouncing;
};

class ButtonDriver {
public:
    // Initialization
    static bool init();
    static void shutdown();

    // Button polling (call from main loop)
    static void update();

    // Button state queries
    static ButtonState getButtonState(uint8_t btn_id);
    static ButtonEvent getLastEvent(uint8_t btn_id);
    static uint32_t getPressTime(uint8_t btn_id);

    // Check individual buttons
    static bool isPressed(uint8_t btn_id);
    static bool isHeld(uint8_t btn_id);
    static bool wasPressed(uint8_t btn_id);   // Returns true once after press
    static bool wasLongPressed(uint8_t btn_id);

    // All buttons at once
    static uint8_t getButtonCount();  // Returns 6

    // Debug
    static void printDebugInfo();
    static void printButtonStates();

private:
    static const uint8_t NUM_BUTTONS = 6;
    static Button buttons[NUM_BUTTONS];

    // Pin mapping (from pinmap.h)
    static const uint8_t button_pins[NUM_BUTTONS];

    // Debounce logic
    static void debounceButton(uint8_t btn_index);
    static void handleButtonPress(uint8_t btn_index);
    static void handleButtonRelease(uint8_t btn_index);
    static bool readPin(uint8_t pin);
};

#endif // IONOS_BUTTON_DRIVER_H

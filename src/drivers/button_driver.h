#ifndef IONOS_BUTTON_DRIVER_H
#define IONOS_BUTTON_DRIVER_H

#include <stdint.h>
#include <Arduino.h>

// ============================================================================
// ionOS v1.0 - BUTTON DRIVER
// Debounced button input with press/long-press detection
// ============================================================================

enum ButtonEvent {
    BTN_NONE = 0,
    BTN_SHORT_PRESS = 1,
    BTN_LONG_PRESS = 2,
    BTN_RELEASE = 3,
    BTN_REPEAT = 4
};

enum ButtonID {
    BTN_ID_UP = 0,
    BTN_ID_DOWN = 1,
    BTN_ID_LEFT = 2,
    BTN_ID_RIGHT = 3,
    BTN_ID_CENTER = 4,
    BTN_ID_BACK = 5,
    BTN_ID_MENU = 6,
    BTN_ID_VOL_UP = 7,
    BTN_ID_VOL_DOWN = 8,
    BTN_ID_MAX = 9
};

struct Button {
    uint8_t pin;
    uint8_t id;
    bool pressed;
    bool released;
    uint32_t press_time;
    uint32_t long_press_triggered;
};

class ButtonDriver {
public:
    // Initialization
    static bool init();
    static void shutdown();

    // Main update loop (call from kernel tick)
    static void update();

    // Event checking
    static ButtonEvent getEvent(ButtonID btn);
    static bool isPressed(ButtonID btn);
    static uint32_t getPressTime(ButtonID btn);

    // Debug
    static void printDebugInfo();

private:
    static Button buttons[BTN_ID_MAX];
    static ButtonEvent last_event[BTN_ID_MAX];

    static void debounceButton(Button &btn);
    static void updateButtonState(Button &btn);
};

#endif // IONOS_BUTTON_DRIVER_H

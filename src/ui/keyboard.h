#ifndef IONOS_KEYBOARD_H
#define IONOS_KEYBOARD_H

#include <stdint.h>
#include <Arduino.h>

// ============================================================================
// ionOS v1.0 - ON-SCREEN KEYBOARD
// 3-mode keyboard (lower, upper, symbols) with 6-button navigation
// Complete 26-letter QWERTY + symbols
// ============================================================================

enum KeyboardMode {
    MODE_LOWER = 0,
    MODE_UPPER = 1,
    MODE_SYMBOLS = 2
};

class Keyboard {
public:
    // Edit string in-place (returns true if accepted/ENTER, false if BACK/CANCEL)
    static bool editString(char *buffer, uint8_t max_len);
    
    // Get keyboard height (for UI layout)
    static uint8_t getHeight() { return 48; }
    
    // Debug
    static void printDebugInfo();

private:
    static KeyboardMode current_mode;
    static uint8_t cursor_row;
    static uint8_t cursor_col;
    
    static void renderKeyboard(KeyboardMode mode);
    static char getCharAt(KeyboardMode mode, uint8_t row, uint8_t col);
    static uint8_t getRowLength(KeyboardMode mode, uint8_t row);
    static void handleButton(uint8_t btn_id, char *buffer, uint8_t max_len, bool &done, bool &accepted);
};

#endif // IONOS_KEYBOARD_H

#include "keyboard.h"
#include "../drivers/display_driver.h"
#include "../drivers/button_driver.h"

KeyboardMode Keyboard::current_mode = MODE_LOWER;
uint8_t Keyboard::cursor_row = 0;
uint8_t Keyboard::cursor_col = 0;

// Keyboard layouts matching your exact ASCII design
static const char* LOWERCASE_KEYS[4] = {
    "qwerty",      // Row 0
    "asdfgh",      // Row 1  
    "zxcvbn",      // Row 2
    "␣⌫.@✔"        // Row 3 (shift, space, backspace, dot, at, enter)
};

static const char* UPPERCASE_KEYS[4] = {
    "QWERTY",      // Row 0
    "ASDFGH",      // Row 1
    "ZXCVBN",      // Row 2
    "␣⌫.@✔"        // Row 3 (same symbols)
};

static const char* SYMBOLS_KEYS[4] = {
    "123456",      // Row 0
    "!?#$%&",      // Row 1
    "()_-=+=",     // Row 2
    "abc␣⌫/:"      // Row 3 (abc=lower, space, backspace, /, :, enter)
};

bool Keyboard::editString(char *buffer, uint8_t max_len) {
    uint8_t len = strlen(buffer);
    bool done = false;
    bool accepted = false;
    
    cursor_row = 0;
    cursor_col = 0;
    current_mode = MODE_LOWER;

    while (!done) {
        DisplayDriver::clear();
        
        // Title bar
        DisplayDriver::drawString(2, 2, "TEXT INPUT", true);
        DisplayDriver::drawString(50, 2, buffer, true);
        DisplayDriver::drawLine(0, 10, 128, 10, true);
        
        // Render keyboard
        renderKeyboard(current_mode);
        
        // Instructions
        DisplayDriver::drawLine(0, 62, 128, 62, true);
        DisplayDriver::drawString(2, 64, "↑↓←→ NAV  SELECT=OK  BACK=CANCEL", false);
        
        // Button handling (non-blocking)
        ButtonDriver::update();
        for (uint8_t id = 0; id < ButtonDriver::getButtonCount(); id++) {
            if (ButtonDriver::wasPressed(id)) {
                handleButton(id);
                if (done) break;
            }
        }
    }
    
    return accepted;
}

void Keyboard::renderKeyboard(KeyboardMode mode) {
    const char** keys = (mode == MODE_LOWER) ? LOWERCASE_KEYS :
                       (mode == MODE_UPPER) ? UPPERCASE_KEYS : SYMBOLS_KEYS;
    
    // Draw keyboard border
    DisplayDriver::drawRect(1, 12, 126, 46, true, false);
    
    // Draw each key
    for (uint8_t row = 0; row < 4; row++) {
        uint8_t y = 16 + (row * 11);
        for (uint8_t col = 0; col < 6; col++) {
            char ch = getCharAt(mode, row, col);
            uint8_t x = 8 + (col * 20);
            
            bool selected = (row == cursor_row && col == cursor_col);
            if (selected) {
                // Highlight selected key
                DisplayDriver::drawRect(x-2, y-1, 22, 12, false, true);
                DisplayDriver::drawString(x, y+1, &ch, 1, false);  // Inverted
            } else {
                DisplayDriver::drawString(x, y+1, &ch, 1, true);
            }
        }
    }
    
    // Mode indicator
    const char* mode_names[] = {"LOWER", "UPPER", "SYMBOLS"};
    DisplayDriver::drawString(100, 13, mode_names[mode], true);
}

char Keyboard::getCharAt(KeyboardMode mode, uint8_t row, uint8_t col) {
    const char** keys = (mode == MODE_LOWER) ? LOWERCASE_KEYS :
                       (mode == MODE_UPPER) ? UPPERCASE_KEYS : SYMBOLS_KEYS;
    
    if (row < 4 && col < 6 && keys[row][col] != '') {
        return keys[row][col];
    }
    return ' ';
}

void Keyboard::handleButton(uint8_t btn_id) {
    uint8_t len = strlen(buffer);
    
    switch (btn_id) {
        case BTN_ID_UP:
            if (cursor_row > 0) cursor_row--;
            break;
            
        case BTN_ID_DOWN:
            if (cursor_row < 3) cursor_row++;
            break;
            
        case BTN_ID_LEFT:
            if (cursor_col > 0) cursor_col--;
            break;
            
        case BTN_ID_RIGHT:
            if (cursor_col < 5) cursor_col++;
            break;
            
        case BTN_ID_SELECT: {
            char ch = getCharAt(current_mode, cursor_row, cursor_col);
            
            if (ch == '␣') {  // Space
                if (len < max_len - 1) {
                    buffer[len++] = ' ';
                    buffer[len] = '';
                }
            } else if (ch == '⌫') {  // Backspace
                if (len > 0) buffer[--len] = '';
            } else if (ch == '✔') {  // Enter/Accept
                accepted = true;
                done = true;
            } else if (ch == '⇧') {  // Shift (cycle modes)
                current_mode = (KeyboardMode)((current_mode + 1) % 3);
                cursor_row = 0;
                cursor_col = 0;
            } else if (ch == 'abc') {  // Return to lowercase
                current_mode = MODE_LOWER;
                cursor_row = 0;
                cursor_col = 0;
            } else if (len < max_len - 1) {
                buffer[len++] = ch;
                buffer[len] = '';
            }
            break;
        }
        
        case BTN_ID_BACK:  // Cancel
            done = true;
            break;
    }
}

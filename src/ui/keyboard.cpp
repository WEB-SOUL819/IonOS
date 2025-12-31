#include "keyboard.h"
#include "../drivers/display_driver.h"
#include "../drivers/button_driver.h"

// ============================================================================
// Static member definitions
// ============================================================================
KeyboardMode Keyboard::current_mode = MODE_LOWER;
uint8_t Keyboard::cursor_row = 0;
uint8_t Keyboard::cursor_col = 0;

// ============================================================================
// COMPLETE KEYBOARD LAYOUTS (ALL 26 LETTERS)
// ============================================================================

// LOWERCASE: q w e r t y u i | a s d f g h j k | z x c v b n m l o p | â‡§ â£ âŒ« . @ âœ”
static const char LOWERCASE_KEYS[4][10] = {
    {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', '\0', '\0'},
    {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', '\0', '\0'},
    {'z', 'x', 'c', 'v', 'b', 'n', 'm', 'l', 'o', 'p'},
    {'\x1B', ' ', '\x08', '.', '@', '\x0D', '\0', '\0', '\0', '\0'}  // â‡§ â£ âŒ« . @ âœ”
};

// UPPERCASE: Q W E R T Y U I | A S D F G H J K | Z X C V B N M L O P | â‡§ â£ âŒ« . @ âœ”
static const char UPPERCASE_KEYS[4][10] = {
    {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', '\0', '\0'},
    {'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', '\0', '\0'},
    {'Z', 'X', 'C', 'V', 'B', 'N', 'M', 'L', 'O', 'P'},
    {'\x1B', ' ', '\x08', '.', '@', '\x0D', '\0', '\0', '\0', '\0'}  // â‡§ â£ âŒ« . @ âœ”
};

// SYMBOLS: 1 2 3 4 5 6 | ! @ # $ % ^ | & * ( ) _ + = - ; ' | abc â£ âŒ« / : âœ”
static const char SYMBOLS_KEYS[4][10] = {
    {'1', '2', '3', '4', '5', '6', '\0', '\0', '\0', '\0'},
    {'!', '@', '#', '$', '%', '^', '\0', '\0', '\0', '\0'},
    {'&', '*', '(', ')', '_', '+', '=', '-', ';', '\''},
    {'\x02', ' ', '\x08', '/', ':', '\x0D', '\0', '\0', '\0', '\0'}  // abc â£ âŒ« / : âœ”
};

// Row lengths for bounds checking
static const uint8_t ROW_LENGTHS[4] = {8, 8, 10, 6};

// ============================================================================
// Main keyboard edit function
// ============================================================================
bool Keyboard::editString(char *buffer, uint8_t max_len) {
    uint8_t len = strlen(buffer);
    bool done = false;
    bool accepted = false;
    
    // Initialize keyboard state
    current_mode = MODE_LOWER;
    cursor_row = 0;
    cursor_col = 0;

    while (!done) {
        // Render UI
        DisplayDriver::clear();
        
        // Title bar with input buffer
        DisplayDriver::drawString(2, 2, "INPUT:", true);
        DisplayDriver::drawString(40, 2, buffer, true);
        DisplayDriver::drawLine(0, 10, 128, 10, true);
        
        // Render keyboard
        renderKeyboard(current_mode);
        
        // Instructions
        DisplayDriver::drawLine(0, 54, 128, 54, true);
        DisplayDriver::drawString(1, 56, "UP/DN/LF/RT NAV  SELECT=OK  BACK=DEL", false);
        
        // Button handling (non-blocking with debounce)
        ButtonDriver::update();
        for (uint8_t id = 0; id < 6; id++) {
            if (ButtonDriver::wasPressed(id)) {
                handleButton(id, buffer, max_len, done, accepted);
                if (done) break;
            }
        }
        
        // Small delay for responsiveness
        delay(50);
    }
    
    return accepted;
}

// ============================================================================
// Render keyboard on display
// ============================================================================
void Keyboard::renderKeyboard(KeyboardMode mode) {
    // Draw keyboard border
    DisplayDriver::drawRect(2, 12, 124, 40, true, false);
    
    // Draw each key
    for (uint8_t row = 0; row < 4; row++) {
        uint8_t y = 16 + (row * 10);
        uint8_t row_len = ROW_LENGTHS[row];
        
        for (uint8_t col = 0; col < row_len; col++) {
            char ch = getCharAt(mode, row, col);
            if (ch == '\0') continue;
            
            // Calculate X position (evenly spaced)
            uint8_t key_width = 120 / row_len;
            uint8_t x = 4 + (col * key_width) + 2;
            
            bool selected = (row == cursor_row && col == cursor_col);
            
            if (selected) {
                // Highlight selected key with box
                DisplayDriver::drawRect(x - 2, y - 1, key_width - 2, 9, false, true);
            }
            
            // Draw key character
            char key_display[2] = {ch, '\0'};
            
            // Display special characters nicely
            if (ch == '\x1B') key_display[0] = 'S';     // â‡§ Shift
            else if (ch == ' ') key_display[0] = '_';   // â£ Space
            else if (ch == '\x08') key_display[0] = 'B'; // âŒ« Backspace
            else if (ch == '\x0D') key_display[0] = 'E'; // âœ” Enter
            else if (ch == '\x02') key_display[0] = 'a'; // abc
            
            if (selected) {
                DisplayDriver::drawString(x, y, key_display, false);
            } else {
                DisplayDriver::drawString(x, y, key_display, true);
            }
        }
    }
    
    // Draw mode indicator in top-right
    const char *mode_names[] = {"lower", "UPPER", "sym"};
    DisplayDriver::drawString(100, 13, mode_names[mode], true);
}

// ============================================================================
// Get character at position
// ============================================================================
char Keyboard::getCharAt(KeyboardMode mode, uint8_t row, uint8_t col) {
    if (row >= 4) return '\0';
    if (col >= ROW_LENGTHS[row]) return '\0';
    
    if (mode == MODE_LOWER) {
        return LOWERCASE_KEYS[row][col];
    } else if (mode == MODE_UPPER) {
        return UPPERCASE_KEYS[row][col];
    } else {
        return SYMBOLS_KEYS[row][col];
    }
}

// ============================================================================
// Get row length
// ============================================================================
uint8_t Keyboard::getRowLength(KeyboardMode mode, uint8_t row) {
    if (row >= 4) return 0;
    return ROW_LENGTHS[row];
}

// ============================================================================
// Handle button input
// ============================================================================
void Keyboard::handleButton(uint8_t btn_id, char *buffer, uint8_t max_len, bool &done, bool &accepted) {
    uint8_t len = strlen(buffer);
    
    switch (btn_id) {
        case BTN_ID_UP:
            if (cursor_row > 0) {
                cursor_row--;
                // Adjust column if new row is shorter
                if (cursor_col >= ROW_LENGTHS[cursor_row]) {
                    cursor_col = ROW_LENGTHS[cursor_row] - 1;
                }
            }
            break;
            
        case BTN_ID_DOWN:
            if (cursor_row < 3) {
                cursor_row++;
                // Adjust column if new row is shorter
                if (cursor_col >= ROW_LENGTHS[cursor_row]) {
                    cursor_col = ROW_LENGTHS[cursor_row] - 1;
                }
            }
            break;
            
        case BTN_ID_LEFT:
            if (cursor_col > 0) {
                cursor_col--;
            }
            break;
            
        case BTN_ID_RIGHT:
            if (cursor_col < ROW_LENGTHS[cursor_row] - 1) {
                cursor_col++;
            }
            break;
            
        case BTN_ID_SELECT: {
            char ch = getCharAt(current_mode, cursor_row, cursor_col);
            
            if (ch == '\x1B') {
                // Shift - cycle modes: lower -> upper -> symbols -> lower
                current_mode = (KeyboardMode)((current_mode + 1) % 3);
                cursor_row = 0;
                cursor_col = 0;
            } 
            else if (ch == ' ') {
                // Space
                if (len < max_len - 1) {
                    buffer[len++] = ' ';
                    buffer[len] = '\0';
                }
            }
            else if (ch == '\x08') {
                // Backspace
                if (len > 0) {
                    buffer[--len] = '\0';
                }
            }
            else if (ch == '\x0D') {
                // Enter/Accept
                accepted = true;
                done = true;
            }
            else if (ch == '\x02') {
                // abc - return to lowercase
                current_mode = MODE_LOWER;
                cursor_row = 0;
                cursor_col = 0;
            }
            else if (ch != '\0') {
                // Regular character
                if (len < max_len - 1) {
                    buffer[len++] = ch;
                    buffer[len] = '\0';
                }
            }
            break;
        }
        
        case BTN_ID_BACK:
            // Backspace (same as âŒ« key)
            if (len > 0) {
                buffer[--len] = '\0';
            } else {
                // Cancel if buffer is empty
                done = true;
                accepted = false;
            }
            break;
    }
}

// ============================================================================
// Debug info
// ============================================================================
void Keyboard::printDebugInfo() {
    Serial.println("\nâ•”â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•—");
    Serial.println("â•‘  KEYBOARD DEBUG INFO              â•‘");
    Serial.println("â• â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•£");
    Serial.printf("â•‘ Mode: %d (0=lower, 1=upper, 2=sym)\n", current_mode);
    Serial.printf("â•‘ Cursor: Row %d, Col %d\n", cursor_row, cursor_col);
    Serial.printf("â•‘ Row Length: %d\n", ROW_LENGTHS[cursor_row]);
    Serial.println("â•‘ Complete QWERTY: a-z all present  â•‘");
    Serial.println("â•šâ•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•\n");
}

#include "keyboard.h"
#include "../drivers/display_driver.h"
#include "../drivers/button_driver.h"

namespace {
    const char *KEY_ROWS[] = {
        "ABCDEF",
        "GHIJKL",
        "MNOPQR",
        "STUVWX",
        "YZ0123",
        "456789",
        " .-_/"
    };
    const uint8_t ROW_COUNT = 7;

    uint8_t cursor_row = 0;
    uint8_t cursor_col = 0;
}

namespace OnScreenKeyboard {

bool editString(char *buffer, uint8_t max_len) {
    uint8_t len = strlen(buffer);
    bool done = false;
    bool accepted = false;

    while (!done) {
        // Render keyboard
        DisplayDriver::clear();
        DisplayDriver::drawString(2, 2, "EDIT:", true);
        DisplayDriver::drawString(30, 2, buffer, true);
        DisplayDriver::drawLine(0, 10, 128, 10, true);

        for (uint8_t r = 0; r < ROW_COUNT; r++) {
            uint8_t y = 14 + r * 8;
            for (uint8_t c = 0; c < strlen(KEY_ROWS[r]); c++) {
                char ch[2] = { KEY_ROWS[r][c], '' };
                uint8_t x = 4 + c * 10;
                bool sel = (r == cursor_row && c == cursor_col);
                if (sel) {
                    DisplayDriver::drawRect(x - 1, y - 1, 9, 9, false, true);
                    DisplayDriver::drawString(x, y, ch, false);
                } else {
                    DisplayDriver::drawString(x, y, ch, true);
                }
            }
        }

        DisplayDriver::drawLine(0, 54, 128, 54, true);
        DisplayDriver::drawString(2, 56, "UP/DN/LF/RT, SEL=OK, BACK", false);

        // Read buttons (poll)
        ButtonDriver::update();
        for (uint8_t id = 0; id < ButtonDriver::getButtonCount(); id++) {
            if (ButtonDriver::wasPressed(id)) {
                switch (id) {
                    case BTN_ID_UP:
                        if (cursor_row > 0) cursor_row--;
                        break;
                    case BTN_ID_DOWN:
                        if (cursor_row + 1 < ROW_COUNT) cursor_row++;
                        break;
                    case BTN_ID_LEFT:
                        if (cursor_col > 0) cursor_col--;
                        break;
                    case BTN_ID_RIGHT:
                        if (cursor_col + 1 < strlen(KEY_ROWS[cursor_row])) cursor_col++;
                        break;
                    case BTN_ID_SELECT: {
                        char ch = KEY_ROWS[cursor_row][cursor_col];
                        if (len < max_len - 1) {
                            buffer[len++] = ch;
                            buffer[len] = '';
                        } else {
                            accepted = true;
                            done = true;
                        }
                        break;
                    }
                    case BTN_ID_BACK:
                        if (len > 0) {
                            buffer[--len] = '';
                        } else {
                            done = true;
                        }
                        break;
                }
            }
        }
    }

    return accepted;
}

} // namespace

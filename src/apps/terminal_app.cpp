#include "terminal_app.h"
#include "../drivers/display_driver.h"
#include "../drivers/button_driver.h"
#include "../core/kernel.h"
#include "../core/events.h"

TerminalApp::TerminalApp() :
    input_pos(0),
    history_lines(0),
    scroll_offset(0) {
    clearBuffer();
    for (uint8_t i = 0; i < HISTORY_LINES; i++) {
        history[i][0] = '';
    }
}

TerminalApp::~TerminalApp() {}

void TerminalApp::onLaunch() {
    state = APP_STATE_ACTIVE;
    clearBuffer();
    history_lines = 0;
    scroll_offset = 0;
    printOutput("ionOS Terminal");
    printOutput("Type 'help' and press SELECT");
}

void TerminalApp::onClose() {
    state = APP_STATE_CLOSING;
}

void TerminalApp::onEvent(const Event &event) {
    if (event.type != EVENT_BUTTON_PRESS) return;

    switch (event.data1) {
        case BTN_ID_UP:
            if (scroll_offset + 1 < history_lines) {
                scroll_offset++;
            }
            break;
        case BTN_ID_DOWN:
            if (scroll_offset > 0) {
                scroll_offset--;
            }
            break;
        case BTN_ID_LEFT:
            if (input_pos > 0) {
                input_pos--;
                input_buffer[input_pos] = '';
            }
            break;
        case BTN_ID_RIGHT:
            // No-op for now, reserved for future cursor movement
            break;
        case BTN_ID_SELECT:
            executeCommand();
            break;
        case BTN_ID_BACK:
            handleBackButton();
            break;
    }
}

void TerminalApp::update() {
    // Nothing time-based yet
}

void TerminalApp::render() {
    DisplayDriver::clear();

    // Draw title
    DisplayDriver::drawString(30, 2, "TERMINAL", true);
    DisplayDriver::drawLine(0, 10, 128, 10, true);

    // Draw history (from bottom up)
    int16_t y = 50;
    int16_t printed = 0;
    for (int16_t i = history_lines - 1 - scroll_offset; i >= 0 && printed < HISTORY_LINES; i--) {
        DisplayDriver::drawString(2, y, history[i], true);
        y -= 8;
        printed++;
    }

    // Draw input line
    DisplayDriver::drawLine(0, 54, 128, 54, true);
    char line[40];
    snprintf(line, sizeof(line), "> %s", input_buffer);
    DisplayDriver::drawString(2, 56, line, true);
}

void TerminalApp::executeCommand() {
    if (input_pos == 0) return;

    // Add to history
    printOutput(input_buffer);

    // Simple commands
    if (strcmp(input_buffer, "help") == 0) {
        printOutput("Commands: help, clear, info");
    } else if (strcmp(input_buffer, "clear") == 0) {
        history_lines = 0;
        scroll_offset = 0;
    } else if (strcmp(input_buffer, "info") == 0) {
        printOutput("ionOS v1.0 - ESP32");
    } else {
        printOutput("Unknown command");
    }

    clearBuffer();
}

void TerminalApp::printOutput(const char *text) {
    if (history_lines < HISTORY_LINES) {
        strncpy(history[history_lines], text, BUFFER_SIZE - 1);
        history[history_lines][BUFFER_SIZE - 1] = '';
        history_lines++;
    } else {
        // Scroll up
        for (uint8_t i = 0; i < HISTORY_LINES - 1; i++) {
            strncpy(history[i], history[i + 1], BUFFER_SIZE);
        }
        strncpy(history[HISTORY_LINES - 1], text, BUFFER_SIZE - 1);
        history[HISTORY_LINES - 1][BUFFER_SIZE - 1] = '';
    }
}

void TerminalApp::clearBuffer() {
    memset(input_buffer, 0, sizeof(input_buffer));
    input_pos = 0;
}

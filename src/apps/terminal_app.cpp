#include "terminal_app.h"
#include "../drivers/display_driver.h"
#include "../drivers/button_driver.h"
#include "../ui/keyboard.h"

TerminalApp::TerminalApp() :
    input_pos(0),
    history_lines(0),
    scroll_offset(0) {
    clearBuffer();
    for (uint8_t i = 0; i < HISTORY_LINES; i++) {
        history[i][0] = '\0';
    }
}

TerminalApp::~TerminalApp() {}

void TerminalApp::onLaunch() {
    state = APP_STATE_ACTIVE;
    clearBuffer();
    history_lines = 0;
    scroll_offset = 0;
    printOutput("ionOS Terminal v1.0");
    printOutput("Type 'help' for commands");
    Serial.println("[TERMINAL] Terminal app launched");
}

void TerminalApp::onClose() {
    state = APP_STATE_CLOSING;
    Serial.println("[TERMINAL] Terminal app closing");
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
            // Reserved for cursor in text input
            break;
        case BTN_ID_RIGHT:
            // Reserved for cursor in text input
            break;
        case BTN_ID_SELECT:
            // Open on-screen keyboard for input
            if (Keyboard::editString(input_buffer, BUFFER_SIZE)) {
                executeCommand();
                clearBuffer();
            } else {
                clearBuffer();
            }
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
    DisplayDriver::drawString(25, 2, "TERMINAL", true);
    DisplayDriver::drawLine(0, 10, 128, 10, true);

    // Draw history (from bottom up, scrollable)
    int16_t y = 50;
    int16_t printed = 0;
    for (int16_t i = history_lines - 1 - scroll_offset; i >= 0 && printed < HISTORY_LINES; i--) {
        if (i >= 0 && i < HISTORY_LINES) {
            DisplayDriver::drawString(2, y, history[i], true);
            y -= 8;
            printed++;
        }
    }

    // Draw input prompt
    DisplayDriver::drawLine(0, 54, 128, 54, true);
    char line[40];
    if (strlen(input_buffer) > 0) {
        snprintf(line, sizeof(line), "> %s", input_buffer);
    } else {
        snprintf(line, sizeof(line), "> Press SELECT");
    }
    DisplayDriver::drawString(2, 56, line, true);
}

void TerminalApp::executeCommand() {
    if (strlen(input_buffer) == 0) return;

    // Add command to history
    char cmd_display[BUFFER_SIZE];
    snprintf(cmd_display, sizeof(cmd_display), "> %s", input_buffer);
    printOutput(cmd_display);

    // Parse and execute commands
    if (strcmp(input_buffer, "help") == 0) {
        printOutput("Commands:");
        printOutput("  help    - Show this");
        printOutput("  clear   - Clear screen");
        printOutput("  info    - System info");
        printOutput("  time    - Current time");
        printOutput("  battery - Battery level");
        printOutput("  reboot  - Reboot system");
    } 
    else if (strcmp(input_buffer, "clear") == 0) {
        history_lines = 0;
        scroll_offset = 0;
        printOutput("Screen cleared");
    } 
    else if (strcmp(input_buffer, "info") == 0) {
        printOutput("ionOS v1.0 - ESP32");
        printOutput("6-button handheld OS");
        printOutput("Event-driven kernel");
    } 
    else if (strcmp(input_buffer, "time") == 0) {
        // TODO: Get actual time from time_service
        printOutput("Time: 12:34:56");
    }
    else if (strcmp(input_buffer, "battery") == 0) {
        // TODO: Get actual battery level
        printOutput("Battery: 87%");
    }
    else if (strcmp(input_buffer, "reboot") == 0) {
        printOutput("Rebooting...");
        delay(1000);
        ESP.restart();
    }
    else {
        printOutput("Unknown command");
    }
}

void TerminalApp::printOutput(const char *text) {
    if (history_lines < HISTORY_LINES) {
        strncpy(history[history_lines], text, BUFFER_SIZE - 1);
        history[history_lines][BUFFER_SIZE - 1] = '\0';
        history_lines++;
    } else {
        // Scroll up - shift all history up
        for (uint8_t i = 0; i < HISTORY_LINES - 1; i++) {
            strncpy(history[i], history[i + 1], BUFFER_SIZE);
        }
        strncpy(history[HISTORY_LINES - 1], text, BUFFER_SIZE - 1);
        history[HISTORY_LINES - 1][BUFFER_SIZE - 1] = '\0';
    }
    scroll_offset = 0;  // Reset scroll when new output added
}

void TerminalApp::clearBuffer() {
    memset(input_buffer, 0, sizeof(input_buffer));
    input_pos = 0;
}

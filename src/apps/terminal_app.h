#ifndef IONOS_TERMINAL_APP_H
#define IONOS_TERMINAL_APP_H

#include "../apps/app_base.h"

// ============================================================================
// ionOS v1.0 - TERMINAL APP
// Simple command-line interface for debugging
// ============================================================================

class TerminalApp : public App {
public:
    TerminalApp();
    ~TerminalApp();

    void onLaunch() override;
    void onClose() override;
    void onEvent(const Event &event) override;
    void update() override;
    void render() override;
    const char* getName() override { return "Terminal"; }

private:
    static const uint8_t BUFFER_SIZE = 32;
    static const uint8_t HISTORY_LINES = 8;

    char input_buffer[BUFFER_SIZE];
    uint8_t input_pos;
    char history[HISTORY_LINES][BUFFER_SIZE];
    uint8_t history_lines;
    uint8_t scroll_offset;

    // Command execution
    void executeCommand();
    void printOutput(const char *text);
    void clearBuffer();
};

#endif // IONOS_TERMINAL_APP_H

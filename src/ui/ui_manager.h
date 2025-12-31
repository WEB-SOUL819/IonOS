#ifndef IONOS_UI_MANAGER_H
#define IONOS_UI_MANAGER_H

#include <stdint.h>
#include <Arduino.h>
#include "../core/events.h"

// ============================================================================
// ionOS v1.0 - UI MANAGER
// Display state machine, screen transitions, layout management
// ============================================================================

enum UIScreen {
    SCREEN_NONE = 0,
    SCREEN_LAUNCHER = 1,
    SCREEN_CLOCK = 2,
    SCREEN_SETTINGS = 3,
    SCREEN_TERMINAL = 4,
    SCREEN_MUSIC = 5,
    SCREEN_GAME_SNAKE = 6,
    SCREEN_GAME_TREX = 7,
    SCREEN_STATUS = 8,
    SCREEN_LOADING = 9,
    SCREEN_ERROR = 10
};

enum UITransition {
    TRANSITION_NONE = 0,
    TRANSITION_SLIDE_LEFT = 1,
    TRANSITION_SLIDE_RIGHT = 2,
    TRANSITION_SLIDE_UP = 3,
    TRANSITION_SLIDE_DOWN = 4,
    TRANSITION_FADE = 5,
    TRANSITION_ZOOM_IN = 6,
    TRANSITION_ZOOM_OUT = 7
};

class UIManager {
public:
    // Initialization
    static bool init();
    static void shutdown();

    // Screen management
    static bool switchScreen(UIScreen screen, UITransition transition = TRANSITION_FADE);
    static UIScreen getCurrentScreen();
    static void goBack();

    // Navigation
    static void setScreenStack(const UIScreen *screens, uint8_t count);
    static bool canGoBack();

    // Rendering
    static void update();
    static void render();
    static void markDirty();  // Force redraw

    // Status bar control
    static void showStatusBar(bool show);
    static bool isStatusBarVisible();

    // Theme/style
    static void setTheme(uint8_t theme_id);
    static uint8_t getTheme();

    // Debug
    static void printDebugInfo();

private:
    static UIScreen current_screen;
    static UIScreen previous_screen;
    static UITransition active_transition;
    static uint32_t transition_start_time;
    static uint32_t transition_duration;

    static const uint8_t MAX_STACK_DEPTH = 5;
    static UIScreen screen_stack[MAX_STACK_DEPTH];
    static uint8_t stack_depth;

    static bool status_bar_visible;
    static bool dirty;
    static uint8_t current_theme;

    // Internal methods
    static void updateTransition();
    static void renderTransition();
};

#endif // IONOS_UI_MANAGER_H

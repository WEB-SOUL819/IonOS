#ifndef IONOS_KERNEL_H
#define IONOS_KERNEL_H

#include <stdint.h>
#include <Arduino.h>
#include "events.h"

// ============================================================================
// ionOS v1.0 - KERNEL (Core Event Loop & App Manager)
// ============================================================================

class App;  // Forward declaration

enum AppState {
    APP_STATE_INACTIVE = 0,
    APP_STATE_RUNNING = 1,
    APP_STATE_SUSPENDED = 2,
    APP_STATE_PAUSED = 3
};

struct AppInstance {
    App *app;
    AppState state;
    uint32_t launch_time;
    uint8_t app_id;
};

class Kernel {
public:
    // Initialization
    static bool init();
    static bool startup();
    static void shutdown();
    static bool isRunning();

    // Main kernel loop (call from main)
    static void tick();
    static void run();  // Blocking main loop

    // App management
    static bool launchApp(App *app, uint8_t app_id);
    static bool closeApp(uint8_t app_id);
    static App* getActiveApp();
    static uint8_t getActiveAppID();

    // Event handling
    static bool postEvent(const Event &event);
    static bool processEvent(Event &event);
    static uint8_t getEventQueueSize();

    // System stats
    static uint32_t getTickCount();
    static uint32_t getTickFrequency();
    static uint32_t getLoopTime();
    static float getLoopFrequency();

    // Debug
    static void printDebugInfo();
    static void printMemoryInfo();

private:
    static bool running;
    static bool initialized;

    static const uint8_t MAX_APPS = 10;
    static AppInstance apps[MAX_APPS];
    static uint8_t active_app_id;
    static uint32_t tick_count;
    static uint32_t last_loop_time;
    static uint32_t loop_start_time;

    // Internal methods
    static void handleButtonEvents();
    static void handlePowerEvents();
    static void updateApps();
    static void renderDisplay();
    static void manageMemory();
};

#endif // IONOS_KERNEL_H

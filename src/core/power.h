#ifndef IONOS_POWER_H
#define IONOS_POWER_H

#include <stdint.h>
#include <Arduino.h>

// ============================================================================
// ionOS v1.0 - POWER MANAGEMENT
// Sleep modes, wake sources, power control
// ============================================================================

enum PowerMode {
    POWER_MODE_ACTIVE = 0,          // Full operation
    POWER_MODE_LIGHT_SLEEP = 1,     // CPU sleep, peripherals active
    POWER_MODE_DEEP_SLEEP = 2,      // Everything off except RTC
    POWER_MODE_HIBERNATION = 3      // Almost everything off
};

enum WakeSource {
    WAKE_GPIO = 0,                  // Buttons (GPIO interrupt)
    WAKE_TIMER = 1,                 // RTC timer
    WAKE_TOUCHPAD = 2,              // Touch (if available)
    WAKE_UART = 3                   // Serial input
};

class PowerManager {
public:
    // Initialization
    static bool init();
    static void shutdown();

    // Power mode control
    static PowerMode getCurrentMode();
    static bool setMode(PowerMode mode);
    static void sleep(uint32_t ms);
    static void lightSleep(uint32_t ms);
    static void deepSleep(uint32_t ms);
    static void wake();

    // Wake configuration
    static void enableWakeSource(WakeSource source);
    static void disableWakeSource(WakeSource source);

    // Power status
    static uint8_t getBatteryPercentage();
    static bool isCharging();
    static bool isCriticalBattery();

    // System stats
    static uint32_t getUptimeMs();
    static uint32_t getIdleTimeMs();
    static void resetIdleTimer();

    // Debug
    static void printDebugInfo();

private:
    static PowerMode current_mode;
    static uint32_t last_activity_time;
    static uint32_t startup_time;
    static uint8_t wake_sources_enabled;

    static void updatePowerState();
    static void handleLowBattery();
};

#endif // IONOS_POWER_H

#ifndef IONOS_TIME_SERVICE_H
#define IONOS_TIME_SERVICE_H

#include <stdint.h>
#include <Arduino.h>
#include "../drivers/rtc_driver.h"

// ============================================================================
// ionOS v1.0 - TIME SERVICE
// Manages time synchronization, timers, alarms
// ============================================================================

struct Alarm {
    uint8_t hour;
    uint8_t minute;
    bool enabled;
    bool triggered;
    void (*callback)();
};

class TimeService {
public:
    // Initialization
    static bool init();
    static void shutdown();

    // Time management
    static DateTime getTime();
    static void setTime(uint8_t hour, uint8_t minute, uint8_t second);
    static void setDate(uint8_t day, uint8_t month, uint16_t year);

    // Alarm management
    static bool addAlarm(uint8_t hour, uint8_t minute, void (*callback)());
    static bool removeAlarm(uint8_t index);
    static bool enableAlarm(uint8_t index);
    static bool disableAlarm(uint8_t index);
    static uint8_t getAlarmCount();

    // Timer support
    static uint32_t getUptime();  // Milliseconds since startup
    static uint32_t getTimestamp();  // Unix-like timestamp

    // NTP sync (when WiFi available)
    static bool syncTimeWithNTP(const char *ntp_server);

    // Update (call from main loop)
    static void update();

    // Debug
    static void printDebugInfo();

private:
    static const uint8_t MAX_ALARMS = 4;
    static Alarm alarms[MAX_ALARMS];
    static uint8_t alarm_count;

    static uint32_t startup_time;
    static uint32_t last_sync_time;

    // Internal helpers
    static void checkAlarms();
    static void fireAlarm(uint8_t index);
};

#endif // IONOS_TIME_SERVICE_H

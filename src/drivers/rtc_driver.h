#ifndef IONOS_RTC_DRIVER_H
#define IONOS_RTC_DRIVER_H

#include <stdint.h>
#include <Arduino.h>
#include <time.h>

// ============================================================================
// ionOS v1.0 - RTC DRIVER (DS3231)
// Real-time clock for accurate time tracking
// ============================================================================

struct DateTime {
    uint16_t year;   // 2000-2099
    uint8_t month;   // 1-12
    uint8_t day;     // 1-31
    uint8_t hour;    // 0-23
    uint8_t minute;  // 0-59
    uint8_t second;  // 0-59
    uint8_t dow;     // 0=Sunday, 1=Monday, etc.
};

class RTCDriver {
public:
    // Initialization
    static bool init();
    static void shutdown();
    static bool isInitialized();

    // Time reading/writing
    static void getTime(DateTime &dt);
    static void setTime(const DateTime &dt);
    static time_t getUnixTime();
    static void setUnixTime(time_t unix_time);

    // Alarm functions (optional, for wake-up)
    static void setAlarm1(uint8_t hour, uint8_t minute);
    static bool checkAlarm1();

    // Temperature reading (DS3231 has built-in temp sensor)
    static float getTemperature();

    // Debug
    static void printDebugInfo();
    static void printTime(const DateTime &dt);

private:
    static bool initialized;
    
    // I2C communication
    static bool writeRegister(uint8_t reg, uint8_t value);
    static uint8_t readRegister(uint8_t reg);
    static void readRegisters(uint8_t start_reg, uint8_t *data, uint8_t len);

    // BCD conversion
    static uint8_t decToBcd(uint8_t val);
    static uint8_t bcdToDec(uint8_t val);
};

#endif // IONOS_RTC_DRIVER_H

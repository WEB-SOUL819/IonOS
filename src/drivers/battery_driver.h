#ifndef IONOS_BATTERY_DRIVER_H
#define IONOS_BATTERY_DRIVER_H

#include <stdint.h>
#include <Arduino.h>

// ============================================================================
// ionOS v1.0 - BATTERY DRIVER
// Battery voltage monitoring via ADC
// ============================================================================

class BatteryDriver {
public:
    // Initialization
    static bool init();
    static void shutdown();

    // Battery status
    static uint16_t getVoltage();        // Voltage in mV
    static uint8_t getPercentage();      // 0-100%
    static bool isCharging();
    static bool isCritical();

    // Configuration
    static void setMinVoltage(uint16_t mv);
    static void setMaxVoltage(uint16_t mv);

    // Main update (call periodically)
    static void update();

    // Debug
    static void printDebugInfo();

private:
    static uint16_t voltage_mv;
    static uint8_t percentage;
    static uint16_t min_voltage;
    static uint16_t max_voltage;
    static uint32_t last_sample_time;

    static uint16_t readRawVoltage();
};

#endif // IONOS_BATTERY_DRIVER_H

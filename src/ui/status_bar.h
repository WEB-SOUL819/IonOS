#ifndef IONOS_STATUS_BAR_H
#define IONOS_STATUS_BAR_H

#include <stdint.h>
#include <Arduino.h>

// ============================================================================
// ionOS v1.0 - STATUS BAR
// Top bar showing time, battery, WiFi signal, etc.
// ============================================================================

class StatusBar {
public:
    // Initialization
    static bool init();
    static void shutdown();

    // Rendering
    static void render();
    static void update();

    // Status updates
    static void setTime(uint8_t hour, uint8_t minute);
    static void setBattery(uint8_t percent, bool charging);
    static void setSignal(uint8_t bars);  // 0-4 WiFi bars
    static void setBluetoothStatus(bool connected);

    // Display control
    static void setVisible(bool visible);
    static bool isVisible();
    static uint8_t getHeight();

    // Debug
    static void printDebugInfo();

private:
    static uint8_t current_hour;
    static uint8_t current_minute;
    static uint8_t battery_percent;
    static bool battery_charging;
    static uint8_t signal_bars;
    static bool bluetooth_connected;
    static bool visible;

    // Rendering helpers
    static void drawTime();
    static void drawBattery();
    static void drawSignal();
    static void drawBluetooth();
    static void drawSeparator();
};

#endif // IONOS_STATUS_BAR_H

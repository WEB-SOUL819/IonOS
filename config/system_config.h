#ifndef IONOS_SYSTEM_CONFIG_H
#define IONOS_SYSTEM_CONFIG_H

#include "version.h"

// ============================================================================
// ionOS v1.0 - SYSTEM CONFIGURATION
// Global settings and feature flags
// ============================================================================

// ---------------------------------------------------------------------------
// DEBUG & LOGGING
// ---------------------------------------------------------------------------
#define IONOS_DEBUG 1           // Enable serial debug output
#define LOG_LEVEL 3             // 0=ERROR, 1=WARN, 2=INFO, 3=DEBUG

// ---------------------------------------------------------------------------
// DISPLAY CONFIGURATION
// ---------------------------------------------------------------------------
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_FPS 60          // Target refresh rate
#define DISPLAY_I2C_FREQ 400000 // I2C frequency (400kHz)

// ---------------------------------------------------------------------------
// BUTTON CONFIGURATION
// ---------------------------------------------------------------------------
#define BTN_DEBOUNCE_MS 20      // Debounce time in milliseconds
#define BTN_LONG_PRESS_MS 800   // Long press threshold
#define BTN_REPEAT_DELAY_MS 500 // Key repeat delay
#define BTN_REPEAT_RATE_MS 100  // Key repeat rate

// ---------------------------------------------------------------------------
// BATTERY & POWER
// ---------------------------------------------------------------------------
#define BAT_MIN_MV 3000             // Minimum battery voltage (mV)
#define BAT_MAX_MV 4200             // Maximum battery voltage (mV)
#define BAT_SAMPLE_INTERVAL_MS 5000 // Battery check interval

// ---------------------------------------------------------------------------
// POWER MANAGEMENT
// ---------------------------------------------------------------------------
#define SLEEP_TIMEOUT_MS 300000      // Sleep after 5 minutes inactive (300s)
#define DEEP_SLEEP_TIMEOUT_MS 600000 // Deep sleep after 10 minutes (600s)
#define ENABLE_LIGHT_SLEEP 1         // Enable light sleep mode
#define ENABLE_DEEP_SLEEP 1          // Enable deep sleep mode

// ---------------------------------------------------------------------------
// AUDIO CONFIGURATION
// ---------------------------------------------------------------------------
#define AUDIO_SAMPLE_RATE 44100   // Sample rate (Hz)
#define AUDIO_BITS_PER_SAMPLE 16  // Bit depth
#define AUDIO_CHANNELS 2          // Stereo
#define AUDIO_BUFFER_SIZE 512     // DMA buffer size
#define AUDIO_MAX_VOLUME 100      // Max volume percentage

// ---------------------------------------------------------------------------
// STORAGE & SD CARD
// ---------------------------------------------------------------------------
#define SD_MAX_FILES 256          // Max files to list
#define SD_MOUNT_POINT "/sdcard"
#define ENABLE_OTA_UPDATES 1      // Enable OTA firmware updates

// ---------------------------------------------------------------------------
// WIFI & NETWORK
// ---------------------------------------------------------------------------
#define WIFI_CONNECT_TIMEOUT_MS 15000 // WiFi connection timeout
#define ENABLE_WIFI 1                 // Enable WiFi capability
#define ENABLE_BLE 0                  // Enable Bluetooth LE (disabled by default)

// ---------------------------------------------------------------------------
// RTC & TIME
// ---------------------------------------------------------------------------
#define RTC_I2C_FREQ 100000          // RTC I2C frequency (100kHz)
#define NTP_SERVER "pool.ntp.org"
#define NTP_SYNC_INTERVAL_MS 86400000 // Sync every 24 hours

// ---------------------------------------------------------------------------
// APP & KERNEL CONFIGURATION
// ---------------------------------------------------------------------------
#define MAX_APPS 10             // Maximum number of apps
#define MAX_EVENTS 32           // Max events in queue
#define KERNEL_TICK_MS 10       // Kernel tick interval (10ms)
#define STACK_SIZE_LARGE 8192   // Large task stack (bytes)
#define STACK_SIZE_SMALL 2048   // Small task stack (bytes)

// ---------------------------------------------------------------------------
// MEMORY & OPTIMIZATION
// ---------------------------------------------------------------------------
#define ENABLE_PSRAM 0             // Use PSRAM if available
#define FREE_HEAP_THRESHOLD 10000  // Warning threshold (bytes)

// ---------------------------------------------------------------------------
// FEATURE FLAGS
// ---------------------------------------------------------------------------
#define FEATURE_GAMES 1           // Enable games
#define FEATURE_MUSIC_PLAYER 1    // Enable music player
#define FEATURE_TERMINAL 1        // Enable terminal/CLI
#define FEATURE_SETTINGS 1        // Enable settings app
#define FEATURE_FLASHLIGHT 1      // Enable flashlight control

#endif // IONOS_SYSTEM_CONFIG_H

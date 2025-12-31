#include "log_service.h"
#include <Arduino.h>
#include <stdarg.h>

// ============================================================================
// ionOS v1.0 - LOG SERVICE IMPLEMENTATION
// System-wide logging with multiple log levels
// ============================================================================

LogLevel LogService::current_level = LOG_INFO;
bool LogService::serial_enabled = true;
bool LogService::file_logging = false;
char LogService::log_file[64] = "/logs/system.log";
uint32_t LogService::total_logs = 0;

// Color codes for serial output
static const char* LOG_COLORS[] = {
    "\033[0;31m",   // RED - ERROR
    "\033[0;33m",   // YELLOW - WARNING
    "\033[0;32m",   // GREEN - INFO
    "\033[0;36m",   // CYAN - DEBUG
    "\033[0;37m"    // WHITE - TRACE
};

static const char* LOG_LEVEL_NAMES[] = {
    "ERROR",
    "WARNING",
    "INFO",
    "DEBUG",
    "TRACE"
};

static const char* LOG_RESET = "\033[0m";

bool LogService::init() {
    current_level = LOG_INFO;
    serial_enabled = true;
    file_logging = false;
    total_logs = 0;
    
    Serial.println("[LOG] Log service initialized");
    return true;
}

void LogService::shutdown() {
    Serial.println("[LOG] Log service shutdown");
}

void LogService::setLogLevel(LogLevel level) {
    current_level = level;
    Serial.printf("[LOG] Log level set to: %s\n", LOG_LEVEL_NAMES[level]);
}

LogLevel LogService::getLogLevel() {
    return current_level;
}

void LogService::enableSerialOutput(bool enabled) {
    serial_enabled = enabled;
}

void LogService::enableFileLogging(bool enabled) {
    file_logging = enabled;
    if (enabled) {
        Serial.printf("[LOG] File logging enabled: %s\n", log_file);
    } else {
        Serial.println("[LOG] File logging disabled");
    }
}

void LogService::setLogFile(const char *filepath) {
    strncpy(log_file, filepath, sizeof(log_file) - 1);
    log_file[sizeof(log_file) - 1] = '\0';
}

void LogService::logError(const char *tag, const char *format, ...) {
    if (current_level < LOG_ERROR) return;
    
    va_list args;
    va_start(args, format);
    logMessage(LOG_ERROR, tag, format, args);
    va_end(args);
}

void LogService::logWarning(const char *tag, const char *format, ...) {
    if (current_level < LOG_WARNING) return;
    
    va_list args;
    va_start(args, format);
    logMessage(LOG_WARNING, tag, format, args);
    va_end(args);
}

void LogService::logInfo(const char *tag, const char *format, ...) {
    if (current_level < LOG_INFO) return;
    
    va_list args;
    va_start(args, format);
    logMessage(LOG_INFO, tag, format, args);
    va_end(args);
}

void LogService::logDebug(const char *tag, const char *format, ...) {
    if (current_level < LOG_DEBUG) return;
    
    va_list args;
    va_start(args, format);
    logMessage(LOG_DEBUG, tag, format, args);
    va_end(args);
}

void LogService::logTrace(const char *tag, const char *format, ...) {
    if (current_level < LOG_TRACE) return;
    
    va_list args;
    va_start(args, format);
    logMessage(LOG_TRACE, tag, format, args);
    va_end(args);
}

void LogService::logMessage(LogLevel level, const char *tag, const char *format, va_list args) {
    total_logs++;
    
    // Format timestamp (simplified)
    uint32_t millis_now = millis();
    uint32_t seconds = millis_now / 1000;
    uint32_t milliseconds = millis_now % 1000;
    
    // Build log message
    char log_buffer[512];
    char msg_buffer[256];
    
    // Format the actual message with va_args
    vsnprintf(msg_buffer, sizeof(msg_buffer), format, args);
    
    if (serial_enabled) {
        // Format for serial with colors
        snprintf(log_buffer, sizeof(log_buffer),
            "%s[%3lu.%03lu] [%-7s] [%s]%s %s\n",
            LOG_COLORS[level],
            seconds, milliseconds,
            LOG_LEVEL_NAMES[level],
            tag,
            LOG_RESET,
            msg_buffer);
        
        Serial.print(log_buffer);
    }
    
    if (file_logging) {
        // Format for file (without colors)
        snprintf(log_buffer, sizeof(log_buffer),
            "[%3lu.%03lu] [%-7s] [%s] %s\n",
            seconds, milliseconds,
            LOG_LEVEL_NAMES[level],
            tag,
            msg_buffer);
        
        // TODO: Write to SD card using StorageService
        // StorageService::appendFile(log_file, log_buffer);
    }
}

uint32_t LogService::getTotalLogCount() {
    return total_logs;
}

void LogService::clearLogs() {
    total_logs = 0;
    // TODO: Delete log file from SD card
    Serial.println("[LOG] Logs cleared");
}

void LogService::printDebugInfo() {
    Serial.println("\nâ•”â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•—");
    Serial.println("â•‘  LOG SERVICE DEBUG INFO           â•‘");
    Serial.println("â• â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•£");
    Serial.printf("â•‘ Current Level: %s\n", LOG_LEVEL_NAMES[current_level]);
    Serial.printf("â•‘ Serial Output: %s\n", serial_enabled ? "ENABLED" : "DISABLED");
    Serial.printf("â•‘ File Logging: %s\n", file_logging ? "ENABLED" : "DISABLED");
    Serial.printf("â•‘ Log File: %s\n", log_file);
    Serial.printf("â•‘ Total Logs: %lu\n", total_logs);
    Serial.println("â•šâ•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•\n");
}

void LogService::printBanner(const char *text) {
    Serial.println("\nâ•”â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•—");
    Serial.printf("â•‘ %s\n", text);
    Serial.println("â•šâ•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•\n");
}

void LogService::printSeparator() {
    Serial.println("â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€");
}

void LogService::printHexDump(const char *tag, const uint8_t *data, uint16_t length) {
    Serial.printf("[%s] Hex Dump (%d bytes):\n", tag, length);
    
    for (uint16_t i = 0; i < length; i += 16) {
        Serial.printf("%04X: ", i);
        
        // Print hex values
        for (uint16_t j = 0; j < 16 && i + j < length; j++) {
            Serial.printf("%02X ", data[i + j]);
        }
        
        // Print ASCII representation
        Serial.print("  ");
        for (uint16_t j = 0; j < 16 && i + j < length; j++) {
            uint8_t c = data[i + j];
            if (c >= 32 && c <= 126) {
                Serial.printf("%c", c);
            } else {
                Serial.print(".");
            }
        }
        
        Serial.println();
    }
}

void LogService::printMemoryUsage() {
    // ESP32-specific memory info
    Serial.println("\nâ•”â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•—");
    Serial.println("â•‘  MEMORY USAGE INFO                â•‘");
    Serial.println("â• â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•£");
    
    // Get free heap
    uint32_t free_heap = esp_get_free_heap_size();
    uint32_t min_free_heap = esp_get_minimum_free_heap_size();
    uint32_t total_heap = 520 * 1024;  // ESP32 has ~520KB SRAM
    
    Serial.printf("â•‘ Free Heap: %lu bytes\n", free_heap);
    Serial.printf("â•‘ Min Free Heap: %lu bytes\n", min_free_heap);
    Serial.printf("â•‘ Total Heap: %lu bytes\n", total_heap);
    Serial.printf("â•‘ Heap Usage: %.1f%%\n", (float)(total_heap - free_heap) / total_heap * 100);
    
    Serial.println("â•šâ•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•\n");
}

void LogService::printUptime() {
    uint32_t ms = millis();
    uint32_t seconds = ms / 1000;
    uint32_t minutes = seconds / 60;
    uint32_t hours = minutes / 60;
    uint32_t days = hours / 24;
    
    Serial.println("\nâ•”â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•—");
    Serial.println("â•‘  SYSTEM UPTIME                    â•‘");
    Serial.println("â• â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•£");
    
    Serial.printf("â•‘ %lu days, %02lu:%02lu:%02lu\n",
        days,
        hours % 24,
        minutes % 60,
        seconds % 60);
    
    Serial.printf("â•‘ Total ms: %lu\n", ms);
    
    Serial.println("â•šâ•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•\n");
}

// Helper macro implementations (if not using macros)
void LogService::assert_fail(const char *condition, const char *file, uint32_t line) {
    logError("ASSERT", "Assertion failed: %s at %s:%lu", condition, file, line);
}

void LogService::panic(const char *message) {
    logError("PANIC", "System panic: %s", message);
    Serial.println("\n!!! SYSTEM PANIC !!!");
    Serial.println("Rebooting in 5 seconds...");
    delay(5000);
    ESP.restart();
}

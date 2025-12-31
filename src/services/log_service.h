#ifndef IONOS_LOG_SERVICE_H
#define IONOS_LOG_SERVICE_H

#include <stdint.h>
#include <Arduino.h>

// ============================================================================
// ionOS v1.0 - LOG SERVICE
// System logging to serial and optional SD card
// ============================================================================

enum LogLevel {
    LOG_DEBUG = 0,      // Detailed debug info
    LOG_INFO = 1,       // General information
    LOG_WARN = 2,       // Warnings
    LOG_ERROR = 3,      // Errors
    LOG_CRITICAL = 4    // Critical failures
};

class LogService {
public:
    // Initialize logging
    static bool init(LogLevel level = LOG_INFO);
    static void shutdown();

    // Set log level
    static void setLogLevel(LogLevel level);
    static LogLevel getLogLevel();

    // Logging functions (use printf-style formatting)
    static void debug(const char *format, ...);
    static void info(const char *format, ...);
    static void warn(const char *format, ...);
    static void error(const char *format, ...);
    static void critical(const char *format, ...);

    // Core logging function
    static void log(LogLevel level, const char *format, ...);

    // Direct logging (raw)
    static void logRaw(const char *message);

    // File logging control
    static void enableFileLogging(bool enable);
    static bool isFileLoggingEnabled();

    // Clear log file
    static bool clearLogFile();

    // Get log file size
    static uint32_t getLogFileSize();

private:
    static LogLevel current_level;
    static bool file_logging_enabled;
    static const char *LOG_FILE;
    static const uint32_t MAX_LOG_FILE_SIZE;

    // Internal helpers
    static const char* levelToString(LogLevel level);
    static void logToSerial(const char *message);
    static void logToFile(const char *message);
};

// Convenience macros for logging
#define LOG_DEBUG(fmt, ...) LogService::debug(fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) LogService::info(fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) LogService::warn(fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) LogService::error(fmt, ##__VA_ARGS__)
#define LOG_CRITICAL(fmt, ...) LogService::critical(fmt, ##__VA_ARGS__)

#endif // IONOS_LOG_SERVICE_H

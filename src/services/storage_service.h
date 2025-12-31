#ifndef IONOS_STORAGE_SERVICE_H
#define IONOS_STORAGE_SERVICE_H

#include <stdint.h>
#include <Arduino.h>
#include <SD.h>

// ============================================================================
// ionOS v1.0 - STORAGE SERVICE
// MicroSD card file I/O and data persistence
// ============================================================================

class StorageService {
public:
    // Initialization
    static bool init();
    static void shutdown();

    // File operations
    static bool fileExists(const char *path);
    static bool deleteFile(const char *path);
    static uint32_t getFileSize(const char *path);

    // Read operations
    static bool readFile(const char *path, uint8_t *buffer, uint32_t size);
    static bool readLine(const char *path, char *line, uint32_t max_len);

    // Write operations
    static bool writeFile(const char *path, const uint8_t *data, uint32_t size);
    static bool appendFile(const char *path, const uint8_t *data, uint32_t size);

    // Directory operations
    static bool createDir(const char *path);
    static bool listDir(const char *path, void (*callback)(const char *filename));

    // Settings persistence
    static bool saveSetting(const char *key, const char *value);
    static bool loadSetting(const char *key, char *value, uint32_t max_len);

    // SD card info
    static bool getCardInfo(uint32_t &total_bytes, uint32_t &used_bytes);
    static bool isCardPresent();

    // Debug
    static void printDebugInfo();

private:
    static bool initialized;
    static const char *SETTINGS_FILE;

    // Internal helpers
    static bool ensureDir(const char *path);
};

#endif // IONOS_STORAGE_SERVICE_H

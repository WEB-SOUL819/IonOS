#include "storage_service.h"
#include <Arduino.h>
#include <SD.h>

// ============================================================================
// ionOS v1.0 - STORAGE SERVICE IMPLEMENTATION
// SD card file I/O operations
// ============================================================================

bool StorageService::is_initialized = false;

bool StorageService::init() {
    // Initialize SPI and SD card
    // CS pin: GPIO 5
    // MOSI: GPIO 23, MISO: GPIO 19, SCK: GPIO 18
    
    if (!SD.begin(5)) {
        Serial.println("[STORAGE] SD card initialization failed!");
        is_initialized = false;
        return false;
    }
    
    is_initialized = true;
    Serial.println("[STORAGE] SD card initialized successfully");
    return true;
}

void StorageService::shutdown() {
    // SD.end() not available in Arduino SD library
    is_initialized = false;
    Serial.println("[STORAGE] SD card disabled");
}

bool StorageService::fileExists(const char *filepath) {
    if (!is_initialized) return false;
    
    return SD.exists(filepath);
}

bool StorageService::createFile(const char *filepath) {
    if (!is_initialized) return false;
    
    File file = SD.open(filepath, FILE_WRITE);
    if (!file) {
        Serial.printf("[STORAGE] Failed to create file: %s\n", filepath);
        return false;
    }
    
    file.close();
    Serial.printf("[STORAGE] File created: %s\n", filepath);
    return true;
}

bool StorageService::deleteFile(const char *filepath) {
    if (!is_initialized) return false;
    
    if (!SD.remove(filepath)) {
        Serial.printf("[STORAGE] Failed to delete file: %s\n", filepath);
        return false;
    }
    
    Serial.printf("[STORAGE] File deleted: %s\n", filepath);
    return true;
}

bool StorageService::readFile(const char *filepath, char *buffer, uint32_t max_len) {
    if (!is_initialized) return false;
    
    File file = SD.open(filepath, FILE_READ);
    if (!file) {
        Serial.printf("[STORAGE] Failed to open file: %s\n", filepath);
        return false;
    }
    
    uint32_t bytes_read = 0;
    while (file.available() && bytes_read < max_len - 1) {
        buffer[bytes_read++] = file.read();
    }
    buffer[bytes_read] = '\0';
    
    file.close();
    Serial.printf("[STORAGE] Read %d bytes from: %s\n", bytes_read, filepath);
    return true;
}

bool StorageService::writeFile(const char *filepath, const char *data) {
    if (!is_initialized) return false;
    
    File file = SD.open(filepath, FILE_WRITE);
    if (!file) {
        Serial.printf("[STORAGE] Failed to open file for writing: %s\n", filepath);
        return false;
    }
    
    uint32_t bytes_written = file.print(data);
    file.close();
    
    Serial.printf("[STORAGE] Wrote %d bytes to: %s\n", bytes_written, filepath);
    return bytes_written > 0;
}

bool StorageService::appendFile(const char *filepath, const char *data) {
    if (!is_initialized) return false;
    
    File file = SD.open(filepath, FILE_APPEND);
    if (!file) {
        Serial.printf("[STORAGE] Failed to open file for appending: %s\n", filepath);
        return false;
    }
    
    uint32_t bytes_written = file.print(data);
    file.close();
    
    Serial.printf("[STORAGE] Appended %d bytes to: %s\n", bytes_written, filepath);
    return bytes_written > 0;
}

uint32_t StorageService::getFileSize(const char *filepath) {
    if (!is_initialized) return 0;
    
    File file = SD.open(filepath, FILE_READ);
    if (!file) {
        return 0;
    }
    
    uint32_t size = file.size();
    file.close();
    
    return size;
}

bool StorageService::listFiles(const char *directory, char *buffer, uint32_t max_len) {
    if (!is_initialized) return false;
    
    File dir = SD.open(directory);
    if (!dir || !dir.isDirectory()) {
        Serial.printf("[STORAGE] Failed to open directory: %s\n", directory);
        return false;
    }
    
    buffer[0] = '\0';
    uint32_t remaining = max_len;
    
    File file = dir.openNextFile();
    while (file && remaining > 0) {
        const char *name = file.name();
        uint32_t name_len = strlen(name);
        
        if (name_len + 2 <= remaining) {
            strcat(buffer, name);
            strcat(buffer, "\n");
            remaining -= (name_len + 1);
        }
        
        file = dir.openNextFile();
    }
    
    dir.close();
    Serial.printf("[STORAGE] Listed files in: %s\n", directory);
    return true;
}

uint32_t StorageService::getTotalSpace() {
    if (!is_initialized) return 0;
    
    // SD card total space depends on card size
    // For now, return a placeholder
    return 0;
}

uint32_t StorageService::getFreeSpace() {
    if (!is_initialized) return 0;
    
    // Free space calculation not available in Arduino SD library
    // For now, return a placeholder
    return 0;
}

bool StorageService::isInitialized() {
    return is_initialized;
}

void StorageService::printDebugInfo() {
    Serial.println("\nâ•”â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•—");
    Serial.println("â•‘  STORAGE SERVICE DEBUG INFO      â•‘");
    Serial.println("â• â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•£");
    Serial.printf("â•‘ SD Card: %s\n", is_initialized ? "READY" : "NOT READY");
    Serial.printf("â•‘ Total Space: %lu bytes\n", getTotalSpace());
    Serial.printf("â•‘ Free Space: %lu bytes\n", getFreeSpace());
    Serial.println("â•‘ Root files:");
    
    // List root directory files
    File root = SD.open("/");
    File file = root.openNextFile();
    while (file) {
        Serial.printf("â•‘   - %s\n", file.name());
        file = root.openNextFile();
    }
    
    Serial.println("â•šâ•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•\n");
}

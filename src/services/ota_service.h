#ifndef IONOS_OTA_SERVICE_H
#define IONOS_OTA_SERVICE_H

#include <stdint.h>
#include <Arduino.h>

// ============================================================================
// ionOS v1.0 - OTA SERVICE
// Over-the-air firmware updates
// ============================================================================

enum OTAState {
    OTA_IDLE = 0,
    OTA_CHECKING = 1,
    OTA_DOWNLOADING = 2,
    OTA_FLASHING = 3,
    OTA_COMPLETE = 4,
    OTA_ERROR = 5
};

class OTAService {
public:
    static bool init();
    static void shutdown();

    // Update checking
    static bool checkForUpdates(const char *server_url);
    static bool startUpdate(const char *update_url);
    static void cancelUpdate();

    // State queries
    static OTAState getState();
    static uint8_t getProgress();  // 0-100%
    static const char* getErrorMessage();

    // Update handler
    static void update();

    // Debug
    static void printDebugInfo();

private:
    static OTAState current_state;
    static uint8_t download_progress;
    static char error_message[64];

    // Internal helpers
    static bool verifySignature(const char *filepath);
    static bool flashFirmware(const char *filepath);
};

#endif // IONOS_OTA_SERVICE_H

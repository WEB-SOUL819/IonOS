#ifndef IONOS_NETWORK_SERVICE_H
#define IONOS_NETWORK_SERVICE_H

#include <stdint.h>
#include <Arduino.h>

// ============================================================================
// ionOS v1.0 - NETWORK SERVICE
// WiFi connectivity and network operations
// ============================================================================

enum WiFiState {
    WIFI_DISCONNECTED = 0,
    WIFI_CONNECTING = 1,
    WIFI_CONNECTED = 2,
    WIFI_ERROR = 3
};

class NetworkService {
public:
    static bool init();
    static void shutdown();

    // WiFi control
    static bool connectToWiFi(const char *ssid, const char *password);
    static bool disconnect();
    static WiFiState getState();
    static bool isConnected();

    // Network info
    static const char* getSSID();
    static const char* getIP();
    static int8_t getSignalStrength();  // -100 to 0 dBm

    // HTTP operations
    static bool httpGet(const char *url, char *response, uint32_t max_len);
    static bool httpPost(const char *url, const char *data, char *response, uint32_t max_len);

    // Update handler
    static void update();

    // Debug
    static void printDebugInfo();

private:
    static WiFiState current_state;
};

#endif // IONOS_NETWORK_SERVICE_H

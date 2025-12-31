#ifndef IONOS_EVENTS_H
#define IONOS_EVENTS_H

#include <stdint.h>
#include <Arduino.h>

// ============================================================================
// ionOS v1.0 - EVENT SYSTEM
// Event queue for inter-system communication
// ============================================================================

enum EventType {
    // System events
    EVENT_NONE = 0,
    EVENT_SYSTEM_STARTUP = 1,
    EVENT_SYSTEM_SHUTDOWN = 2,
    EVENT_SYSTEM_TICK = 3,
    EVENT_SYSTEM_ERROR = 4,

    // Input events
    EVENT_BUTTON_PRESS = 10,
    EVENT_BUTTON_RELEASE = 11,
    EVENT_BUTTON_LONG_PRESS = 12,

    // Power events
    EVENT_POWER_LOW_BATTERY = 20,
    EVENT_POWER_CHARGING = 21,
    EVENT_POWER_SLEEP = 22,
    EVENT_POWER_WAKE = 23,

    // UI events
    EVENT_UI_UPDATE = 30,
    EVENT_UI_NAVIGATE = 31,
    EVENT_UI_REDRAW = 32,

    // App events
    EVENT_APP_LAUNCH = 40,
    EVENT_APP_CLOSE = 41,
    EVENT_APP_SUSPEND = 42,
    EVENT_APP_RESUME = 43,

    // Service events
    EVENT_SERVICE_READY = 50,
    EVENT_SERVICE_ERROR = 51,

    // Custom app events
    EVENT_CUSTOM_START = 100
};

enum EventPriority {
    PRIORITY_LOW = 0,
    PRIORITY_NORMAL = 1,
    PRIORITY_HIGH = 2,
    PRIORITY_CRITICAL = 3
};

struct Event {
    EventType type;
    EventPriority priority;
    uint32_t timestamp;
    uint32_t data1;          // Generic data field 1
    uint32_t data2;          // Generic data field 2
    void *ptr;               // Pointer to additional data
};

class EventQueue {
public:
    // Queue management
    static bool init();
    static void shutdown();

    // Event operations
    static bool enqueue(const Event &event);
    static bool dequeue(Event &event);
    static bool peek(Event &event);
    static void clear();

    // Status
    static uint8_t getQueueSize();
    static uint8_t getMaxQueueSize();
    static bool isEmpty();
    static bool isFull();

    // Debug
    static void printDebugInfo();

private:
    static const uint8_t MAX_EVENTS = 32;
    static Event queue[MAX_EVENTS];
    static uint8_t head;
    static uint8_t tail;
    static uint8_t count;
};

#endif // IONOS_EVENTS_H

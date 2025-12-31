#ifndef IONOS_EVENTS_H
#define IONOS_EVENTS_H

#include <stdint.h>

// ============================================================================
// ionOS v1.0 - EVENT SYSTEM
// Central event queue for kernel-driven architecture
// ============================================================================

// Event types
enum EventType {
    // System events
    EVENT_SYSTEM_INIT = 0,
    EVENT_SYSTEM_SHUTDOWN = 1,
    EVENT_SYSTEM_TICK = 2,
    EVENT_SYSTEM_ERROR = 3,
    EVENT_SYSTEM_WARNING = 4,

    // Button events (6-button layout: UP, DOWN, LEFT, RIGHT, SELECT, BACK)
    EVENT_BUTTON_PRESS = 10,           // Short press
    EVENT_BUTTON_RELEASE = 11,         // Button released
    EVENT_BUTTON_LONG_PRESS = 12,      // Long press detected

    // Power events
    EVENT_POWER_LOW_BATTERY = 20,
    EVENT_POWER_CHARGING_START = 21,
    EVENT_POWER_CHARGING_STOP = 22,
    EVENT_POWER_SLEEP = 23,
    EVENT_POWER_WAKE = 24,

    // Display events
    EVENT_DISPLAY_UPDATE = 30,
    EVENT_DISPLAY_ERROR = 31,

    // App events
    EVENT_APP_LAUNCH = 40,
    EVENT_APP_CLOSE = 41,
    EVENT_APP_SUSPEND = 42,
    EVENT_APP_RESUME = 43,
    EVENT_APP_BACK = 44,

    // Input events (general)
    EVENT_INPUT_GESTURE = 50,

    // Network events
    EVENT_NETWORK_CONNECTED = 60,
    EVENT_NETWORK_DISCONNECTED = 61,
    EVENT_NETWORK_ERROR = 62,

    // Storage events
    EVENT_STORAGE_ERROR = 70,

    // Audio events
    EVENT_AUDIO_PLAY = 80,
    EVENT_AUDIO_PAUSE = 81,
    EVENT_AUDIO_STOP = 82,
    EVENT_AUDIO_ERROR = 83,

    // Custom/user events
    EVENT_CUSTOM = 100,

    // Null event (unused slot)
    EVENT_NONE = 255
};

// Priority levels
enum EventPriority {
    PRIORITY_LOW = 0,
    PRIORITY_NORMAL = 1,
    PRIORITY_HIGH = 2,
    PRIORITY_CRITICAL = 3
};

// Event structure
struct Event {
    EventType type;           // What happened
    EventPriority priority;   // How urgent
    uint32_t timestamp;       // When it happened (millis())
    uint8_t data1;           // Generic data field 1
    uint8_t data2;           // Generic data field 2
    void *data3;             // Generic pointer (for larger data)
};

// Button IDs (matching 6-button layout)
enum ButtonID {
    BTN_ID_UP = 0,
    BTN_ID_DOWN = 1,
    BTN_ID_LEFT = 2,
    BTN_ID_RIGHT = 3,
    BTN_ID_SELECT = 4,
    BTN_ID_BACK = 5
};

// Event queue manager
class EventQueue {
public:
    // Initialization
    static bool init(uint16_t queue_size = 64);
    static void shutdown();

    // Queue operations
    static bool postEvent(const Event &event);
    static bool getEvent(Event &event);  // FIFO - removes from queue
    static bool peekEvent(Event &event); // Peek without removing
    static void flush();                 // Clear all events

    // Queue state
    static uint16_t getEventCount();
    static uint16_t getQueueCapacity();
    static bool isEmpty();
    static bool isFull();

    // Event filtering (optional)
    static void setEventFilter(EventType type, bool enabled);

    // Debug
    static void printDebugInfo();
    static void printQueue();

private:
    static const uint16_t MAX_QUEUE_SIZE = 256;
    static Event event_queue[MAX_QUEUE_SIZE];
    static uint16_t queue_head;
    static uint16_t queue_tail;
    static uint16_t queue_count;
    static uint16_t queue_capacity;

    // Event filtering
    static bool event_filter[256];

    // Internal helpers
    static uint16_t nextIndex(uint16_t current);
};

#endif // IONOS_EVENTS_H

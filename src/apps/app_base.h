#ifndef IONOS_APP_BASE_H
#define IONOS_APP_BASE_H

#include <stdint.h>
#include "../core/events.h"

// ============================================================================
// ionOS v1.0 - APP BASE CLASS
// Abstract base for all applications
// ============================================================================

enum AppState {
    APP_STATE_INACTIVE = 0,
    APP_STATE_ACTIVE = 1,
    APP_STATE_SUSPENDED = 2,
    APP_STATE_CLOSING = 3
};

class App {
public:
    virtual ~App() = default;

    // Lifecycle methods
    virtual void onLaunch() = 0;      // Called when app starts
    virtual void onSuspend() {}       // Called when app paused
    virtual void onResume() {}        // Called when app resumed
    virtual void onClose() = 0;       // Called when app closes

    // Event handling
    virtual void onEvent(const Event &event) = 0;

    // Game loop
    virtual void update() = 0;        // Update game logic
    virtual void render() = 0;        // Render to display

    // App metadata
    virtual const char* getName() = 0;
    virtual const char* getVersion() { return "1.0"; }

    // State queries
    AppState getState() const { return state; }
    void setState(AppState new_state) { state = new_state; }

    // Utility
    virtual bool handleBackButton() { return false; }  // True if handled

protected:
    AppState state = APP_STATE_INACTIVE;
};

#endif // IONOS_APP_BASE_H

#include "app_base.h"

// Constructor
App::App() : state(APP_STATE_INACTIVE) {}

// Destructor  
App::~App() {}

// No-op virtual method (can be overridden)
void App::handleBackButton() {
    // Default: go back to launcher
    // Override in derived classes if needed
}

AppState App::getState() {
    return state;
}

#include "launcher_app.h"
#include "../drivers/display_driver.h"
#include "../drivers/button_driver.h"

// ============================================================================
// ionOS v1.0 - LAUNCHER APP IMPLEMENTATION
// ============================================================================

LauncherApp::LauncherApp() : app_count(0), selected_index(0), last_input_time(0) {
    for (int i = 0; i < MAX_APPS; i++) {
        apps[i].name = nullptr;
        apps[i].icon = nullptr;
        apps[i].app_instance = nullptr;
    }
}

LauncherApp::~LauncherApp() {
    // Cleanup
}

void LauncherApp::onLaunch() {
    state = APP_STATE_ACTIVE;
    selected_index = 0;
    Serial.println("[LAUNCHER] App launched");
}

void LauncherApp::onClose() {
    state = APP_STATE_CLOSING;
    Serial.println("[LAUNCHER] App closing");
}

void LauncherApp::onEvent(const Event &event) {
    uint32_t now = millis();

    // Debounce rapid inputs
    if (now - last_input_time < 200) return;
    last_input_time = now;

    switch (event.type) {
        case EVENT_BUTTON_PRESS:
            switch (event.data1) {
                case BTN_ID_UP:
                case BTN_ID_LEFT:
                    moveSelection(-1);
                    break;
                case BTN_ID_DOWN:
                case BTN_ID_RIGHT:
                    moveSelection(1);
                    break;
                case BTN_ID_SELECT:
                    launchSelectedApp();
                    break;
                case BTN_ID_BACK:
                    handleBackButton();
                    break;
            }
            break;
        default:
            break;
    }
}

void LauncherApp::update() {
    // Update selected app animation or state
}

void LauncherApp::render() {
    DisplayDriver::clear();

    // Draw title
    DisplayDriver::drawString(40, 2, "LAUNCHER", true);

    // Draw separator
    DisplayDriver::drawLine(0, 10, 128, 10, true);

    // Draw app list
    renderAppList();

    // Draw footer with button hints
    DisplayDriver::drawLine(0, 54, 128, 54, true);
    DisplayDriver::drawString(2, 56, "Select: OK  Back: MENU", false);
}

void LauncherApp::registerApp(const char *name, const char *icon, App *app) {
    if (app_count >= MAX_APPS) {
        Serial.println("[LAUNCHER] App list full");
        return;
    }

    apps[app_count].name = name;
    apps[app_count].icon = icon;
    apps[app_count].app_instance = app;
    app_count++;

    Serial.printf("[LAUNCHER] Registered app: %s\n", name);
}

void LauncherApp::unregisterApp(uint8_t index) {
    if (index >= app_count) return;

    // Shift array down
    for (int i = index; i < app_count - 1; i++) {
        apps[i] = apps[i + 1];
    }
    app_count--;
}

uint8_t LauncherApp::getAppCount() {
    return app_count;
}

void LauncherApp::renderAppList() {
    const uint8_t items_per_screen = 4;
    uint8_t start_idx = (selected_index / items_per_screen) * items_per_screen;

    for (int i = 0; i < items_per_screen && start_idx + i < app_count; i++) {
        uint8_t app_idx = start_idx + i;
        uint8_t y = 15 + (i * 10);

        // Highlight selected item
        if (app_idx == selected_index) {
            DisplayDriver::drawRect(0, y - 2, 128, 10, false, true);
            DisplayDriver::drawString(4, y, apps[app_idx].name, false);  // Inverted
        } else {
            DisplayDriver::drawString(4, y, apps[app_idx].name, true);
        }
    }
}

void LauncherApp::renderAppGrid() {
    // Alternative grid layout (not implemented in this basic version)
}

void LauncherApp::renderSelectedApp() {
    if (selected_index < app_count) {
        Serial.printf("[LAUNCHER] Selected: %s\n", apps[selected_index].name);
    }
}

void LauncherApp::moveSelection(int8_t direction) {
    int8_t new_index = selected_index + direction;

    if (new_index < 0) {
        new_index = app_count - 1;  // Wrap to end
    } else if (new_index >= app_count) {
        new_index = 0;  // Wrap to start
    }

    selected_index = new_index;
    Serial.printf("[LAUNCHER] Selection moved to: %s\n", apps[selected_index].name);
}

void LauncherApp::launchSelectedApp() {
    if (selected_index >= app_count) return;

    App *selected_app = apps[selected_index].app_instance;
    if (selected_app == nullptr) return;

    Serial.printf("[LAUNCHER] Launching app: %s\n", apps[selected_index].name);

    // Launch the app (kernel will handle switching)
    selected_app->onLaunch();
    selected_app->setState(APP_STATE_ACTIVE);
}

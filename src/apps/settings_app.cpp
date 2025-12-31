#include "settings_app.h"
#include "../drivers/display_driver.h"
#include "../drivers/button_driver.h"

SettingsApp::SettingsApp() : current_menu(SETTINGS_BRIGHTNESS), brightness_level(80), volume_level(70), sleep_timeout_ms(300000) {}

SettingsApp::~SettingsApp() {}

void SettingsApp::onLaunch() {
    state = APP_STATE_ACTIVE;
    Serial.println("[SETTINGS] Settings app launched");
}

void SettingsApp::onClose() {
    state = APP_STATE_CLOSING;
    Serial.println("[SETTINGS] Settings app closing");
}

void SettingsApp::onEvent(const Event &event) {
    if (event.type != EVENT_BUTTON_PRESS) return;

    switch (event.data1) {
        case BTN_ID_UP:
        case BTN_ID_LEFT:
            moveMenuSelection(-1);
            break;
        case BTN_ID_DOWN:
        case BTN_ID_RIGHT:
            moveMenuSelection(1);
            break;
        case BTN_ID_SELECT:
            Serial.printf("[SETTINGS] Selected menu item: %d\n", current_menu);
            break;
        case BTN_ID_BACK:
            handleBackButton();
            break;
    }
}

void SettingsApp::update() {
    // Update logic
}

void SettingsApp::render() {
    DisplayDriver::clear();
    DisplayDriver::drawString(35, 2, "SETTINGS", true);
    DisplayDriver::drawLine(0, 10, 128, 10, true);
    renderMainMenu();
    DisplayDriver::drawLine(0, 54, 128, 54, true);
}

void SettingsApp::renderMainMenu() {
    const char *menu_items[] = { "Brightness", "Volume", "Time", "Date", "Sleep", "Language", "About" };
    for (int i = 0; i < 7; i++) {
        uint8_t y = 15 + (i * 8);
        if (i == current_menu) {
            DisplayDriver::drawRect(0, y - 2, 128, 8, false, true);
            DisplayDriver::drawString(4, y, menu_items[i], false);
        } else {
            DisplayDriver::drawString(4, y, menu_items[i], true);
        }
    }
}

void SettingsApp::renderBrightnessSetting() {}
void SettingsApp::renderVolumeSetting() {}
void SettingsApp::renderTimeSetting() {}
void SettingsApp::renderDateSetting() {}
void SettingsApp::renderSleepTimeout() {}
void SettingsApp::renderAbout() {}

void SettingsApp::moveMenuSelection(int8_t direction) {
    int8_t new_menu = current_menu + direction;
    if (new_menu < 0) new_menu = 6;
    else if (new_menu > 6) new_menu = 0;
    current_menu = (SettingsMenu)new_menu;
}

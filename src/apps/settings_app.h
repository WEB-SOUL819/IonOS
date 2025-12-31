#ifndef IONOS_SETTINGS_APP_H
#define IONOS_SETTINGS_APP_H

#include "../apps/app_base.h"

// ============================================================================
// ionOS v1.0 - SETTINGS APP
// System configuration (brightness, volume, time, etc.)
// ============================================================================

enum SettingsMenu {
    SETTINGS_BRIGHTNESS = 0,
    SETTINGS_VOLUME = 1,
    SETTINGS_TIME = 2,
    SETTINGS_DATE = 3,
    SETTINGS_SLEEP_TIMEOUT = 4,
    SETTINGS_LANGUAGE = 5,
    SETTINGS_ABOUT = 6
};

class SettingsApp : public App {
public:
    SettingsApp();
    ~SettingsApp();

    void onLaunch() override;
    void onClose() override;
    void onEvent(const Event &event) override;
    void update() override;
    void render() override;
    const char* getName() override { return "Settings"; }

private:
    SettingsMenu current_menu;
    uint8_t brightness_level;
    uint8_t volume_level;
    uint32_t sleep_timeout_ms;

    // Menu rendering
    void renderMainMenu();
    void renderBrightnessSetting();
    void renderVolumeSetting();
    void renderTimeSetting();
    void renderDateSetting();
    void renderSleepTimeout();
    void renderAbout();

    // Navigation
    void moveMenuSelection(int8_t direction);
};

#endif // IONOS_SETTINGS_APP_H

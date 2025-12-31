#ifndef IONOS_LAUNCHER_APP_H
#define IONOS_LAUNCHER_APP_H

#include "../apps/app_base.h"
#include <vector>

// ============================================================================
// ionOS v1.0 - LAUNCHER APP
// Main menu - displays and launches other apps
// ============================================================================

struct AppMenuItem {
    const char *name;
    const char *icon;
    App *app_instance;
};

class LauncherApp : public App {
public:
    LauncherApp();
    ~LauncherApp();

    void onLaunch() override;
    void onClose() override;
    void onEvent(const Event &event) override;
    void update() override;
    void render() override;
    const char* getName() override { return "Launcher"; }

    // App registration
    void registerApp(const char *name, const char *icon, App *app);
    void unregisterApp(uint8_t index);
    uint8_t getAppCount();

private:
    static const uint8_t MAX_APPS = 8;
    AppMenuItem apps[MAX_APPS];
    uint8_t app_count;
    int8_t selected_index;
    uint32_t last_input_time;

    // Rendering
    void renderAppGrid();
    void renderAppList();
    void renderSelectedApp();

    // Navigation
    void moveSelection(int8_t direction);
    void launchSelectedApp();
};

#endif // IONOS_LAUNCHER_APP_H

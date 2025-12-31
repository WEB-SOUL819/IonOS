#ifndef IONOS_CLOCK_APP_H
#define IONOS_CLOCK_APP_H

#include "../apps/app_base.h"
#include "../services/time_service.h"

// ============================================================================
// ionOS v1.0 - CLOCK APP
// Digital clock display with date and alarms
// ============================================================================

enum ClockView {
    CLOCK_VIEW_ANALOG = 0,
    CLOCK_VIEW_DIGITAL = 1,
    CLOCK_VIEW_DATE = 2,
    CLOCK_VIEW_ALARM = 3
};

class ClockApp : public App {
public:
    ClockApp();
    ~ClockApp();

    void onLaunch() override;
    void onClose() override;
    void onEvent(const Event &event) override;
    void update() override;
    void render() override;
    const char* getName() override { return "Clock"; }

private:
    ClockView current_view;
    uint32_t last_update_time;

    // View rendering
    void renderDigitalClock();
    void renderAnalogClock();
    void renderDate();
    void renderAlarms();

    // Helper methods
    void switchView(ClockView view);
    void drawAnalogNeedle(uint8_t cx, uint8_t cy, uint8_t length, float angle);
};

#endif // IONOS_CLOCK_APP_H

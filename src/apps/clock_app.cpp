#include "clock_app.h"
#include "../drivers/display_driver.h"
#include "../drivers/button_driver.h"
#include <cmath>

// ============================================================================
// ionOS v1.0 - CLOCK APP IMPLEMENTATION
// ============================================================================

ClockApp::ClockApp() : current_view(CLOCK_VIEW_DIGITAL), last_update_time(0) {}

ClockApp::~ClockApp() {}

void ClockApp::onLaunch() {
    state = APP_STATE_ACTIVE;
    current_view = CLOCK_VIEW_DIGITAL;
    Serial.println("[CLOCK] Clock app launched");
}

void ClockApp::onClose() {
    state = APP_STATE_CLOSING;
    Serial.println("[CLOCK] Clock app closing");
}

void ClockApp::onEvent(const Event &event) {
    switch (event.type) {
        case EVENT_BUTTON_PRESS:
            switch (event.data1) {
                case BTN_ID_UP:
                case BTN_ID_DOWN:
                    // Cycle through clock views
                    switchView((ClockView)((current_view + 1) % 4));
                    break;
                case BTN_ID_LEFT:
                case BTN_ID_RIGHT:
                    // Reserved for time adjustment
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

void ClockApp::update() {
    uint32_t now = millis();
    // Update at 1 Hz
    if (now - last_update_time >= 1000) {
        last_update_time = now;
    }
}

void ClockApp::render() {
    DisplayDriver::clear();

    // Draw title bar
    DisplayDriver::drawString(44, 2, "CLOCK", true);
    DisplayDriver::drawLine(0, 10, 128, 10, true);

    // Draw selected view
    switch (current_view) {
        case CLOCK_VIEW_DIGITAL:
            renderDigitalClock();
            break;
        case CLOCK_VIEW_ANALOG:
            renderAnalogClock();
            break;
        case CLOCK_VIEW_DATE:
            renderDate();
            break;
        case CLOCK_VIEW_ALARM:
            renderAlarms();
            break;
    }

    // Draw footer
    DisplayDriver::drawLine(0, 54, 128, 54, true);
    const char *view_names[] = { "ANALOG", "DIGITAL", "DATE", "ALARM" };
    DisplayDriver::drawString(2, 56, view_names[current_view], false);
}

void ClockApp::renderDigitalClock() {
    DateTime now = TimeService::getTime();
    char time_str[16];
    char date_str[20];

    // Large time display
    sprintf(time_str, "%02d:%02d:%02d", now.hour, now.minute, now.second);
    DisplayDriver::drawString(28, 25, time_str, true);  // 9x18 equivalent

    // Date below
    sprintf(date_str, "%02d/%02d/%04d", now.day, now.month, now.year);
    DisplayDriver::drawString(20, 45, date_str, true);
}

void ClockApp::renderAnalogClock() {
    DateTime now = TimeService::getTime();

    // Draw clock circle
    uint8_t cx = 64;
    uint8_t cy = 32;
    uint8_t radius = 20;

    DisplayDriver::drawCircle(cx, cy, radius, true);

    // Draw hour markers
    for (int i = 0; i < 12; i++) {
        float angle = (i * 30.0f) * 3.14159f / 180.0f;
        uint8_t x = cx + (radius - 3) * cos(angle);
        uint8_t y = cy + (radius - 3) * sin(angle);
        DisplayDriver::drawRect(x - 1, y - 1, 2, 2, true, true);
    }

    // Hour hand
    float hour_angle = ((now.hour % 12) + now.minute / 60.0f) * 30.0f * 3.14159f / 180.0f;
    drawAnalogNeedle(cx, cy, 10, hour_angle);

    // Minute hand
    float min_angle = (now.minute + now.second / 60.0f) * 6.0f * 3.14159f / 180.0f;
    drawAnalogNeedle(cx, cy, 15, min_angle);

    // Center dot
    DisplayDriver::drawRect(cx - 1, cy - 1, 2, 2, true, true);
}

void ClockApp::renderDate() {
    DateTime now = TimeService::getTime();
    char date_str[30];
    const char *months[] = { "JAN", "FEB", "MAR", "APR", "MAY", "JUN",
                            "JUL", "AUG", "SEP", "OCT", "NOV", "DEC" };

    sprintf(date_str, "%s %02d, %04d", months[now.month - 1], now.day, now.year);
    DisplayDriver::drawString(10, 30, date_str, true);

    // Day of week (simplified - always shows a day)
    const char *days[] = { "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN" };
    uint8_t day_idx = now.day % 7;
    DisplayDriver::drawString(50, 45, days[day_idx], true);
}

void ClockApp::renderAlarms() {
    uint8_t alarm_count = TimeService::getAlarmCount();

    if (alarm_count == 0) {
        DisplayDriver::drawString(30, 30, "No alarms set", true);
        return;
    }

    DisplayDriver::drawString(10, 20, "Active Alarms:", true);

    for (int i = 0; i < alarm_count && i < 3; i++) {
        char alarm_str[20];
        sprintf(alarm_str, "%d. Alarm %d", i + 1, i + 1);
        DisplayDriver::drawString(15, 30 + (i * 8), alarm_str, true);
    }
}

void ClockApp::switchView(ClockView view) {
    current_view = view;
    Serial.printf("[CLOCK] Switched to view: %d\n", view);
}

void ClockApp::drawAnalogNeedle(uint8_t cx, uint8_t cy, uint8_t length, float angle) {
    int8_t x2 = cx + length * cos(angle);
    int8_t y2 = cy + length * sin(angle);
    DisplayDriver::drawLine(cx, cy, x2, y2, true);
}

#ifndef IONOS_DISPLAY_DRIVER_H
#define IONOS_DISPLAY_DRIVER_H

#include <stdint.h>
#include <Arduino.h>

// Forward declaration
class U8G2;

// ============================================================================
// ionOS v1.0 - DISPLAY DRIVER (SSD1306 OLED)
// ============================================================================

class DisplayDriver {
public:
    // Initialization & lifecycle
    static bool init();
    static void shutdown();
    static bool isInitialized();

    // Rendering
    static void clear();
    static void display();
    static void setContrast(uint8_t value);
    static void setPowerMode(bool on);

    // Drawing primitives
    static void drawPixel(uint16_t x, uint16_t y, bool color);
    static void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, bool color);
    static void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool fill, bool color);
    static void drawCircle(uint16_t x, uint16_t y, uint16_t radius, bool fill, bool color);
    static void drawString(uint16_t x, uint16_t y, const char *str, bool color);

    // Backlight control
    static void setBacklight(uint8_t brightness);
    static uint8_t getBacklight();

    // Info
    static uint16_t getWidth();
    static uint16_t getHeight();
    static void printDebugInfo();

private:
    static U8G2 *u8g2;
    static bool initialized;
    static uint8_t backlight_level;
};

#endif // IONOS_DISPLAY_DRIVER_H

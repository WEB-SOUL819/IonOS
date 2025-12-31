#ifndef IONOS_FONTS_H
#define IONOS_FONTS_H

#include <stdint.h>

// ============================================================================
// ionOS v1.0 - FONT DEFINITIONS
// Font mappings for U8g2 library (used with SSD1306 OLED)
// ============================================================================

// Forward declare U8g2 font types (from U8g2 library)
// These will be linked at compile time from U8g2

// Primary fonts (used throughout UI)
extern const uint8_t u8g2_font_5x8_tf[];           // Tiny font (5x8)
extern const uint8_t u8g2_font_6x10_mf[];          // Small font (6x10)
extern const uint8_t u8g2_font_7x14_mf[];          // Medium font (7x14)
extern const uint8_t u8g2_font_9x18_mf[];          // Large font (9x18)
extern const uint8_t u8g2_font_10x20_mf[];         // Extra large font (10x20)

// Monospace fonts (for terminal, code display)
extern const uint8_t u8g2_font_5x8_mn[];           // Mono tiny (5x8)
extern const uint8_t u8g2_font_6x12_mn[];          // Mono small (6x12)
extern const uint8_t u8g2_font_7x14_mn[];          // Mono medium (7x14)

// Symbol fonts (icons, special characters)
extern const uint8_t u8g2_font_siji_t_6x10[];      // Siji icon font (6x10)

// Enum for easy font selection
enum FontSize {
    FONT_TINY = 0,      // 5x8
    FONT_SMALL = 1,     // 6x10
    FONT_MEDIUM = 2,    // 7x14
    FONT_LARGE = 3,     // 9x18
    FONT_XLARGE = 4     // 10x20
};

enum FontStyle {
    FONT_NORMAL = 0,
    FONT_MONOSPACE = 1,
    FONT_ICON = 2
};

// Font selection helper
struct Font {
    const uint8_t *data;
    uint8_t width;
    uint8_t height;
    FontSize size;
    FontStyle style;
};

// Font lookup table
class FontManager {
public:
    // Get font by size
    static const uint8_t* getFont(FontSize size, FontStyle style = FONT_NORMAL);

    // Get font dimensions
    static uint8_t getFontWidth(FontSize size);
    static uint8_t getFontHeight(FontSize size);

    // Helper for calculating string width
    static uint16_t getStringWidth(const char *str, FontSize size);

    // Get character at position
    static char getCharAt(const char *str, uint16_t index);

private:
    // Font data array
    static const Font font_table[];
    static const uint8_t font_table_size;

    // Width/height lookup
    static const uint8_t font_widths[];
    static const uint8_t font_heights[];
};

// Font constants for direct use
#define FONT_TINY_WIDTH 5
#define FONT_TINY_HEIGHT 8

#define FONT_SMALL_WIDTH 6
#define FONT_SMALL_HEIGHT 10

#define FONT_MEDIUM_WIDTH 7
#define FONT_MEDIUM_HEIGHT 14

#define FONT_LARGE_WIDTH 9
#define FONT_LARGE_HEIGHT 18

#define FONT_XLARGE_WIDTH 10
#define FONT_XLARGE_HEIGHT 20

#endif // IONOS_FONTS_H

#ifndef IONOS_AUDIO_SERVICE_H
#define IONOS_AUDIO_SERVICE_H

#include <stdint.h>
#include <Arduino.h>

// ============================================================================
// ionOS v1.0 - AUDIO SERVICE
// Sound playback and audio management
// ============================================================================

enum AudioFormat {
    AUDIO_WAV = 0,
    AUDIO_MP3 = 1,
    AUDIO_RAW = 2
};

class AudioService {
public:
    static bool init();
    static void shutdown();

    // Playback control
    static bool play(const char *filepath);
    static bool pause();
    static bool resume();
    static bool stop();
    static bool isPlaying();

    // Volume control (0-255)
    static void setVolume(uint8_t volume);
    static uint8_t getVolume();

    // Tone generation (beep for UI feedback)
    static void beep(uint16_t frequency, uint16_t duration_ms);
    static void playTone(uint16_t frequency, uint16_t duration_ms);

    // Sound effects
    static void soundSuccess();
    static void soundError();
    static void soundWarning();

    // Update
    static void update();

    // Debug
    static void printDebugInfo();

private:
    static uint8_t current_volume;
    static bool is_playing;
};

#endif // IONOS_AUDIO_SERVICE_H

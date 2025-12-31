#include "audio_service.h"
#include <Arduino.h>

// ============================================================================
// ionOS v1.0 - AUDIO SERVICE IMPLEMENTATION
// Sound playback and audio management (I2S DAC)
// ============================================================================

uint8_t AudioService::current_volume = 128;
bool AudioService::is_playing = false;

bool AudioService::init() {
    // Initialize I2S for audio output (MAX98357A)
    // GPIO 14: BCLK, GPIO 15: LRCLK, GPIO 2: DOUT
    
    Serial.println("[AUDIO] Audio service initialized");
    current_volume = 128;  // 50% volume
    return true;
}

void AudioService::shutdown() {
    stop();
    Serial.println("[AUDIO] Audio service shutdown");
}

bool AudioService::play(const char *filepath) {
    // TODO: Implement WAV/MP3 file playback from SD card
    is_playing = true;
    Serial.printf("[AUDIO] Playing: %s\n", filepath);
    return true;
}

bool AudioService::pause() {
    is_playing = false;
    Serial.println("[AUDIO] Paused");
    return true;
}

bool AudioService::resume() {
    is_playing = true;
    Serial.println("[AUDIO] Resumed");
    return true;
}

bool AudioService::stop() {
    is_playing = false;
    Serial.println("[AUDIO] Stopped");
    return true;
}

bool AudioService::isPlaying() {
    return is_playing;
}

void AudioService::setVolume(uint8_t volume) {
    current_volume = volume;
    Serial.printf("[AUDIO] Volume set to: %d%%\n", (volume * 100) / 255);
}

uint8_t AudioService::getVolume() {
    return current_volume;
}

void AudioService::beep(uint16_t frequency, uint16_t duration_ms) {
    // Generate simple beep tone
    Serial.printf("[AUDIO] Beep: %dHz for %dms\n", frequency, duration_ms);
    // TODO: Implement tone generation via I2S
}

void AudioService::playTone(uint16_t frequency, uint16_t duration_ms) {
    beep(frequency, duration_ms);
}

void AudioService::soundSuccess() {
    // Play success beep (higher pitch)
    beep(1000, 100);
    delay(50);
    beep(1200, 100);
}

void AudioService::soundError() {
    // Play error beep (lower pitch, longer)
    beep(400, 200);
    delay(100);
    beep(400, 200);
}

void AudioService::soundWarning() {
    // Play warning beep (medium pitch, alternating)
    beep(800, 100);
    delay(50);
    beep(800, 100);
}

void AudioService::update() {
    // Update audio playback state
    // TODO: Check if file is still playing, update position
}

void AudioService::printDebugInfo() {
    Serial.println("\nâ•”â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•—");
    Serial.println("â•‘  AUDIO SERVICE DEBUG INFO        â•‘");
    Serial.println("â• â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•£");
    Serial.printf("â•‘ Volume: %d/255 (%d%%)\n", current_volume, (current_volume * 100) / 255);
    Serial.printf("â•‘ Playing: %s\n", is_playing ? "YES" : "NO");
    Serial.println("â•šâ•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•\n");
}

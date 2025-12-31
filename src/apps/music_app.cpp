#include "music_app.h"
#include "../drivers/display_driver.h"
#include "../drivers/button_driver.h"
#include "../ui/keyboard.h"
#include "../services/audio_service.h"

MusicApp::MusicApp() :
    song_count(0),
    current_song_index(-1),
    is_playing(false),
    elapsed_time(0) {
    memset(playlist, 0, sizeof(playlist));
}

MusicApp::~MusicApp() {}

void MusicApp::onLaunch() {
    state = APP_STATE_ACTIVE;
    
    // Add example songs (replace with SD card scanning later)
    addSong("/music/song1.wav", "Song One", 180000);
    addSong("/music/song2.wav", "Song Two", 200000);
    addSong("/music/song3.wav", "Song Three", 220000);
    
    if (song_count > 0) {
        current_song_index = 0;
    }
    
    Serial.println("[MUSIC] Music app launched");
}

void MusicApp::onClose() {
    state = APP_STATE_CLOSING;
    AudioService::stop();
    Serial.println("[MUSIC] Music app closing");
}

void MusicApp::onEvent(const Event &event) {
    if (event.type != EVENT_BUTTON_PRESS) return;

    switch (event.data1) {
        case BTN_ID_UP:
            previousSong();
            break;
            
        case BTN_ID_DOWN:
            nextSong();
            break;
            
        case BTN_ID_LEFT: {
            // Volume down
            uint8_t v = AudioService::getVolume();
            if (v >= 10) {
                AudioService::setVolume(v - 10);
            } else {
                AudioService::setVolume(0);
            }
            break;
        }
        
        case BTN_ID_RIGHT: {
            // Volume up
            uint8_t v = AudioService::getVolume();
            if (v <= 245) {
                AudioService::setVolume(v + 10);
            } else {
                AudioService::setVolume(255);
            }
            break;
        }
        
        case BTN_ID_SELECT:
            if (!is_playing && current_song_index >= 0) {
                playSong(current_song_index);
            } else if (is_playing) {
                pause();
            }
            break;
            
        case BTN_ID_BACK:
            handleBackButton();
            break;
    }
}

void MusicApp::update() {
    if (is_playing) {
        elapsed_time += 50;  // Update every ~50ms
        // In real implementation, query actual playback position from AudioService
    }
}

void MusicApp::render() {
    DisplayDriver::clear();
    
    // Title bar
    DisplayDriver::drawString(40, 2, "MUSIC", true);
    DisplayDriver::drawLine(0, 10, 128, 10, true);

    // Check if songs exist
    if (song_count == 0) {
        DisplayDriver::drawString(15, 30, "No songs found", true);
        DisplayDriver::drawString(10, 42, "Press SELECT to search", false);
        return;
    }

    // Render playlist (show first 3 songs)
    renderPlaylist();
    
    // Render now playing info at bottom
    renderNowPlaying();
}

void MusicApp::addSong(const char *filename, const char *title, uint32_t duration) {
    if (song_count >= MAX_SONGS) {
        Serial.printf("[MUSIC] Playlist full! Max %d songs\n", MAX_SONGS);
        return;
    }
    
    playlist[song_count].filename = filename;
    playlist[song_count].title = title;
    playlist[song_count].duration_ms = duration;
    song_count++;
    
    Serial.printf("[MUSIC] Added song: %s (%d ms)\n", title, duration);
}

void MusicApp::removeSong(uint8_t index) {
    if (index >= song_count) return;
    
    for (uint8_t i = index; i < song_count - 1; i++) {
        playlist[i] = playlist[i + 1];
    }
    song_count--;
    
    if (current_song_index == (int8_t)index) {
        current_song_index = -1;
    }
}

uint8_t MusicApp::getSongCount() {
    return song_count;
}

void MusicApp::playSong(uint8_t index) {
    if (index >= song_count) return;
    
    if (AudioService::play(playlist[index].filename)) {
        is_playing = true;
        current_song_index = index;
        elapsed_time = 0;
        Serial.printf("[MUSIC] Playing: %s\n", playlist[index].title);
    } else {
        Serial.printf("[MUSIC] Failed to play: %s\n", playlist[index].title);
    }
}

void MusicApp::nextSong() {
    if (song_count == 0) return;
    
    int8_t idx = current_song_index;
    idx++;
    if (idx >= song_count) idx = 0;
    
    playSong(idx);
}

void MusicApp::previousSong() {
    if (song_count == 0) return;
    
    int8_t idx = current_song_index;
    idx--;
    if (idx < 0) idx = song_count - 1;
    
    playSong(idx);
}

void MusicApp::pause() {
    if (!is_playing) return;
    
    AudioService::pause();
    is_playing = false;
    Serial.println("[MUSIC] Paused");
}

void MusicApp::resume() {
    if (is_playing) return;
    
    AudioService::resume();
    is_playing = true;
    Serial.println("[MUSIC] Resumed");
}

void MusicApp::renderPlaylist() {
    uint8_t display_count = (song_count < 3) ? song_count : 3;
    
    for (uint8_t i = 0; i < display_count; i++) {
        uint8_t y = 14 + i * 10;
        
        if ((int8_t)i == current_song_index) {
            // Highlight current song
            DisplayDriver::drawRect(0, y - 2, 128, 10, false, true);
            
            char line[32];
            if (is_playing) {
                snprintf(line, sizeof(line), "â–¶ %s", playlist[i].title);
            } else {
                snprintf(line, sizeof(line), "â¸ %s", playlist[i].title);
            }
            DisplayDriver::drawString(4, y, line, false);
        } else {
            // Regular song
            DisplayDriver::drawString(4, y, playlist[i].title, true);
        }
    }
}

void MusicApp::renderNowPlaying() {
    DisplayDriver::drawLine(0, 54, 128, 54, true);
    
    char status[40];
    if (current_song_index >= 0 && current_song_index < song_count) {
        if (is_playing) {
            snprintf(status, sizeof(status), "Playing: %s",
                playlist[current_song_index].title);
        } else {
            snprintf(status, sizeof(status), "Paused");
        }
    } else {
        snprintf(status, sizeof(status), "No song selected");
    }
    
    DisplayDriver::drawString(2, 56, status, false);
    
    // Volume indicator
    uint8_t vol = AudioService::getVolume();
    char vol_str[8];
    snprintf(vol_str, sizeof(vol_str), "Vol:%d%%", (vol * 100) / 255);
    DisplayDriver::drawString(100, 56, vol_str, false);
}

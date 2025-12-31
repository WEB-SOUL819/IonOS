#include "music_app.h"
#include "../drivers/display_driver.h"
#include "../drivers/button_driver.h"
#include "../ui/keyboard.h"

MusicApp::MusicApp() :
    song_count(0),
    current_song_index(-1),
    is_playing(false),
    elapsed_time(0) {}

MusicApp::~MusicApp() {}

void MusicApp::onLaunch() {
    state = APP_STATE_ACTIVE;
    // Example songs (replace with SD card scanning later)
    addSong("/music/song1.wav", "Song One", 180000);
    addSong("/music/song2.wav", "Song Two", 200000);
    if (song_count > 0) current_song_index = 0;
}

void MusicApp::onClose() {
    state = APP_STATE_CLOSING;
    AudioService::stop();
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
        case BTN_ID_SELECT:
            if (!is_playing) {
                playSong(current_song_index);
            } else {
                pause();
            }
            break;
        case BTN_ID_RIGHT:
            AudioService::setVolume(AudioService::getVolume() + 10);
            break;
        case BTN_ID_LEFT: {
            uint8_t v = AudioService::getVolume();
            AudioService::setVolume(v > 10 ? v - 10 : 0);
            break;
        }
        case BTN_ID_BACK:
            handleBackButton();
            break;
    }
}

void MusicApp::update() {
    if (is_playing) {
        // For now, just increment elapsed time
        // TODO: read from AudioService when implemented
    }
}

void MusicApp::render() {
    DisplayDriver::clear();
    DisplayDriver::drawString(40, 2, "MUSIC", true);
    DisplayDriver::drawLine(0, 10, 128, 10, true);

    renderPlaylist();
    renderNowPlaying();
}

void MusicApp::addSong(const char *filename, const char *title, uint32_t duration) {
    if (song_count >= MAX_SONGS) return;
    playlist[song_count].filename = filename;
    playlist[song_count].title = title;
    playlist[song_count].duration_ms = duration;
    song_count++;
}

void MusicApp::removeSong(uint8_t index) {
    if (index >= song_count) return;
    for (uint8_t i = index; i < song_count - 1; i++) {
        playlist[i] = playlist[i + 1];
    }
    song_count--;
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
}

void MusicApp::resume() {
    AudioService::resume();
    is_playing = true;
}

void MusicApp::renderPlaylist() {
    for (uint8_t i = 0; i < song_count && i < 4; i++) {
        uint8_t y = 14 + i * 10;
        if ((int8_t)i == current_song_index) {
            DisplayDriver::drawRect(0, y - 2, 128, 10, false, true);
            DisplayDriver::drawString(4, y, playlist[i].title, false);
        } else {
            DisplayDriver::drawString(4, y, playlist[i].title, true);
        }
    }
}

void MusicApp::renderNowPlaying() {
    DisplayDriver::drawLine(0, 54, 128, 54, true);
    if (current_song_index >= 0 && current_song_index < song_count) {
        DisplayDriver::drawString(2, 56,
            is_playing ? "Playing" : "Paused",
            false);
    } else {
        DisplayDriver::drawString(2, 56, "No song selected", false);
    }
}

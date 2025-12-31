#ifndef IONOS_MUSIC_APP_H
#define IONOS_MUSIC_APP_H

#include "../apps/app_base.h"
#include "../services/audio_service.h"

// ============================================================================
// ionOS v1.0 - MUSIC APP
// Simple music/audio player
// ============================================================================

struct Song {
    const char *filename;
    const char *title;
    uint32_t duration_ms;
};

class MusicApp : public App {
public:
    MusicApp();
    ~MusicApp();

    void onLaunch() override;
    void onClose() override;
    void onEvent(const Event &event) override;
    void update() override;
    void render() override;
    const char* getName() override { return "Music"; }

    // Playlist management
    void addSong(const char *filename, const char *title, uint32_t duration);
    void removeSong(uint8_t index);
    uint8_t getSongCount();

private:
    static const uint8_t MAX_SONGS = 20;

    Song playlist[MAX_SONGS];
    uint8_t song_count;
    int8_t current_song_index;
    bool is_playing;
    uint32_t elapsed_time;

    // Playback
    void playSong(uint8_t index);
    void nextSong();
    void previousSong();
    void pause();
    void resume();

    // Rendering
    void renderPlaylist();
    void renderNowPlaying();
};

#endif // IONOS_MUSIC_APP_H

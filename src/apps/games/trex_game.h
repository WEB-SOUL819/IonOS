#ifndef IONOS_TREX_GAME_H
#define IONOS_TREX_GAME_H

#include "../apps/app_base.h"

// ============================================================================
// ionOS v1.0 - T-REX RUNNER GAME
// Chrome-style T-Rex dinosaur running game
// ============================================================================

class TRexGame : public App {
public:
    TRexGame();
    ~TRexGame();

    void onLaunch() override;
    void onClose() override;
    void onEvent(const Event &event) override;
    void update() override;
    void render() override;
    const char* getName() override { return "T-Rex"; }

private:
    enum GameState {
        STATE_MENU = 0,
        STATE_PLAYING = 1,
        STATE_PAUSED = 2,
        STATE_GAME_OVER = 3
    };

    struct Obstacle {
        int16_t x;
        uint8_t type;  // 0=cactus, 1=bird
    };

    GameState game_state;
    int16_t dino_y;
    int16_t dino_vy;  // Vertical velocity
    bool jumping;
    uint16_t score;
    uint32_t last_update_time;
    uint32_t spawn_timer;

    static const uint8_t MAX_OBSTACLES = 4;
    Obstacle obstacles[MAX_OBSTACLES];
    uint8_t obstacle_count;

    // Game logic
    void initGame();
    void updatePhysics();
    void spawnObstacle();
    void updateObstacles();
    void checkCollisions();
    void gameOver();

    // Rendering
    void renderDino();
    void renderObstacle(const Obstacle &obs);
    void renderGameOver();
    void renderMenu();
};

#endif // IONOS_TREX_GAME_H

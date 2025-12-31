#ifndef IONOS_SNAKE_GAME_H
#define IONOS_SNAKE_GAME_H

#include "../apps/app_base.h"

// ============================================================================
// ionOS v1.0 - SNAKE GAME
// Classic snake game for 128x64 OLED
// ============================================================================

enum GameState {
    GAME_PLAYING = 0,
    GAME_PAUSED = 1,
    GAME_OVER = 2,
    GAME_MENU = 3
};

struct Point {
    uint8_t x;
    uint8_t y;
};

class SnakeGame : public App {
public:
    SnakeGame();
    ~SnakeGame();

    void onLaunch() override;
    void onClose() override;
    void onEvent(const Event &event) override;
    void update() override;
    void render() override;
    const char* getName() override { return "Snake"; }

private:
    static const uint8_t GRID_WIDTH = 16;
    static const uint8_t GRID_HEIGHT = 8;
    static const uint8_t MAX_SNAKE_LEN = 64;

    Point snake[MAX_SNAKE_LEN];
    uint8_t snake_len;
    Point food;
    uint8_t direction;    // 0=up, 1=right, 2=down, 3=left
    uint8_t next_direction;
    GameState game_state;
    uint16_t score;
    uint32_t last_move_time;

    // Game logic
    void initGame();
    void updateGame();
    void moveSnake();
    void checkCollision();
    void generateFood();
    void gameOver();

    // Rendering
    void renderGame();
    void renderGameOver();
    void renderMenu();
};

#endif // IONOS_SNAKE_GAME_H

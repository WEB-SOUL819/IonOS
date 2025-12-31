#include "snake_game.h"
#include "../drivers/display_driver.h"
#include "../drivers/button_driver.h"
#include <stdlib.h>

SnakeGame::SnakeGame() : snake_len(3), direction(1), next_direction(1), game_state(GAME_MENU), score(0), last_move_time(0) {
    initGame();
}

SnakeGame::~SnakeGame() {}

void SnakeGame::onLaunch() {
    state = APP_STATE_ACTIVE;
    initGame();
    Serial.println("[SNAKE] Game launched");
}

void SnakeGame::onClose() {
    state = APP_STATE_CLOSING;
    Serial.println("[SNAKE] Game closing");
}

void SnakeGame::onEvent(const Event &event) {
    if (event.type != EVENT_BUTTON_PRESS) return;

    switch (event.data1) {
        case BTN_ID_UP:
            if (direction != 2) next_direction = 0;
            break;
        case BTN_ID_DOWN:
            if (direction != 0) next_direction = 2;
            break;
        case BTN_ID_LEFT:
            if (direction != 1) next_direction = 3;
            break;
        case BTN_ID_RIGHT:
            if (direction != 3) next_direction = 1;
            break;
        case BTN_ID_SELECT:
            if (game_state == GAME_MENU) game_state = GAME_PLAYING;
            else if (game_state == GAME_PLAYING) game_state = GAME_PAUSED;
            else if (game_state == GAME_PAUSED) game_state = GAME_PLAYING;
            else if (game_state == GAME_OVER) initGame();
            break;
        case BTN_ID_BACK:
            handleBackButton();
            break;
    }
}

void SnakeGame::update() {
    if (game_state != GAME_PLAYING) return;

    uint32_t now = millis();
    if (now - last_move_time < 200) return;  // Move every 200ms
    last_move_time = now;

    direction = next_direction;
    moveSnake();
    checkCollision();
}

void SnakeGame::render() {
    DisplayDriver::clear();
    DisplayDriver::drawString(45, 2, "SNAKE", true);
    DisplayDriver::drawLine(0, 10, 128, 10, true);

    switch (game_state) {
        case GAME_PLAYING:
        case GAME_PAUSED:
            renderGame();
            break;
        case GAME_OVER:
            renderGameOver();
            break;
        case GAME_MENU:
            renderMenu();
            break;
    }

    // Draw score
    char score_str[16];
    sprintf(score_str, "Score: %d", score);
    DisplayDriver::drawString(2, 56, score_str, false);
}

void SnakeGame::initGame() {
    snake_len = 3;
    snake[0] = {8, 4};
    snake[1] = {7, 4};
    snake[2] = {6, 4};
    direction = 1;
    next_direction = 1;
    score = 0;
    game_state = GAME_MENU;
    generateFood();
}

void SnakeGame::moveSnake() {
    // Shift snake body
    for (int i = snake_len - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    // Move head
    switch (direction) {
        case 0: snake[0].y--; break;  // Up
        case 1: snake[0].x++; break;  // Right
        case 2: snake[0].y++; break;  // Down
        case 3: snake[0].x--; break;  // Left
    }

    // Wrap around edges
    if (snake[0].x >= GRID_WIDTH) snake[0].x = 0;
    if (snake[0].x < 0) snake[0].x = GRID_WIDTH - 1;
    if (snake[0].y >= GRID_HEIGHT) snake[0].y = 0;
    if (snake[0].y < 0) snake[0].y = GRID_HEIGHT - 1;
}

void SnakeGame::checkCollision() {
    // Check food collision
    if (snake[0].x == food.x && snake[0].y == food.y) {
        if (snake_len < MAX_SNAKE_LEN) {
            snake_len++;
            score += 10;
        }
        generateFood();
    }

    // Check self collision
    for (int i = 1; i < snake_len; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            game_state = GAME_OVER;
        }
    }
}

void SnakeGame::generateFood() {
    food.x = rand() % GRID_WIDTH;
    food.y = rand() % GRID_HEIGHT;
}

void SnakeGame::renderGame() {
    const uint8_t cell_size = 8;

    // Draw grid border
    DisplayDriver::drawRect(0, 12, GRID_WIDTH * cell_size, GRID_HEIGHT * cell_size, true, false);

    // Draw snake
    for (int i = 0; i < snake_len; i++) {
        uint8_t x = snake[i].x * cell_size;
        uint8_t y = 12 + snake[i].y * cell_size;
        DisplayDriver::drawRect(x, y, cell_size, cell_size, true, true);
    }

    // Draw food
    uint8_t fx = food.x * cell_size;
    uint8_t fy = 12 + food.y * cell_size;
    DisplayDriver::drawRect(fx + 2, fy + 2, cell_size - 4, cell_size - 4, true, true);

    // Draw paused indicator
    if (game_state == GAME_PAUSED) {
        DisplayDriver::drawString(50, 40, "PAUSED", true);
    }
}

void SnakeGame::renderGameOver() {
    DisplayDriver::drawString(30, 20, "GAME OVER", true);
    char score_str[20];
    sprintf(score_str, "Final: %d", score);
    DisplayDriver::drawString(20, 35, score_str, true);
    DisplayDriver::drawString(10, 48, "Press SELECT to restart", false);
}

void SnakeGame::renderMenu() {
    DisplayDriver::drawString(20, 25, "SNAKE GAME", true);
    DisplayDriver::drawString(15, 40, "Press SELECT to start", false);
}

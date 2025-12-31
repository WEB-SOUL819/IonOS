#include "trex_game.h"
#include "../drivers/display_driver.h"
#include "../drivers/button_driver.h"
#include <stdlib.h>

TRexGame::TRexGame() : game_state(STATE_MENU), dino_y(40), dino_vy(0), jumping(false),
                       score(0), last_update_time(0), spawn_timer(0), obstacle_count(0) {}

TRexGame::~TRexGame() {}

void TRexGame::onLaunch() {
    state = APP_STATE_ACTIVE;
    initGame();
    Serial.println("[TREX] T-Rex game launched");
}

void TRexGame::onClose() {
    state = APP_STATE_CLOSING;
    Serial.println("[TREX] T-Rex game closing");
}

void TRexGame::onEvent(const Event &event) {
    if (event.type != EVENT_BUTTON_PRESS) return;

    switch (event.data1) {
        case BTN_ID_UP:
        case BTN_ID_SELECT:
            if (game_state == STATE_MENU) game_state = STATE_PLAYING;
            else if (game_state == STATE_PLAYING && !jumping) jumping = true;
            else if (game_state == STATE_GAME_OVER) initGame();
            break;
        case BTN_ID_DOWN:
            if (game_state == STATE_PLAYING) game_state = STATE_PAUSED;
            else if (game_state == STATE_PAUSED) game_state = STATE_PLAYING;
            break;
        case BTN_ID_BACK:
            handleBackButton();
            break;
    }
}

void TRexGame::update() {
    if (game_state != STATE_PLAYING) return;

    uint32_t now = millis();
    if (now - last_update_time < 30) return;  // ~30fps
    last_update_time = now;

    updatePhysics();
    updateObstacles();
    spawnObstacle();
    checkCollisions();
}

void TRexGame::render() {
    DisplayDriver::clear();
    DisplayDriver::drawString(40, 2, "T-REX", true);
    DisplayDriver::drawLine(0, 10, 128, 10, true);

    switch (game_state) {
        case STATE_MENU:
            renderMenu();
            break;
        case STATE_PLAYING:
        case STATE_PAUSED:
            renderDino();
            for (int i = 0; i < obstacle_count; i++) {
                renderObstacle(obstacles[i]);
            }
            if (game_state == STATE_PAUSED) {
                DisplayDriver::drawString(50, 35, "PAUSED", true);
            }
            break;
        case STATE_GAME_OVER:
            renderGameOver();
            break;
    }

    char score_str[16];
    sprintf(score_str, "Score: %d", score);
    DisplayDriver::drawString(2, 56, score_str, false);
}

void TRexGame::initGame() {
    dino_y = 40;
    dino_vy = 0;
    jumping = false;
    score = 0;
    obstacle_count = 0;
    spawn_timer = 0;
    game_state = STATE_MENU;
}

void TRexGame::updatePhysics() {
    const int16_t GROUND_Y = 40;
    const int16_t GRAVITY = 2;
    const int16_t JUMP_VELOCITY = -15;

    if (jumping) {
        dino_vy = JUMP_VELOCITY;
        jumping = false;
    }

    dino_vy += GRAVITY;
    dino_y += dino_vy;

    if (dino_y >= GROUND_Y) {
        dino_y = GROUND_Y;
        dino_vy = 0;
    }
}

void TRexGame::spawnObstacle() {
    spawn_timer++;
    if (spawn_timer > 60 && obstacle_count < MAX_OBSTACLES) {
        obstacles[obstacle_count].x = 128;
        obstacles[obstacle_count].type = rand() % 2;
        obstacle_count++;
        spawn_timer = 0;
    }
}

void TRexGame::updateObstacles() {
    const int16_t SPEED = 4;

    for (int i = 0; i < obstacle_count; i++) {
        obstacles[i].x -= SPEED;

        if (obstacles[i].x < -10) {
            // Remove off-screen obstacle
            for (int j = i; j < obstacle_count - 1; j++) {
                obstacles[j] = obstacles[j + 1];
            }
            obstacle_count--;
            i--;
            score += 10;
        }
    }
}

void TRexGame::checkCollisions() {
    const uint8_t DINO_X = 10;
    const uint8_t DINO_W = 8;
    const uint8_t DINO_H = 8;

    for (int i = 0; i < obstacle_count; i++) {
        uint8_t obs_w = (obstacles[i].type == 0) ? 6 : 8;
        uint8_t obs_h = (obstacles[i].type == 0) ? 8 : 6;

        if (DINO_X < obstacles[i].x + obs_w &&
            DINO_X + DINO_W > obstacles[i].x &&
            dino_y < 48 + obs_h &&
            dino_y + DINO_H > 48) {
            game_state = STATE_GAME_OVER;
        }
    }
}

void TRexGame::renderDino() {
    DisplayDriver::drawRect(10, dino_y, 8, 8, true, true);
}

void TRexGame::renderObstacle(const Obstacle &obs) {
    if (obs.type == 0) {
        // Cactus
        DisplayDriver::drawRect(obs.x, 48, 6, 8, true, true);
    } else {
        // Bird
        DisplayDriver::drawRect(obs.x, 42, 8, 6, true, true);
    }
}

void TRexGame::renderGameOver() {
    DisplayDriver::drawString(30, 20, "GAME OVER", true);
    char score_str[20];
    sprintf(score_str, "Final: %d", score);
    DisplayDriver::drawString(20, 35, score_str, true);
    DisplayDriver::drawString(10, 48, "Press UP to restart", false);
}

void TRexGame::renderMenu() {
    DisplayDriver::drawString(20, 25, "T-REX RUNNER", true);
    DisplayDriver::drawString(15, 40, "Press UP to start", false);
}

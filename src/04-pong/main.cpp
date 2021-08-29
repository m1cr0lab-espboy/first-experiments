/**
 * ----------------------------------------------------------------------------
 * Tiny Pong Game
 * ----------------------------------------------------------------------------
 */
#include <ESPboy.h>
#include "Splasher.h"
#include "Paddle.h"
#include "Ball.h"
#include "assets.h"

// ----------------------------------------------------------------------------
// Definition of Game structure
// ----------------------------------------------------------------------------

struct Game {

    enum class State : uint8_t {

        BOOT,
        SPLASH,
        START,
        SERVE,
        TOSS,
        PLAY,
        GAME_OVER

    };

    State    state;
    uint32_t timer;

};

// ----------------------------------------------------------------------------
// Definition of global variables
// ----------------------------------------------------------------------------

ESPboy espboy;
LGFX_Sprite fb(&espboy.tft);

Game game;
Splasher splasher;
Paddle robot(TFT_WIDTH >> 1, Paddle::Position::Up);
Paddle human(TFT_WIDTH >> 1, Paddle::Position::Down);
Ball ball(human.x, human.top() - ball.RADIUS - 1);

// ----------------------------------------------------------------------------
// Player interactions
// ----------------------------------------------------------------------------

void waitForStart() {

    if (espboy.readButtons() & 0x10) {

        splasher.hide();
        human.tx = TFT_WIDTH >> 1;
        game.state = Game::State::START;

    }

}

void controlPlayer(bool serving = false) {

    uint8_t read = espboy.readButtons();

         if (read & 0x01) human.moveToLeft();
    else if (read & 0x08) human.moveToRight();
    else if (read & 0x10 && serving) game.state = Game::State::TOSS;

}

// ----------------------------------------------------------------------------
// Control of the opponent
// ----------------------------------------------------------------------------

void controlRobot() {

    if (ball.vy < 0 && ball.y < (TFT_HEIGHT >> 1)) {

        uint8_t epsilon = random(robot.SW2 >> 1, robot.SW2);

             if (ball.x + epsilon < robot.x) robot.moveToLeft();
        else if (ball.x > epsilon + robot.x) robot.moveToRight();

    } else {

        robot.wait(true);

    }

}

// ----------------------------------------------------------------------------
// Game physics handling
// ----------------------------------------------------------------------------

void handleCollision(Paddle &paddle) {
        
    if (ball.right() > paddle.solid_left() && ball.left() < paddle.solid_right()) {

        if (paddle.up) {

            int8_t dy = ball.top() - paddle.y;

            if (dy < 0 && dy > -paddle.H2) {

                ball.y = paddle.y + ball.RADIUS;
                ball.vy *= -1;

            }

        } else {

            int8_t dy = ball.bottom() - paddle.y;

            if (dy > 0 && dy < paddle.H2) {

                ball.y = paddle.y - ball.RADIUS;
                ball.vy *= -1;

            }

        }

    }

}

void checkWalls() {

    if (ball.bottom() < 0 || ball.top() > TFT_HEIGHT) {

        game.timer = millis();
        game.state = Game::State::GAME_OVER;
        return;

    }

    if (ball.left() < 0) {

        ball.x = ball.RADIUS;
        ball.vx *= -1;

    } else if (ball.right() > TFT_WIDTH) {

        ball.x = TFT_WIDTH - ball.RADIUS;
        ball.vx *= -1;

    }

}

// ----------------------------------------------------------------------------
// Game stages
// ----------------------------------------------------------------------------

void boot() {

    if (!espboy.isFading()) {

        splasher.show();
        game.state = Game::State::SPLASH;

    }

}

void splash() {

    splasher.update();

    if (splasher.done) {

        robot.wait(true);
        human.wait(true);
        ball.x = human.x;
        ball.y = human.y - ball.RADIUS - 1;
        waitForStart();

    }

}

void start() {

    splasher.update();

    if (splasher.done) {

        robot.wait(true);
        human.wait();
        ball.x = human.x;
        
        if (human.x == (TFT_WIDTH >> 1)) game.state = Game::State::SERVE;

    }

}

void serve() {

    robot.wait(true);
    controlPlayer(true);
    ball.x = human.x;

}

void toss() {

    ball.toss();
    game.state = Game::State::PLAY;

}

void play() {

    controlRobot();
    controlPlayer();

    ball.move();
    checkWalls();
    handleCollision(ball.vy < 0 ? robot : human);

}

void gameOver() {

    robot.wait();

    if (millis() - game.timer > 500) {

        splasher.show();
        game.state = Game::State::SPLASH;

    }

}

// ----------------------------------------------------------------------------
// Game stage handling
// ----------------------------------------------------------------------------

void update() {

    switch (game.state) {

        case Game::State::BOOT:
            boot();
            break;
        case Game::State::SPLASH:
            splash();
            break;
        case Game::State::START:
            start();
            break;
        case Game::State::SERVE:
            serve();
            break;
        case Game::State::TOSS:
            toss();
            break;
        case Game::State::PLAY:
            play();
            break;
        case Game::State::GAME_OVER:
            gameOver();

    }

}

// ----------------------------------------------------------------------------
// Graphics rendering
// ----------------------------------------------------------------------------

void draw() {

    fb.pushImage(0, 0, TFT_WIDTH, TFT_HEIGHT, BACKGROUND);

    ball.draw(fb);
    robot.draw(fb);
    human.draw(fb);

    if (game.state == Game::State::SPLASH || game.state == Game::State::START) splasher.draw(fb);

    fb.pushSprite(0, 0);

}

// ----------------------------------------------------------------------------
// Initialization
// ----------------------------------------------------------------------------

void setup() {

    espboy.splash();

    fb.createSprite(TFT_WIDTH, TFT_HEIGHT);

    game.state = Game::State::BOOT;
    
}

// ----------------------------------------------------------------------------
// Main control loop
// ----------------------------------------------------------------------------

void loop() {

    espboy.update();

    update();
    draw();

}

/**
 * ----------------------------------------------------------------------------
 * First experiments with ESPboy2
 * ----------------------------------------------------------------------------
 * Copyright (c) 2021 Stéphane Calderoni (https://github.com/m1cr0lab)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 * ----------------------------------------------------------------------------
 */
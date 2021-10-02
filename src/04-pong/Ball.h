/**
 * ----------------------------------------------------------------------------
 * Tiny Pong Game
 * ----------------------------------------------------------------------------
 */
#pragma once

#include <ESPboy.h>
#include "assets.h"

struct Ball {

    static constexpr uint8_t SIZE   = 6;
    static constexpr uint8_t RADIUS = SIZE >> 1;
    static constexpr uint8_t VX     = 3;
    static constexpr uint8_t VY     = 3;

    uint8_t x;
    uint8_t y;
    int8_t  vx;
    int8_t  vy;

    Ball(const uint8_t x, const uint8_t y) : x(x), y(y), vx(0), vy(0) {}

    inline int16_t top()    const { return y - RADIUS; }
    inline int16_t bottom() const { return y + RADIUS; }
    inline int16_t left()   const { return x - RADIUS; }
    inline int16_t right()  const { return x + RADIUS; }

    void toss() {

        vx = random(2) ? -VX : VX;
        vy = -VY;

    }

    void move() {

        x += vx;
        y += vy;

    }

    void draw(LGFX_Sprite &fb) {

        fb.pushImage(
            left(),
            top(),
            SIZE,
            SIZE,
            BALL,
            TRANS_COLOR
        );

    }

};

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
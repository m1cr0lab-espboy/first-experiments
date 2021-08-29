/**
 * ----------------------------------------------------------------------------
 * Tiny Pong Game
 * ----------------------------------------------------------------------------
 */
#pragma once

#include <ESPboy.h>
#include "assets.h"

struct Paddle {

    static constexpr const uint8_t WIDTH       = 32;
    static constexpr const uint8_t HEIGHT      = 8;
    static constexpr const uint8_t MARGIN      = 4;
    static constexpr const uint8_t PADDING     = 4;
    static constexpr const uint8_t SOLID_WIDTH = WIDTH - (PADDING << 1);

    static constexpr const uint8_t W2  = WIDTH >> 1;
    static constexpr const uint8_t H2  = HEIGHT >> 1;
    static constexpr const uint8_t SW2 = SOLID_WIDTH >> 1;
    static constexpr const uint8_t Y   = HEIGHT + MARGIN;

    static constexpr const uint8_t V_MAX = 3;

    enum class Position : uint8_t { Up, Down };

    float_t x;
    float_t y;
    uint8_t tx;

    bool up;
    bool waiting;

    Paddle(uint8_t x, Position pos)
    : x(x)
    , up(pos == Position::Up)
    {
        y = up ? Y : TFT_HEIGHT - 1 - Y;
    }

    inline uint8_t top()         { return up ? y - HEIGHT : y; }
    inline uint8_t left()        { return x - W2; }
    inline uint8_t right()       { return x + W2; }
    inline uint8_t solid_left()  { return x - SW2; }
    inline uint8_t solid_right() { return x + SW2; }

    void moveToLeft()  {

        x = (x > W2 + V_MAX) ? x - V_MAX : W2;
    
    }

    void moveToRight() {

        const uint8_t MW = TFT_WIDTH - W2;

        x = (x + V_MAX < MW) ? x + V_MAX : MW;
    
    }

    void wait(bool loop = false) {

        static bool flag = true;

        if (waiting) {

            float_t dx = tx - x;

            if (abs(dx) > 0.05f) {

                float_t vx = .25f * dx;
                if (abs(vx) > V_MAX ) vx = V_MAX * (vx < 0 ? -1 : 1);
                vx *= .98f;
                x  += vx;

            } else {
                
                x = tx;
                waiting = false;

            }

        } else if (loop) {

            tx      = (TFT_WIDTH >> 1) + (flag ? -1 : 1) * random(WIDTH);
            flag    = !flag;
            waiting = true;

        }

    }

    void draw(LGFX_Sprite &fb) {

        fb.pushImage(
            left(),
            top(),
            WIDTH,
            HEIGHT,
            up ? RED_PADDLE : BLUE_PADDLE,
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
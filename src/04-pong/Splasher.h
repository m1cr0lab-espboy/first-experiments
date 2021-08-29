/**
 * ----------------------------------------------------------------------------
 * Tiny Pong Game
 * ----------------------------------------------------------------------------
 */
#pragma once

#include <ESPboy.h>
#include "assets.h"

struct Splasher {

    static constexpr const uint8_t WIDTH  = 76;
    static constexpr const uint8_t HEIGHT = 19;
    static constexpr const uint8_t X      = (TFT_WIDTH - WIDTH) >> 1;

    enum class FX : uint8_t { SHOW, HIDE };

    float_t y;
    float_t vy;
    int8_t  ty;
    FX      fx;
    bool    done;

    void show() {

        y    = TFT_HEIGHT;
        ty   = (TFT_HEIGHT - HEIGHT) >> 1;
        fx   = FX::SHOW;
        done = false;

    }

    void hide() {

        ty   = -HEIGHT;
        vy   = -1;
        fx   = FX::HIDE;
        done = false;

    }

    void exit() {

        float dy = ty - y;

        if (dy < 0) {

            vy *= 1.2f;
            y  += vy;

        } else done = true;

    }

    void spring() {

        float_t dy = ty - y;
        float_t ay = .1f * dy;

        if (abs(dy) > 0.05f) {

            vy += ay;
            vy *= .85f;
            y  += vy;

        } else done = true;

    }

    void update() {

        if (!done) {

            switch (fx) {

                case FX::SHOW: spring(); break;
                case FX::HIDE: exit();

            }

        }

    }

    void draw(LGFX_Sprite &fb) {

        fb.pushImage(X, y, WIDTH, HEIGHT, SPLASH, TRANS_COLOR);

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
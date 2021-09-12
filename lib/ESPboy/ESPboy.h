/**
 * ----------------------------------------------------------------------------
 * ESPboy library
 * ----------------------------------------------------------------------------
 * Copyright (c) 2021 Stéphane Calderoni (https://github.com/m1cr0lab)
 * ----------------------------------------------------------------------------
 * Primary controller of the ESPboy
 * ----------------------------------------------------------------------------
 */
#pragma once

#include <Adafruit_MCP23X17.h>
#include "NeoPixel.h"
#include "lgfx.h"
#include "espboy-logo.h"

class ESPboy {

    private:

        static constexpr uint8_t _TFT_BRIGHTNESS_MIN  = 32;
        static constexpr uint8_t _TFT_BRIGHTNESS_MAX  = 128;

        struct Fading {

            uint32_t last_us;
            uint8_t  level;
            bool     inc;
            bool     active;

        };
        
        Adafruit_MCP23X17 _mcp;

        uint32_t _frame_count;
        uint32_t _fps;
        Fading   _fading;

        void _initTFT();
        void _initMCP23017();
        void _updateFPS();

        void _drawLogo();
        void _fade();

    public:

        LGFX     tft;
        NeoPixel pixel;

        void begin(bool show_logo = true);
        void update();

        uint32_t fps();
        uint8_t  readButtons();

        bool fading();
        void fadeIn();
        void fadeOut();

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
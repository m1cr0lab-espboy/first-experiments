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

#include <Arduino.h>
#include <Adafruit_MCP4725.h>
#include <Adafruit_MCP23017.h>
#include "tft-config.h"
#include "espboy-logo.h"

class ESPboy {

    private:

        enum class _Fader : uint8_t { NONE, FADEIN, FADEOUT };

        static constexpr const uint8_t _MCP23017_TFT_CS_PIN = 8;
        static constexpr const uint8_t _MCP23017_I2C_ADDR   = 0;
        static constexpr const uint8_t _MCP4725_I2C_ADDR    = 0x60;

        static constexpr const uint16_t _DAC_VOLTAGE_MIN = 650;  // lower brightness
        static constexpr const uint16_t _DAC_VOLTAGE_MAX = 1000; // upper brightness

        Adafruit_MCP4725  _dac;
        Adafruit_MCP23017 _mcp;

        uint32_t _frame_count;
        uint32_t _fps;

        _Fader   _fader;
        uint16_t _fading;

        void _initMCP23017();
        void _initMCP4725(uint16_t voltage = 4095);
        void _initTFT();
        void _updateFPS();
        void _fade();
        void _drawLogo(LGFX_Sprite &logo);

    public:

        LGFX tft;

        void begin(uint8_t brightness = 0xff);
        void splash();

        void fadeIn();
        void fadeOut();
        bool isFading();

        uint8_t  readButtons();
        uint32_t fps();

        void update();

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
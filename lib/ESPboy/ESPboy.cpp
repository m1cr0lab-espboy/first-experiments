/**
 * ----------------------------------------------------------------------------
 * ESPboy library
 * ----------------------------------------------------------------------------
 * Copyright (c) 2021 Stéphane Calderoni (https://github.com/m1cr0lab)
 * ----------------------------------------------------------------------------
 * Primary controller of the ESPboy
 * ----------------------------------------------------------------------------
 */
#include "ESPboy.h"

void ESPboy::begin(bool show_logo) {

    _frame_count = _fps = 0;

    _initTFT();
    _initMCP23017();

    if (show_logo) {

        tft.setBrightness(0);
        _drawLogo();
        fadeIn();  while (_fading.active) _fade();
        fadeOut(); while (_fading.active) _fade();
        tft.fillScreen(0);
        fadeIn();

    } else tft.setBrightness(0xff);
    
}

void ESPboy::update() {

    button.read(~(_mcp.readGPIOAB() & 0xff));
    
    if (_fading.active) _fade();

    pixel.update();
    
    _updateFPS();

}

void ESPboy::_initTFT() {

    tft.init();

    #if ESPBOY_VSCROLL_FIX

        // ST7735 40MHz overclocking tradeoff:
        // vscroll init setting needs to be replayed
        tft.startWrite();
        tft.writeCommand(0x37); // vscroll command
        tft.writeData16(1);     // vscroll start address
        tft.endWrite();
    
    #endif

}

void ESPboy::_initMCP23017() {

    _mcp.begin_I2C();

    // Buttons
    for (uint8_t i=0; i<8; ++i) _mcp.pinMode(i, INPUT_PULLUP);

    // NeoPixel LED
    pixel.begin(_mcp);
    
}

void ESPboy::_updateFPS() {

    static uint32_t last_sec = 0;
           uint32_t sec      = millis() / 1000;

    if (sec != last_sec) {
        _fps = _frame_count;
        _frame_count = 0;
        last_sec = sec;
    }

    _frame_count++;

}

uint32_t ESPboy::fps() {

    return _fps;

}

uint8_t ESPboy::readButtons() {

    return ~(_mcp.readGPIOAB() & 0xff);

}

void ESPboy::_drawLogo() {

    LGFX_Sprite logo(&tft);

    logo.createSprite(ESPBOY_LOGO_WIDTH, ESPBOY_LOGO_HEIGHT);
    
    logo.pushImage(
        0,
        0,
        ESPBOY_LOGO_WIDTH,
        ESPBOY_LOGO_HEIGHT,
        ESPBOY_LOGO
    );

    logo.pushSprite(
        (TFT_WIDTH  - ESPBOY_LOGO_WIDTH) >> 1,
        (TFT_HEIGHT - ESPBOY_LOGO_HEIGHT) >> 1
    );

    logo.deleteSprite();

}

bool ESPboy::fading() { return _fading.active; }

void ESPboy::fadeIn() {

    _fading.last_us = micros();
    _fading.level   = _TFT_BRIGHTNESS_MIN;
    _fading.inc     = true;
    _fading.active  = true;

}

void ESPboy::fadeOut() {

    _fading.last_us = micros();
    _fading.level   = _TFT_BRIGHTNESS_MAX;
    _fading.inc     = false;
    _fading.active  = true;

}

void ESPboy::_fade() {

    uint32_t now = micros();

    if (now - _fading.last_us > 9999) {

             if ( _fading.inc && _fading.level < _TFT_BRIGHTNESS_MAX) tft.setBrightness(++_fading.level);
        else if (!_fading.inc && _fading.level > _TFT_BRIGHTNESS_MIN) tft.setBrightness(--_fading.level);
        else _fading.active = false;

        _fading.last_us = now;

    }

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
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

void ESPboy::_initMCP23017() {

    _mcp.begin(_MCP23017_I2C_ADDR);

    // Buttons
    for (uint8_t i=0; i<8; ++i) {
        _mcp.pinMode(i, INPUT);
        _mcp.pullUp(i, HIGH);
    }

    // TFT chip select
    _mcp.pinMode(_MCP23017_TFT_CS_PIN, OUTPUT);
    _mcp.digitalWrite(_MCP23017_TFT_CS_PIN, LOW);
    
}

void ESPboy::_initMCP4725(uint16_t voltage) {

    _dac.begin(_MCP4725_I2C_ADDR);
    _dac.setVoltage(voltage, false);
    _fader = _Fader::NONE;

}

void ESPboy::_initTFT() {

    tft.init();

    #if ESPBOY_FAST_SPI
        // ST7735 40MHz overclocking tradeoff:
        // vscroll init setting needs to be replayed
        tft.startWrite();
        tft.writeCommand(0x37); // vscroll command
        tft.writeData16(1);     // vscroll start address
        tft.endWrite();
    #endif

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

void ESPboy::_fade() {

    if (_fader == _Fader::FADEIN) {

        if (_fading < _DAC_VOLTAGE_MAX) {
            _dac.setVoltage(_fading += 5, false);
        } else {
            _dac.setVoltage(4095, false);
            _fader = _Fader::NONE;
        }

    } else if (_fader == _Fader::FADEOUT) {

        if (_fading > _DAC_VOLTAGE_MIN) {
            _dac.setVoltage(_fading -= 5, false);
        } else {
            _dac.setVoltage(0, false);
            _fader = _Fader::NONE;
        }

    }

}

void ESPboy::fadeIn() {

    _fader  = _Fader::FADEIN;
    _fading = _DAC_VOLTAGE_MIN;

}

void ESPboy::fadeOut() {

    _fader  = _Fader::FADEOUT;
    _fading = _DAC_VOLTAGE_MAX;

}

bool ESPboy::isFading() {

    return _fader != _Fader::NONE;

}

void ESPboy::_drawLogo(LGFX_Sprite &logo) {

    logo.pushImage(
        0,
        0,
        ESPBOY_LOGO_WIDTH,
        ESPBOY_LOGO_HEIGHT,
        ESPBOY_LOGO
    );

    logo.pushSprite(
        (TFT_WIDTH - ESPBOY_LOGO_WIDTH) >> 1,
        (TFT_HEIGHT - ESPBOY_LOGO_HEIGHT) >> 1
    );

}

void ESPboy::begin(uint8_t brightness) {

    uint16_t dac_voltage;

         if (brightness == 0)    dac_voltage = 0;
    else if (brightness == 0xff) dac_voltage = 4095;
    else                         dac_voltage = _DAC_VOLTAGE_MIN + brightness * (_DAC_VOLTAGE_MAX - _DAC_VOLTAGE_MIN) / 0xff;

    _initMCP23017();
    _initMCP4725(dac_voltage);
    _initTFT();

    _frame_count = _fps = 0;
    
}

void ESPboy::splash() {

    begin(0);

    LGFX_Sprite logo(&tft);
    logo.createSprite(ESPBOY_LOGO_WIDTH, ESPBOY_LOGO_HEIGHT);

    fadeIn();
    while (_fader != _Fader::NONE) { delay(5); _drawLogo(logo); update(); }
    
    delay(1000);

    fadeOut();
    while (_fader != _Fader::NONE) { delay(5); _drawLogo(logo); update(); }
    
    logo.deleteSprite();
    tft.fillScreen(TFT_BLACK);

    delay(1000);

    fadeIn();

}

uint8_t ESPboy::readButtons() {

    return ~_mcp.readGPIOAB() & 255;

}

uint32_t ESPboy::fps() {

    return _fps;

}

void ESPboy::update() {

    if (_fader != _Fader::NONE) _fade();

    _updateFPS();

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
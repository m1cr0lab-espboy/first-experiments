/**
 * ----------------------------------------------------------------------------
 * Control the display backlight LED with dimming effect
 * ----------------------------------------------------------------------------
 */
#include <ESPboy.h>

ESPboy espboy;

void setup() {

    espboy.begin();

    LGFX_Sprite fb(&espboy.tft);
    
    fb.createSprite(TFT_WIDTH, TFT_HEIGHT);

    fb.drawRect(0, 0, TFT_WIDTH, TFT_HEIGHT, TFT_GREEN);
    fb.setTextColor(TFT_WHITE);
    fb.drawCenterString(F("TFT Backlight"), 64, 40);
    fb.setTextColor(TFT_LIGHTGRAY);
    fb.drawString(F("ESC: fade out"), 26, 64);
    fb.drawString(F("ACT: fade in"),  26, 80);
    fb.pushSprite(0, 0);

    fb.deleteSprite();

}

void loop() {

    espboy.update();

    if (uint8_t read = espboy.readButtons()) {

             if (read & 0x10) espboy.fadeIn();
        else if (read & 0x20) espboy.fadeOut();

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
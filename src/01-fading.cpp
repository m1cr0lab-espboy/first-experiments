/**
 * ----------------------------------------------------------------------------
 * Using the DAC to control the display backlight LED with dimming effect
 * ----------------------------------------------------------------------------
 */
#include <ESPboy.h>

ESPboy espboy;
LGFX_Sprite fb(&espboy.tft);

void setup() {

    espboy.begin(0);
    espboy.fadeIn();

    fb.createSprite(TFT_WIDTH, TFT_HEIGHT);

}

void loop() {

    espboy.update();

    if (uint8_t read = espboy.readButtons()) {
             if (read == 0x10) espboy.fadeIn();
        else if (read == 0x20) espboy.fadeOut();
    }

    fb.drawRect(0, 0, TFT_WIDTH, TFT_HEIGHT, TFT_GREEN);
    
    fb.setTextColor(TFT_WHITE);
    fb.drawString(F("A: fade in"),  31, 56);
    fb.drawString(F("B: fade out"), 31, 72);

    fb.pushSprite(0, 0);

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
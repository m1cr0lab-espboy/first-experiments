/**
 * ----------------------------------------------------------------------------
 * NeoPixel Lighting FX demo
 * ----------------------------------------------------------------------------
 */
#include <ESPboy.h>

ESPboy espboy;
NeoPixel * const neopixel = &espboy.pixel;

uint8_t brightness;

void setup() {

    espboy.begin();

    espboy.tft.drawCenterString(F("NeoPixel Effects"), TFT_WIDTH >> 1, 18);

    uint8_t y;
    espboy.tft.setTextColor(TFT_LIGHTGRAY);
    espboy.tft.drawString(F("LEFT  Slow Flashing"), 8, y  = 42);
    espboy.tft.drawString(F("RIGHT Fast Flashing"), 8, y += 12);
    espboy.tft.drawString(F("UP    Breathe"),       8, y += 12);
    espboy.tft.drawString(F("DOWN  Rainbow"),       8, y += 12);
    espboy.tft.drawString(F("ACT   + Brightness"),  8, y += 12);
    espboy.tft.drawString(F("ESC   - Brightness"),  8, y += 12);

    neopixel->setBrightness(brightness = 0x40);

}

void loop() {

    espboy.update();

    if (const uint8_t read = espboy.readButtons()) {

        static uint16_t hue   = 0;
        static uint32_t color = 0;
        static uint8_t  wait  = 0;

        hue   = (hue + 20 * random(1, 10)) % 360;
        color = neopixel->hsv(hue);
        wait  = random(1, 6) << 1;

             if (read & 0x01) neopixel->flash(color, 100, 0, 1000);
        else if (read & 0x02) neopixel->breathe(color, wait, 0);
        else if (read & 0x04) neopixel->rainbow(wait, 0);
        else if (read & 0x08) neopixel->flash(color, 50, 0, 250);
        else {

            static uint32_t last = millis();
                   uint32_t now  = millis();

            if (now - last > 5) {

                     if ((read & 0x10) && (brightness < 0xff)) neopixel->setBrightness(brightness++);
                else if ((read & 0x20) && (brightness > 0x00)) neopixel->setBrightness(brightness--);

                last = now;

            }

        }

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
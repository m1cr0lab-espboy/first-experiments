#pragma once

#include <ESPboy.h>

class Tilemap {

    private:

        static constexpr uint16_t TILE[] PROGMEM = {

            0b1111111111111111,
            0b1000000000000001,
            0b1000000000000001,
            0b1000000000000001,
            0b1000011000000001,
            0b1000000000000001,
            0b1000000000000001,
            0b1000000000011111,
            0b1111100000000001,
            0b1000000000000001,
            0b1000000000000001,
            0b1000011111100001,
            0b1000000000000001,
            0b1000000000000001,
            0b1000000000000001,
            0b1111111111111111

        };

    public:

        static constexpr uint8_t TILE_SIZE = 8;
        static constexpr uint8_t WIDTH     = TFT_WIDTH  / TILE_SIZE;
        static constexpr uint8_t HEIGHT    = TFT_HEIGHT / TILE_SIZE;

        bool isSolid(uint8_t x, uint8_t y);

        void draw(LGFX_Sprite &fb);

};
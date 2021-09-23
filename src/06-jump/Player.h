#pragma once

#include <ESPboy.h>
#include "Tilemap.h"

class Player {

    private:

        static constexpr uint8_t SIZE       = 16;
        static constexpr uint8_t HIT_POINTS = 1 + SIZE / Tilemap::TILE_SIZE;

        static constexpr uint8_t SPEED = 2;
        static constexpr uint8_t JUMP  = 6;


        float_t _x, _y;
        float_t _vx, _vy;
        bool    _standing;

        void _handleHorizontalCollisions(Tilemap &tilemap);
        void _handleVerticalCollisions(Tilemap &tilemap);

    public:

        Player(uint16_t x, uint16_t y);

        int16_t x();
        int16_t y();
        int16_t right();
        int16_t bottom();
        bool    standing();

        void moveLeft();
        void moveRight();
        void jump();

        void update(Tilemap &tilemap);
        void draw(LGFX_Sprite &fb);

};
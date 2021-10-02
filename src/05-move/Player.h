#pragma once

#include <ESPboy.h>
#include "Tilemap.h"

class Player {

    private:

        static constexpr uint8_t SIZE       = 16;
        static constexpr uint8_t HIT_POINTS = 1 + SIZE / Tilemap::TILE_SIZE;
        static constexpr uint8_t SPEED      = 2;

        float_t _x, _y;
        float_t _vx, _vy;

        void _handleHorizontalCollisions(const Tilemap &tilemap);
        void _handleVerticalCollisions(const Tilemap &tilemap);

    public:

        Player(const uint16_t x, const uint16_t y);

        int16_t x()      const;
        int16_t y()      const;
        int16_t right()  const;
        int16_t bottom() const;

        void moveLeft();
        void moveRight();
        void moveUp();
        void moveDown();

        void update(const Tilemap &tilemap);
        void draw(LGFX_Sprite &fb) const;

};
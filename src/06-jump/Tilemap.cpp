#include "Tilemap.h"

bool Tilemap::isSolid(const uint8_t x, const uint8_t y) const {

    return pgm_read_word(TILE + y) & (0x8000 >> x);

}

void Tilemap::draw(LGFX_Sprite &fb) const {

    for (uint8_t ty = 0; ty < HEIGHT; ++ty) {
        for (uint8_t tx = 0; tx < WIDTH; ++tx) {

            if (isSolid(tx, ty))
                fb.fillRect(tx * TILE_SIZE, ty * TILE_SIZE, TILE_SIZE, TILE_SIZE, 0x34df);

        }
    }

}
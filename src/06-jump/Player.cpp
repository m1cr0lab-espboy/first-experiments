#include "Player.h"

Player::Player(const uint16_t x, const uint16_t y)
: _x(x)
, _y(y)
, _vx(0)
, _vy(0)
, _standing(true)
{}

int16_t Player::x()        const { return _x;            }
int16_t Player::y()        const { return _y;            }
int16_t Player::right()    const { return _x + SIZE - 1; }
int16_t Player::bottom()   const { return _y + SIZE - 1; }
bool    Player::standing() const { return _standing;     }

void Player::moveLeft()  { _vx = -SPEED;                   }
void Player::moveRight() { _vx =  SPEED;                   }
void Player::jump()      { _vy = -JUMP; _standing = false; }

void Player::_handleHorizontalCollisions(const Tilemap &tilemap) {

    // hit points along the y-axis
    uint16_t hpy[HIT_POINTS];

    hpy[0] = _y;
    hpy[HIT_POINTS - 1] = bottom();
    if (HIT_POINTS > 2) for (uint8_t i = 1; i < HIT_POINTS - 1; ++i) hpy[i] = _y + i * Tilemap::TILE_SIZE;

    const uint16_t tx = (_vx < 0 ? _x : right()) / Tilemap::TILE_SIZE;

    for (uint8_t i = 0; i < HIT_POINTS; ++i) {

        const uint16_t ty = hpy[i] / Tilemap::TILE_SIZE;

        if (tilemap.isSolid(tx, ty)) {

            _x = _vx < 0 ? (tx + 1) * Tilemap::TILE_SIZE : tx * Tilemap::TILE_SIZE - SIZE;

            return;

        }
        
    }

}

void Player::_handleVerticalCollisions(const Tilemap &tilemap) {

    // hit points along the x-axis
    uint16_t hpx[HIT_POINTS];

    hpx[0] = _x;
    hpx[HIT_POINTS - 1] = right();
    if (HIT_POINTS > 2) for (uint8_t i = 1; i < HIT_POINTS - 1; ++i) hpx[i] = _x + i * Tilemap::TILE_SIZE;

    const uint16_t ty = (_vy < 0 ? _y : bottom()) / Tilemap::TILE_SIZE;

    for (uint8_t i = 0; i < HIT_POINTS; ++i) {

        const uint16_t tx = hpx[i] / Tilemap::TILE_SIZE;

        if (tilemap.isSolid(tx, ty)) {

            if (_vy < 0) {

                _y = (ty + 1) * Tilemap::TILE_SIZE;   

            } else {

                _y  = ty * Tilemap::TILE_SIZE - SIZE;
                _vy = 0; _standing = true;

            }

            return;

        }

    }

    _standing = false;

}

void Player::update(const Tilemap &tilemap) {

    if (_vx) {

        _x  += _vx; _handleHorizontalCollisions(tilemap);
        _vx *= .8f; if (abs(_vx) < .2f) _vx = 0;

    }

    _y += _vy; _handleVerticalCollisions(tilemap);

    if (!_standing) _vy += .5;

}

void Player::draw(LGFX_Sprite &fb) const {

    fb.fillRect(_x, _y, SIZE, SIZE, 0x07e0);

}
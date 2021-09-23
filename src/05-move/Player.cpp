#include "Player.h"

Player::Player(uint16_t x, uint16_t y)
: _x(x)
, _y(y)
, _vx(0)
, _vy(0)
{}

int16_t Player::x()      { return _x;            }
int16_t Player::y()      { return _y;            }
int16_t Player::right()  { return _x + SIZE - 1; }
int16_t Player::bottom() { return _y + SIZE - 1; }

void Player::moveLeft()  { _vx = -SPEED; }
void Player::moveRight() { _vx =  SPEED; }
void Player::moveUp()    { _vy = -SPEED; }
void Player::moveDown()  { _vy =  SPEED; }

void Player::_handleHorizontalCollisions(Tilemap &tilemap) {

    // hit points along the y-axis
    uint16_t hpy[HIT_POINTS];

    hpy[0] = _y;
    hpy[HIT_POINTS - 1] = bottom();
    if (HIT_POINTS > 2) for (uint8_t i = 1; i < HIT_POINTS - 1; ++i) hpy[i] = _y + i * Tilemap::TILE_SIZE;

    uint16_t tx = (_vx < 0 ? _x : right()) / Tilemap::TILE_SIZE;

    for (uint8_t i = 0; i < HIT_POINTS; ++i) {

        uint16_t ty = hpy[i] / Tilemap::TILE_SIZE;

        if (tilemap.isSolid(tx, ty)) {
            _x = _vx < 0 ? (tx + 1) * Tilemap::TILE_SIZE : tx * Tilemap::TILE_SIZE - SIZE;
            return;
        }
        
    }

}

void Player::_handleVerticalCollisions(Tilemap &tilemap) {

    // hit points along the x-axis
    uint16_t hpx[HIT_POINTS];

    hpx[0] = _x;
    hpx[HIT_POINTS - 1] = right();
    if (HIT_POINTS > 2) for (uint8_t i = 1; i < HIT_POINTS - 1; ++i) hpx[i] = _x + i * Tilemap::TILE_SIZE;

    uint16_t ty = (_vy < 0 ? _y : bottom()) / Tilemap::TILE_SIZE;

    for (uint8_t i = 0; i < HIT_POINTS; ++i) {

        uint16_t tx = hpx[i] / Tilemap::TILE_SIZE;

        if (tilemap.isSolid(tx, ty)) {
            _y = _vy < 0 ? (ty + 1) * Tilemap::TILE_SIZE : ty * Tilemap::TILE_SIZE - SIZE;
            return;
        }

    }

}

void Player::update(Tilemap &tilemap) {

    if (_vx) {

        _x  += _vx; _handleHorizontalCollisions(tilemap);
        _vx *= .8f; if (abs(_vx) < .2f) _vx = 0;

    }

    if (_vy) {

        _y  += _vy; _handleVerticalCollisions(tilemap);
        _vy *= .8f; if (abs(_vy) < .2f) _vy = 0;

    }

}

void Player::draw(LGFX_Sprite &fb) {

    fb.fillRect(_x, _y, SIZE, SIZE, 0xff60);

}
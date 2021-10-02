#include <ESPboy.h>
#include "Tilemap.h"
#include "Player.h"

#define SHOW_FPS 0

ESPboy espboy;
LGFX_Sprite fb(&espboy.tft);

const Tilemap tilemap;
Player player(96, 72);

// ----------------------------------------------------------------------------
// Initialization
// ----------------------------------------------------------------------------

void setup() {

    espboy.begin(false);
    fb.createSprite(TFT_WIDTH, TFT_HEIGHT);

}

// ----------------------------------------------------------------------------
// User controls
// ----------------------------------------------------------------------------

void input() {

    const uint8_t read = espboy.readButtons();

         if (read & 0x01) player.moveLeft();
    else if (read & 0x08) player.moveRight();

         if (read & 0x02) player.moveUp();
    else if (read & 0x04) player.moveDown();

}

// ----------------------------------------------------------------------------
// Game handling
// ----------------------------------------------------------------------------

void update() {

    player.update(tilemap);

}

// ----------------------------------------------------------------------------
// Graphics rendering
// ----------------------------------------------------------------------------

void draw() {

    fb.fillSprite(0);

    tilemap.draw(fb);
    player.draw(fb);

    #if SHOW_FPS
        fb.setTextColor(0);
        fb.drawNumber(espboy.fps(), 1, 2);
        fb.setTextColor(0xffff);
        fb.drawNumber(espboy.fps(), 1, 1);
    #endif
    
    fb.pushSprite(0, 0);

}

// ----------------------------------------------------------------------------
// Main control loop
// ----------------------------------------------------------------------------

void loop() {

    espboy.update();

    input();
    update();
    draw();

}
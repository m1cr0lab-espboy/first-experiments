/**
 * ----------------------------------------------------------------------------
 * TFT screen settings
 * ----------------------------------------------------------------------------
 * Thanks to :
 *   - lovyan03 (https://github.com/lovyan03)
 *   - tobozo   (https://github.com/tobozo)
 * ----------------------------------------------------------------------------
 */
#pragma once

#define LGFX_USE_V1
#include <LovyanGFX.h>

static constexpr const uint8_t TFT_MISO = D6; // GPIO 12
static constexpr const uint8_t TFT_CS   = -1; // wired to MCP23017 (pin 8)
static constexpr const uint8_t TFT_MOSI = D7; // GPIO 13
static constexpr const uint8_t TFT_SCK  = D5; // GPIO 14
static constexpr const uint8_t TFT_DC   = D0; // GPIO 16
static constexpr const uint8_t TFT_RST  = -1;
static constexpr const uint8_t TFT_LED  = -1; // wired to MCP4725 DAC

/**
 * According to the ST7735 specifications, the minimum serial clock cycle is as follows:
 * 
 *   - Serial Clock Cycle (Write) 66 ns
 *   - Serial Clock Cycle (Read) 150 ns
 * 
 * @see https://www.crystalfontz.com/controllers/Sitronix/ST7735S/
 * 
 * In other words, the maximum frequency is as follows:
 * 
 *   - freq_write = 15 MHz
 *   - freq_read  = 6.66 MHz
 * 
 * However, it is possible to overclock the SPI bus, and the following frequencies
 * are generally well supported on the ST7735:
 * 
 *   - freq_write = 27 MHz
 *   - freq_read  = 16 MHz
 * 
 * The SPI clock can be 80 MHz divided by an integer, so:
 * 
 *   - freq_write = 27 MHz will be rounded to 80/3 = 26,67 MHz
 *   - freq_read  = 16 MHz = 80/5
 * 
 * The corresponding setting are as follows:
 * 
 *     TFT_FREQ_WRITE = 27000000
 *     TFT_FREQ_READ  = 16000000
 *     TFT_OFFSET_X   = 2
 *     TFT_OFFSET_Y   = 1
 * 
 * On the ESPboy, we can even (in some cases) overclock the SPI bus with:
 * 
 *   - freq_write = 40 MHz = 80/2
 *   - freq_read  = 20 MHz = 80/4
 * 
 * But in this case, the initialization command sequence is not interpreted correctly
 * and Vertical Scrolling is being executed incorrectly.
 * To fix that, we can apply these settings:
 * 
 *     TFT_FREQ_WRITE = 40000000
 *     TFT_FREQ_READ  = 20000000
 *     TFT_OFFSET_X   = 2
 *     TFT_OFFSET_Y   = 0
 * 
 * In addition, when initializing the TFT screen, we have to send the following
 * commands to fix the vertical alignment:
 * 
 *     tft.startWrite();
 *     tft.writeCommand(0x37); // Vertical Scrolling start address command
 *     tft.writeData16(1);     // start to scroll at line 1 (not 0)
 *     tft.endWrite();
 * 
 * @see https://github.com/lovyan03/LovyanGFX/issues/130#issuecomment-896273398
 */

#ifdef ESPBOY_FAST_SPI

    static constexpr const uint32_t TFT_FREQ_WRITE = 40000000;
    static constexpr const uint32_t TFT_FREQ_READ  = 20000000;
    static constexpr const uint8_t  TFT_OFFSET_X   = 2;
    static constexpr const uint8_t  TFT_OFFSET_Y   = 0;

#else

    static constexpr const uint32_t TFT_FREQ_WRITE = 27000000;
    static constexpr const uint32_t TFT_FREQ_READ  = 16000000;
    static constexpr const uint8_t  TFT_OFFSET_X   = 2;
    static constexpr const uint8_t  TFT_OFFSET_Y   = 1;

#endif

static constexpr const uint8_t  TFT_MEM_WIDTH  = 132;
static constexpr const uint8_t  TFT_MEM_HEIGHT = 132;
static constexpr const uint8_t  TFT_WIDTH      = 128;
static constexpr const uint8_t  TFT_HEIGHT     = 128;

class LGFX : public lgfx::LGFX_Device {

    lgfx::Panel_ST7735S _panel;
    lgfx::Bus_SPI       _bus;

    public:

        LGFX(void) {
        
            {
                auto cfg = _bus.config();

                cfg.spi_mode   = 0;
                cfg.freq_write = TFT_FREQ_WRITE;
                cfg.freq_read  = TFT_FREQ_READ;
                cfg.pin_sclk   = TFT_SCK;
                cfg.pin_mosi   = TFT_MOSI;
                cfg.pin_miso   = TFT_MISO;
                cfg.pin_dc     = TFT_DC;

                _bus.config(cfg);
                _panel.setBus(&_bus);
            }

            {
                auto cfg = _panel.config();
                
                cfg.pin_cs           = TFT_CS;
                cfg.pin_rst          = TFT_RST;
                cfg.pin_busy         = -1;
                cfg.memory_width     = TFT_MEM_WIDTH;
                cfg.memory_height    = TFT_MEM_HEIGHT;
                cfg.panel_width      = TFT_WIDTH;
                cfg.panel_height     = TFT_HEIGHT;
                cfg.offset_x         = TFT_OFFSET_X;
                cfg.offset_y         = TFT_OFFSET_Y;
                cfg.offset_rotation  = 2;
                cfg.dummy_read_pixel = 8;
                cfg.dummy_read_bits  = 1;
                cfg.readable         = true;
                cfg.invert           = false;
                cfg.rgb_order        = false;
                cfg.dlen_16bit       = false;
                cfg.bus_shared       = true;
                
                _panel.config(cfg);
            }

            setPanel(&_panel);

        }

};

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
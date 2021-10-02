/**
 * ----------------------------------------------------------------------------
 * TFT screen settings
 * ----------------------------------------------------------------------------
 */
#pragma once

#define LGFX_USE_V1
#include <LovyanGFX.h>

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
 * So:
 * 
 *     TFT_FREQ_WRITE = 40000000
 *     TFT_FREQ_READ  = 20000000
 * 
 * But in this case, the initialization command sequence is not interpreted correctly
 * and Vertical Scrolling is being executed incorrectly.
 * To fix that, we can apply these settings:
 * 
 *     TFT_OFFSET_Y = 0
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

static constexpr uint8_t TFT_WIDTH  = 128;
static constexpr uint8_t TFT_HEIGHT = 128;

class LGFX : public lgfx::LGFX_Device {

    #ifdef ESPBOY_FAST_SPI

        static constexpr uint32_t TFT_FREQ_WRITE = 40000000;

    #else

        static constexpr uint32_t TFT_FREQ_WRITE = 27000000;

    #endif
    
    static constexpr uint32_t TFT_FREQ_READ = 8000000; // 16000000 ?

    static constexpr uint8_t TFT_MISO = D6; // GPIO 12
    static constexpr uint8_t TFT_MOSI = D7; // GPIO 13
    static constexpr uint8_t TFT_SCK  = D5; // GPIO 14
    static constexpr uint8_t TFT_DC   = D0; // GPIO 16

    static constexpr uint8_t TFT_MEM_WIDTH  = 132;
    static constexpr uint8_t TFT_MEM_HEIGHT = 132;
    static constexpr uint8_t TFT_OFFSET_X   = 2;

    #ifdef ESPBOY_VSCROLL_FIX

        static constexpr uint8_t TFT_OFFSET_Y = 0;

    #else

        static constexpr uint8_t TFT_OFFSET_Y = 1;

    #endif

    struct Panel_ESPboy : public lgfx::Panel_ST7735S {

        static constexpr uint32_t I2C_FREQ = 400000;
        static constexpr uint8_t  PIN_SDA  = D2; // GPIO 4
        static constexpr uint8_t  PIN_SCL  = D1; // GPIO 5
        static constexpr uint8_t  PORT     = 0;

        // For CS control
        static constexpr uint8_t MCP23017_ADDR    = 0x20;
        static constexpr uint8_t MCP23017_GPIO_B  = 0x13;
        static constexpr uint8_t MCP23017_IODIR_B = 0x01;

        // For Backlight control
        static constexpr uint8_t MCP4725_ADDR = 0x60;

        void init_cs(void) override {

            lgfx::i2c::init(PORT, PIN_SDA, PIN_SCL);

            // MCP23017 PortB bit0 = CS (set to output)
            lgfx::i2c::writeRegister8(PORT, MCP23017_ADDR, MCP23017_IODIR_B, 0x00, 0xfe, I2C_FREQ);

            cs_control(true);

        }

        void cs_control(const bool level) override {

            // MCP23017 PortB bit0 = CS (set level)
            lgfx::i2c::writeRegister8(PORT, MCP23017_ADDR, MCP23017_GPIO_B, level, 0xfe, I2C_FREQ);
        
        }

        void setBrightness(uint8_t brightness) override {

            if (brightness) brightness = (128 + brightness) >> 2;

            uint8_t buf[2];
            buf[0] = brightness >> 4;
            buf[1] = brightness >> 4 | brightness << 4;

            lgfx::i2c::transactionWrite(PORT, MCP4725_ADDR, buf, 2, I2C_FREQ);
        
        }

    };

    Panel_ESPboy  _panel;
    lgfx::Bus_SPI _bus;

    public:

        LGFX(void) {

            // Bus control settings
        
            {
                auto cfg = _bus.config();

                cfg.spi_mode   = 0;              // Set SPI communication mode (0 ~ 3)
                cfg.spi_3wire  = true;           // Set to true if receiving is done on MOSI pin
                cfg.freq_write = TFT_FREQ_WRITE; // SPI clock for transmission
                cfg.freq_read  = TFT_FREQ_READ;  // SPI clock when receiving
                cfg.pin_sclk   = TFT_SCK;        // Set the SCLK pin number for SPI
                cfg.pin_mosi   = TFT_MOSI;       // Set the MOSI pin number for SPI
                cfg.pin_miso   = TFT_MISO;       // Set the MISO pin number for SPI
                cfg.pin_dc     = TFT_DC;         // Set Data/Command pin number for SPI

                _bus.config(cfg);
                _panel.setBus(&_bus);
            }

            // Display panel control settings

            {
                auto cfg = _panel.config();
                
                cfg.memory_width     = TFT_MEM_WIDTH;  // Maximum width supported by the driver IC
                cfg.memory_height    = TFT_MEM_HEIGHT; // Maximum height supported by the driver IC
                cfg.panel_width      = TFT_WIDTH;      // Actual displayable width
                cfg.panel_height     = TFT_HEIGHT;     // Actual displayable height
                cfg.offset_x         = TFT_OFFSET_X;   // Horizontal offset for panel
                cfg.offset_y         = TFT_OFFSET_Y;   // Vertical offset for panel
                cfg.offset_rotation  = 2;              // Offset of rotation direction value 0~7 (4~7 is upside down)
                cfg.dummy_read_pixel = 9;              // Number of bits in dummy read before pixel readout
                cfg.dummy_read_bits  = 1;              // Number of bits of dummy read before reading out data other than pixels
                cfg.readable         = true;           // Set to true if data readout is enabled
                cfg.bus_shared       = false;          // We are not sharing the SPI bus with an SD card reader
                
                _panel.config(cfg);
            }

            setPanel(&_panel);

        }

};

/**
 * ----------------------------------------------------------------------------
 * Acknowledgements
 * ----------------------------------------------------------------------------
 * Thanks for their valuable help:
 *  - lovyan03 (https://github.com/lovyan03)
 *  - tobozo   (https://github.com/tobozo)
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
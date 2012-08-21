#include <Arduino.h>
#include <SPI.h>
#include <GD.h>

#ifndef __AVR__
#include "font8x8.h"
#endif

namespace {

uint8_t stretch[16] =
{
    0x00, 0x03, 0x0c, 0x0f,
    0x30, 0x33, 0x3c, 0x3f,
    0xc0, 0xc3, 0xcc, 0xcf,
    0xf0, 0xf3, 0xfc, 0xff
};

}

// Font loading from assets, see http://excamera.com/sphinx/gameduino/samples/assetlibrary/index.html
void assetASCII()
{
#ifdef __AVR__
    Asset a;
    a.open("fonts", "font8x8", NULL);
#endif

    for (uint16_t i=0; i<768; ++i)
    {
        uint8_t b;

#ifndef __AVR__
        b = pgm_read_byte(font8x8 + i);
#else
        a.read(&b, 1);
#endif
        uint8_t h = stretch[b >> 4];
        uint8_t l = stretch[b & 0xf];
        GD.wr(0x1000 + (16 * ' ') + (2 * i), h);
        GD.wr(0x1000 + (16 * ' ') + (2 * i) + 1, l);
    }

    for (uint8_t i = 0x20; i < 0x80; ++i)
    {
        GD.setpal(4 * i + 0, TRANSPARENT);
        GD.setpal(4 * i + 3, RGB(0, 0, 0));
    }

//    GD.fill(RAM_PIC, ' ', 4096);
}


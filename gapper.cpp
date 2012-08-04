#include <SPI.h>
#include <GD.h>

#include "gfx.h"

enum
{
    LINE_VERT=1,
    LINE_HORIZ,
    LINE_VERT_MARKED,
    LINE_HORIZ_MARKED,
    CORNER_TOP_LEFT,
    CORNER_TOP_RIGHT,
    CORNER_TOP_LEFT_MARKED,
    CORNER_TOP_RIGHT_MARKED,
    CORNER_BOTTOM_LEFT,
    CORNER_BOTTOM_RIGHT,
    CORNER_BOTTOM_LEFT_MARKED,
    CORNER_BOTTOM_RIGHT_MARKED,
    TEE_VERT_LEFT,
    TEE_VERT_RIGHT,
    TEE_VERT_LEFT_MARKED,
    TEE_VERT_RIGHT_MARKED,
    TEE_HORIZ_TOP,
    TEE_HORIZ_TOP_MARKED,
    CROSS,
    CROSS_MARKED,
    TEE_HORIZ_BOTTOM,
    TEE_HORIZ_BOTTOM_MARKED
};

uint16_t atxy(uint8_t x, uint8_t y)
{
    // Copied from frogger tutorial
    return RAM_PIC + 64 * y + x;
}

void drawLevel(uint8_t gridsize)
{
    const uint16_t chwidth = 400 / 8, chheight = 300 / 8;
    
    for (uint16_t x=1; x<chwidth; x += gridsize)
    {
        const bool hasleft = (x > 1), hasright = ((x + gridsize) < chwidth);
        
        for (uint16_t y=1; y<chheight; y += gridsize)
        {
            const bool hasup = (y > 1), hasdown = ((y + gridsize) < chheight);
            
            if (!hasleft)
            {
                if (!hasup)
                    GD.wr(atxy(x, y), CORNER_TOP_LEFT);
                else if (!hasdown)
                    GD.wr(atxy(x, y), CORNER_BOTTOM_LEFT);
                else
                    GD.wr(atxy(x, y), TEE_VERT_LEFT);
            }
            else if (!hasright)
            {
                if (!hasup)
                    GD.wr(atxy(x, y), CORNER_TOP_RIGHT);
                else if (!hasdown)
                    GD.wr(atxy(x, y), CORNER_BOTTOM_RIGHT);
                else
                    GD.wr(atxy(x, y), TEE_VERT_RIGHT);
            }
            else if (hasup && hasdown)
                GD.wr(atxy(x, y), CROSS);
            else if (hasup)
                GD.wr(atxy(x, y), TEE_HORIZ_BOTTOM);
            else if (hasdown)
                GD.wr(atxy(x, y), TEE_HORIZ_TOP);
            
            if (hasdown)
            {
                for (uint16_t sy=y+1; sy<(y + gridsize); ++sy)
                    GD.wr(atxy(x, sy), LINE_VERT);
            }
            
            if (hasright)
            {
                for (uint16_t sx=x+1; sx<(x + gridsize); ++sx)
                    GD.wr(atxy(sx, y), LINE_HORIZ);
            }
        }
    }
}

void setup()
{
    Serial.begin(115200);
    
    GD.begin();

    GD.wr16(BG_COLOR, RGB(0, 0, 255));
    GD.wr16(RAM_PAL, 1<<15);
    
    GD.copy(RAM_CHR + 16, lines_chr, sizeof(lines_chr));
    GD.copy(RAM_PAL + 8, lines_pal, sizeof(lines_pal));

#if 0
    int i = RAM_PIC;
    GD.wr(i, CORNER_TOP_LEFT);
    GD.wr(i + 1, LINE_HORIZ);
    GD.wr(i + 2, TEE_HORIZ_TOP);
    GD.wr(i + 3, LINE_HORIZ);
    GD.wr(i + 4, CORNER_TOP_RIGHT);
    
    i += 64;
    GD.wr(i, LINE_VERT);
    GD.wr(i + 2, LINE_VERT);
    GD.wr(i + 4, LINE_VERT);
    
    i += 64;
    GD.wr(i, TEE_VERT_LEFT);
    GD.wr(i + 1, LINE_HORIZ);
    GD.wr(i + 2, CROSS);
    GD.wr(i + 3, LINE_HORIZ);
    GD.wr(i + 4, TEE_VERT_RIGHT);

    i += 64;
    GD.wr(i, LINE_VERT);
    GD.wr(i + 2, LINE_VERT);
    GD.wr(i + 4, LINE_VERT);
    
    i += 64;
    GD.wr(i, CORNER_BOTTOM_LEFT);
    GD.wr(i + 1, LINE_HORIZ);
    GD.wr(i + 2, TEE_HORIZ_BOTTOM);
    GD.wr(i + 3, LINE_HORIZ);
    GD.wr(i + 4, CORNER_BOTTOM_RIGHT);
    
#endif
    GD.copy(PALETTE4A, players_pal, sizeof(players_pal));
    GD.copy(RAM_SPRIMG, players_img, sizeof(players_img));
    
    GD.__wstartspr(0);
    draw_players(12, 12, 0, 0);
    draw_players(400-8, 300-8, 1, 0);
    GD.__end();
    
    drawLevel(5);
}



void loop()
{
    if (Serial.available() > 0)
    {
        const char b = Serial.read();
        Serial.print("Got: ");
        Serial.println(b);
    }
}


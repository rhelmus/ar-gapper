#include <SPI.h>
#include <GD.h>

#include "level.h"

extern CLevel level;

namespace {

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

EPathType getCharPathType(uint8_t x, uint8_t y)
{
    // UNDONE: Check whether tiles are enabled
    const uint8_t chwidth = 400 / 8, chheight = 300 / 8;
    const bool hasleft = (x > 0), hasright = ((x + level.getGridSize()) < chwidth);
    const bool hasup = (y > 0), hasdown = ((y + level.getGridSize()) < chheight);

    if (!hasleft)
    {
        if (!hasup)
            return PATH_CORNER_TOP_LEFT;
        else if (!hasdown)
            return PATH_CORNER_BOTTOM_LEFT;
        else
            return PATH_TEE_VERT_LEFT;
    }
    else if (!hasright)
    {
        if (!hasup)
            return PATH_CORNER_TOP_RIGHT;
        else if (!hasdown)
            return PATH_CORNER_BOTTOM_RIGHT;
        else
            return PATH_TEE_VERT_RIGHT;
    }
    else if (hasup && hasdown)
        return PATH_CROSS;
    else if (hasup)
        return PATH_TEE_HORIZ_BOTTOM;
    else if (hasdown)
        return PATH_TEE_HORIZ_TOP;
}

}


void CLevel::draw()
{
    // UNDONE: Check tiles

    const uint8_t chwidth = 400 / 8, chheight = 300 / 8;

    for (uint8_t x=0; x<chwidth; x += gridSize)
    {
        const bool hasleft = (x > 0), hasright = ((x + gridSize) < chwidth);

        for (uint8_t y=0; y<chheight; y += gridSize)
        {
            const bool hasup = (y > 0), hasdown = ((y + gridSize) < chheight);

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
                for (uint16_t sy=y+1; sy<(y + gridSize); ++sy)
                    GD.wr(atxy(x, sy), LINE_VERT);
            }

            if (hasright)
            {
                for (uint16_t sx=x+1; sx<(x + gridSize); ++sx)
                    GD.wr(atxy(sx, y), LINE_HORIZ);
            }
        }
    }
}

void CLevel::load(uint8_t level)
{
    // UNDONE
    gridSize = 5;

    draw();
}


EPathType getPxPathType(uint16_t x, uint16_t y)
{
    const uint8_t xchar = x / 8, ychar = y / 8;
    const uint8_t xgridoffset = xchar % level.getGridSize();
    const uint8_t ygridoffset = ychar % level.getGridSize();
    const uint8_t xchoffset = x % 8, ychoffset = y % 8;
    const uint8_t chwidth = 400 / 8, chheight = 300 / 8; // UNDONE

    if (xgridoffset > 0)
        return PATH_HORIZONTAL;
    else if (ygridoffset > 0)
        return PATH_VERTICAL;
    else // junction
    {
        EPathType chtype = getCharPathType(xchar, ychar);

        if (chtype == PATH_CORNER_BOTTOM_LEFT)
        {
            if (xchoffset > 5)
                return PATH_HORIZONTAL;
            else if (ychoffset < 4)
                return PATH_VERTICAL;
            else
                return PATH_CORNER_BOTTOM_LEFT;
        }
        else if (chtype == PATH_CORNER_BOTTOM_RIGHT)
        {
            if (xchoffset < 5)
                return PATH_HORIZONTAL;
            else if (ychoffset < 4)
                return PATH_VERTICAL;
            else
                return PATH_CORNER_BOTTOM_RIGHT;
        }
        else if (chtype == PATH_CORNER_TOP_LEFT)
        {
            if (xchoffset > 5)
                return PATH_HORIZONTAL;
            else if (ychoffset > 4)
                return PATH_VERTICAL;
            else
                return PATH_CORNER_TOP_LEFT;
        }
        else if (chtype == PATH_CORNER_TOP_RIGHT)
        {
            if (xchoffset < 5)
                return PATH_HORIZONTAL;
            else if (ychoffset > 4)
                return PATH_VERTICAL;
            else
                return PATH_CORNER_TOP_RIGHT;
        }
        else if (chtype == PATH_TEE_HORIZ_BOTTOM)
        {
            if (xchoffset != 4)
                return PATH_HORIZONTAL;
            else if (ychoffset < 4)
                return PATH_VERTICAL;
            else
                return PATH_TEE_HORIZ_BOTTOM;
        }
        else if (chtype == PATH_TEE_HORIZ_TOP)
        {
            if (xchoffset != 4)
                return PATH_HORIZONTAL;
            else if (ychoffset > 4)
                return PATH_VERTICAL;
            else
                return PATH_TEE_HORIZ_TOP;
        }
        else if (chtype == PATH_TEE_VERT_LEFT)
        {
            if (xchoffset > 5)
                return PATH_HORIZONTAL;
            else if (ychoffset != 4)
                return PATH_VERTICAL;
            else
                return PATH_TEE_VERT_LEFT;
        }
        else if (chtype == PATH_TEE_VERT_RIGHT)
        {
            if (xchoffset < 5)
                return PATH_HORIZONTAL;
            else if (ychoffset != 4)
                return PATH_VERTICAL;
            else
                return PATH_TEE_VERT_RIGHT;
        }
        else if (chtype == PATH_CROSS)
        {
            if (xchoffset != 4)
                return PATH_HORIZONTAL;
            else if (ychoffset != 4)
                return PATH_VERTICAL;
            else
                return PATH_CROSS;
        }
    }
}

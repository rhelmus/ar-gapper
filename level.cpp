#include <SPI.h>
#include <GD.h>

#include "level.h"

namespace {

enum
{
    CHAR_LINE_VERT=1,
    CHAR_LINE_HORIZ,
    CHAR_LINE_VERT_MARKED,
    CHAR_LINE_HORIZ_MARKED,
    CHAR_CORNER_TOP_LEFT,
    CHAR_CORNER_TOP_RIGHT,
    CHAR_CORNER_TOP_LEFT_MARKED,
    CHAR_CORNER_TOP_RIGHT_MARKED,
    CHAR_CORNER_BOTTOM_LEFT,
    CHAR_CORNER_BOTTOM_RIGHT,
    CHAR_CORNER_BOTTOM_LEFT_MARKED,
    CHAR_CORNER_BOTTOM_RIGHT_MARKED,
    CHAR_TEE_VERT_LEFT,
    CHAR_TEE_VERT_RIGHT,
    CHAR_TEE_VERT_LEFT_MARKED,
    CHAR_TEE_VERT_RIGHT_MARKED,
    CHAR_TEE_HORIZ_TOP,
    CHAR_TEE_HORIZ_TOP_MARKED,
    CHAR_CROSS,
    CHAR_CROSS_MARKED,
    CHAR_TEE_HORIZ_BOTTOM,
    CHAR_TEE_HORIZ_BOTTOM_MARKED
};

uint16_t atxy(uint8_t x, uint8_t y)
{
    // Copied from frogger tutorial
    return RAM_PIC + 64 * y + x;
}

EPathType getCharPathType(uint8_t x, uint8_t y)
{
    // UNDONE: Check whether tiles are enabled

    if (getChTileWOffset(x) > 0)
        return PATH_HORIZONTAL;
    else if (getChTileHOffset(y) > 0)
        return PATH_VERTICAL;
    else // junction
    {
        const bool hasleft = (x > 0), hasright = ((x + level.getGridWidth()) < MAX_GRID_WIDTH);
        const bool hasup = (y > 0), hasdown = ((y + level.getGridHeight()) < MAX_GRID_HEIGHT);

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

}


void CLevel::draw()
{
    // UNDONE: Check whether tiles are enabled

    for (uint8_t x=0; x<MAX_GRID_WIDTH; x+=gridWidth)
    {
        for (uint8_t y=0; y<MAX_GRID_HEIGHT; y+=gridHeight)
        {
            const EPathType path = getCharPathType(x, y);

            switch (path)
            {
            case PATH_CORNER_BOTTOM_LEFT: GD.wr(atxy(x, y), CHAR_CORNER_BOTTOM_LEFT); break;
            case PATH_CORNER_BOTTOM_RIGHT: GD.wr(atxy(x, y), CHAR_CORNER_BOTTOM_RIGHT); break;
            case PATH_CORNER_TOP_LEFT: GD.wr(atxy(x, y), CHAR_CORNER_TOP_LEFT); break;
            case PATH_CORNER_TOP_RIGHT: GD.wr(atxy(x, y), CHAR_CORNER_TOP_RIGHT); break;
            case PATH_TEE_HORIZ_BOTTOM: GD.wr(atxy(x, y), CHAR_TEE_HORIZ_BOTTOM); break;
            case PATH_TEE_HORIZ_TOP: GD.wr(atxy(x, y), CHAR_TEE_HORIZ_TOP); break;
            case PATH_TEE_VERT_LEFT: GD.wr(atxy(x, y), CHAR_TEE_VERT_LEFT); break;
            case PATH_TEE_VERT_RIGHT: GD.wr(atxy(x, y), CHAR_TEE_VERT_RIGHT); break;
            case PATH_CROSS: GD.wr(atxy(x, y), CHAR_CROSS); break;
            }

            // UNDONE
            if ((y + gridHeight) < MAX_GRID_HEIGHT)
            {
                for (uint16_t sy=y+1; sy<(y + gridHeight); ++sy)
                    GD.wr(atxy(x, sy), CHAR_LINE_VERT);
            }

            // UNDONE
            if ((x + gridWidth) < MAX_GRID_WIDTH)
            {
                for (uint16_t sx=x+1; sx<(x + gridWidth); ++sx)
                    GD.wr(atxy(sx, y), CHAR_LINE_HORIZ);
            }
        }
    }
}

void CLevel::load(uint8_t level)
{
    memset(markedColumns, 0, sizeof(markedColumns));
    memset(markedRows, 0, sizeof(markedRows));

    // UNDONE
    gridWidth = 8;
    gridHeight = 5;

    draw();
}

void CLevel::markChar(uint8_t x, uint8_t y)
{
    const EPathType path = getCharPathType(x, y);

    // Note: junctions are both at a column and row
    const bool atcol = (path != PATH_HORIZONTAL);
    const bool atrow = (path != PATH_VERTICAL);
    bool marked = false;

    if (atcol)
    {
        const uint8_t col = x / gridWidth;
        if (!(markedColumns[col] & (1<<y)))
        {
            markedColumns[col] |= (1<<y);
            marked = true;
        }
    }

    if (atrow)
    {
        const uint8_t row = y / gridHeight;
        if (!(markedRows[row] & (1<<x)))
        {
            markedRows[row] |= (1<<x);
            marked = true;
        }
    }

    if (marked)
    {
        switch (path)
        {
        case PATH_HORIZONTAL: GD.wr(atxy(x, y), CHAR_LINE_HORIZ_MARKED); break;
        case PATH_VERTICAL: GD.wr(atxy(x, y), CHAR_LINE_VERT_MARKED); break;
        case PATH_CORNER_BOTTOM_LEFT: GD.wr(atxy(x, y), CHAR_CORNER_BOTTOM_LEFT_MARKED); break;
        case PATH_CORNER_BOTTOM_RIGHT: GD.wr(atxy(x, y), CHAR_CORNER_BOTTOM_RIGHT_MARKED); break;
        case PATH_CORNER_TOP_LEFT: GD.wr(atxy(x, y), CHAR_CORNER_TOP_LEFT_MARKED); break;
        case PATH_CORNER_TOP_RIGHT: GD.wr(atxy(x, y), CHAR_CORNER_TOP_RIGHT_MARKED); break;
        case PATH_TEE_HORIZ_BOTTOM: GD.wr(atxy(x, y), CHAR_TEE_HORIZ_BOTTOM_MARKED); break;
        case PATH_TEE_HORIZ_TOP: GD.wr(atxy(x, y), CHAR_TEE_HORIZ_TOP_MARKED); break;
        case PATH_TEE_VERT_LEFT: GD.wr(atxy(x, y), CHAR_TEE_VERT_LEFT_MARKED); break;
        case PATH_TEE_VERT_RIGHT: GD.wr(atxy(x, y), CHAR_TEE_VERT_RIGHT_MARKED); break;
        case PATH_CROSS: GD.wr(atxy(x, y), CHAR_CROSS_MARKED); break;
        }
    }
}


EPathType getPxPathType(uint16_t x, uint16_t y)
{
    const uint8_t xchar = getChFromPx(x), ychar = getChFromPx(y);
    const uint8_t xchoffset = x % CHAR_SIZE, ychoffset = y % CHAR_SIZE;

    if (getChTileWOffset(xchar) > 0)
        return PATH_HORIZONTAL;
    else if (getChTileHOffset(ychar) > 0)
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

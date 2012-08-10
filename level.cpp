#include <SPI.h>
#include <GD.h>

#include "constants.h"
#include "level.h"
#include "utils.h"

namespace {

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
                for (uint8_t sy=y+1; sy<(y + gridHeight); ++sy)
                    GD.wr(atxy(x, sy), CHAR_LINE_VERT);
            }

            // UNDONE
            if ((x + gridWidth) < MAX_GRID_WIDTH)
                GD.fill(atxy(x+1, y), CHAR_LINE_HORIZ, gridWidth-1);
        }
    }
}

void CLevel::markTiles(uint8_t chx, uint8_t chy, EPathType path)
{
    // Positions to be checked relative to given character pos
    bool checkleft = false, checkright = false, checkup = false, checkdown = false;
    bool checkupleft = false;

    switch (path)
    {
    case PATH_HORIZONTAL: checkup = checkdown = true; break;
    case PATH_VERTICAL: checkleft = checkright = true; break;
    case PATH_CORNER_BOTTOM_LEFT: checkup = true; break;
    case PATH_CORNER_BOTTOM_RIGHT: checkleft = true; break;
    case PATH_CORNER_TOP_LEFT: checkright = true; break;
    case PATH_CORNER_TOP_RIGHT: checkleft = true; break;
    case PATH_TEE_HORIZ_BOTTOM: checkleft = checkup = true; break;
    case PATH_TEE_HORIZ_TOP: checkleft = checkdown = true; break;
    case PATH_TEE_VERT_LEFT: checkup = checkdown = true; break;
    case PATH_TEE_VERT_RIGHT: checkupleft = checkleft = true; break;
    case PATH_CROSS: checkupleft = checkup = checkright = true; break;
    }

    if (checkleft)
        checkTile((chx - 1) / gridWidth, chy / gridHeight);
    if (checkright)
        checkTile((chx + 1) / gridWidth, chy / gridHeight);
    if (checkup)
        checkTile(chx / gridWidth, (chy - 1) / gridHeight);
    if (checkdown)
        checkTile(chx / gridWidth, (chy + 1) / gridHeight);
    if (checkupleft)
        checkTile((chx - 1) / gridWidth, (chy - 1) / gridHeight);
}

void CLevel::checkTile(uint8_t tx, uint8_t ty)
{
    if ((tx >= (MAX_GRID_WIDTH / gridWidth)) || (ty >= (MAX_GRID_HEIGHT / gridHeight)))
        return;

    if (!markedTiles[tx][ty])
    {
        const uint8_t chx = tx * gridWidth, chy = ty * gridHeight;
        const uint8_t maxsize = max(gridWidth, gridHeight);

        for (uint8_t i=0; i<maxsize; ++i)
        {
            if (i < gridWidth)
            {
                // Check top
                if (!check64Bit(markedRows[ty], chx + i))
                    return;

                // Check bottom
                if (!check64Bit(markedRows[ty+1], chx + i))
                    return;
            }

            if (i < gridHeight)
            {
                // Check left
                if (!check64Bit(markedColumns[tx], chy + i))
                    return;

                // Check right
                if (!check64Bit(markedColumns[tx+1], chy + i))
                    return;
            }
        }

        markedTiles[tx][ty] = true;

        for (uint8_t y=chy+1; y<(chy + gridHeight); ++y)
            GD.fill(atxy(chx+1, y), CHAR_FILL, gridWidth-1);

        GD.putstr(chx+((gridWidth-1)/2),
                  chy+(gridHeight/2), "50");
    }
}

void CLevel::load(uint8_t level)
{
    memset(markedColumns, 0, sizeof(markedColumns));
    memset(markedRows, 0, sizeof(markedRows));
    memset(markedTiles, 0, sizeof(markedTiles));

    // UNDONE
    gridWidth = 9-1;
    gridHeight = 7-1;

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
        if (!check64Bit(markedColumns[col], y))
        {
            set64Bit(markedColumns[col], y);
            marked = true;
        }
    }

    if (atrow)
    {
        const uint8_t row = y / gridHeight;
        if (!check64Bit(markedRows[row], x))
        {
            set64Bit(markedRows[row], x);
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

    markTiles(x, y, path);
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

#include <SPI.h>
#include <GD.h>

#include "gfx.h"
#include "level.h"
#include "player.h"

bool CPlayer::move()
{
    if (desiredDirection == DIR_NONE) // Waiting on initial movement
        return false;

    const EPathType path = getPxPathType(position.x, position.y);
    Serial.print("pxpath: "); Serial.println(path, DEC);

    if (desiredDirection != currentDirection)
    {
        if (((path == PATH_VERTICAL) &&
            ((desiredDirection == DIR_UP) || (desiredDirection == DIR_DOWN))) ||
            ((path == PATH_HORIZONTAL) &&
            ((desiredDirection == DIR_LEFT) || (desiredDirection == DIR_RIGHT))) ||
            ((path != PATH_VERTICAL) && (path != PATH_HORIZONTAL)))
            currentDirection = desiredDirection;
    }

    if (currentDirection == DIR_NONE)
        return false;

    const uint8_t oldchx = getChFromPx(position.x), oldchy = getChFromPx(position.y);

    if (currentDirection == DIR_LEFT)
    {
        if ((path == PATH_VERTICAL) || (path == PATH_CORNER_BOTTOM_LEFT) ||
            (path == PATH_CORNER_TOP_LEFT) || (path == PATH_TEE_VERT_LEFT))
            return false;
        --position.x;
    }
    else if (currentDirection == DIR_RIGHT)
    {
        if ((path == PATH_VERTICAL) || (path == PATH_CORNER_BOTTOM_RIGHT) ||
            (path == PATH_CORNER_TOP_RIGHT) || (path == PATH_TEE_VERT_RIGHT))
            return false;
        ++position.x;
    }
    else if (currentDirection == DIR_UP)
    {
        if ((path == PATH_HORIZONTAL) || (path == PATH_CORNER_TOP_LEFT) ||
            (path == PATH_CORNER_TOP_RIGHT) || (path == PATH_TEE_HORIZ_TOP))
            return false;
        --position.y;
    }
    else //if (currentDirection == DIR_DOWN)
    {
        if ((path == PATH_HORIZONTAL) || (path == PATH_CORNER_BOTTOM_LEFT) ||
            (path == PATH_CORNER_BOTTOM_RIGHT) || (path == PATH_TEE_HORIZ_BOTTOM))
            return false;
        ++position.y;
    }

    const uint8_t newchx = getChFromPx(position.x), newchy = getChFromPx(position.y);
    if ((newchx != oldchx) || (newchy != oldchy))
        level.markChar(oldchx, oldchy); // Mark char from which we left

    return true;
}

void CPlayer::draw()
{
    GD.__wstartspr(0);
    draw_players(position.x, position.y, 0, 0);
    GD.__end();
}

void CPlayer::frame()
{
    if (move())
        draw();
}

void CPlayer::reset()
{
    currentDirection = desiredDirection = DIR_NONE;
    position.x = position.y = 4;
    draw();
}

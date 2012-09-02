#include <SPI.h>
#include <GD.h>

#include "constants.h"
#include "level.h"
#include "enemy.h"

extern CPlayer player;

namespace
{

uint16_t getDistance(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

}

void CEnemy::frame()
{
    const uint16_t x = getX(), y = getY();
    const EPathType path = getPxPathType(x, y);

    if ((path != PATH_HORIZONTAL) && (path != PATH_VERTICAL))
    {
        const bool canup = ((y > 0) && (getPxPathType(x, y-1) != PATH_NONE));
        const bool candown = (getPxPathType(x, y+1) != PATH_NONE);
        const bool canleft = ((x > 0) && (getPxPathType(x-1, y) != PATH_NONE));
        const bool canright = (getPxPathType(x+1, y) != PATH_NONE);

        const uint16_t px = player.getX(), py = player.getY();
        const uint16_t curdist = getDistance(x, y, px, py);

        if (canup && (getDistance(x, y-1, px, py)) < curdist)
            setDesiredDirection(DIR_UP);
        else if (candown && (getDistance(x, y+1, px, py)) < curdist)
            setDesiredDirection(DIR_DOWN);
        else if (canleft && (getDistance(x-1, y, px, py)) < curdist)
            setDesiredDirection(DIR_LEFT);
        else if (canright && (getDistance(x+1, y, px, py)) < curdist)
            setDesiredDirection(DIR_RIGHT);
    }

    CPlayer::frame();
}

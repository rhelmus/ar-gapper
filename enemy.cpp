#include <SPI.h>
#include <GD.h>

#include "constants.h"
#include "enemy.h"
#include "gfx.h"

bool CEnemy::move()
{
    return false;
}

void CEnemy::draw()
{
    GD.__wstartspr(1);
    draw_players(position.x, position.y, 1, 0);
    GD.__end();
}

void CEnemy::frame()
{
    if (move())
        draw();
}

void CEnemy::reset()
{
    // UNDONE
    position.x = 14;
    position.y = 12;
    draw();
}

#ifndef ENEMY_H
#define ENEMY_H

#include "player.h"

class CEnemy: public CPlayer
{
public:
    CEnemy(uint8_t s) : CPlayer(s) { }

    virtual void frame(void);
};

#endif // ENEMY_H

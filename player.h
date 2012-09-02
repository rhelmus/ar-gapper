#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>

class CPlayer
{
public:
    enum EDirection { DIR_NONE, DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN };

/*private*/protected:
    uint8_t sprite;
    struct { uint16_t x, y; } position;
    EDirection currentDirection, desiredDirection;

    bool move(void);
    void draw(void);

public:
    CPlayer(uint8_t s) : sprite(s) { }

    virtual void frame(void);
    void reset(uint16_t x, uint16_t y);

    uint16_t getX(void) const { return position.x; }
    uint16_t getY(void) const { return position.y; }
    void setDesiredDirection(EDirection dir) { desiredDirection = dir; }
};

#endif // PLAYER_H

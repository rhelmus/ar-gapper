#ifndef ENEMY_H
#define ENEMY_H

class CEnemy
{
    struct { uint16_t x, y; } position;

    bool move(void);
    void draw(void);

public:
    CEnemy(void) { }

    void frame(void);
    void reset(void);
};

#endif // ENEMY_H

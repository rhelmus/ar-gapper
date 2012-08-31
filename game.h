#ifndef GAME_H
#define GAME_H

#include <stdint.h>

class CGame
{
    uint16_t score;
    uint8_t lives, level;

    void updateScore(void);
    void updateLives(void);
    void updateLevel(void);

public:
    CGame(void) { }

    void reset(void);
    void frame(void);

    void addScore(uint16_t s) { score += s; updateScore(); }
    void takeLive(void) { --lives; updateLives(); }
    void setLevel(uint8_t l) { level = l; updateLevel(); }
};

extern CGame game;

#endif // GAME_H

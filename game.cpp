#include <SPI.h>
#include <GD.h>

#include "constants.h"
#include "game.h"

#include <stdlib.h>

extern const char bottomBarText[] PROGMEM;

void CGame::updateScore()
{
    char txt[7];
    itoa(score, txt, 10);
    txt[6] = 0;
    GD.putstr(12+(6-strlen(txt)), SCREEN_HEIGHT_CHAR-1, txt);
}

void CGame::updateLives()
{
    char txt[2];
    itoa(lives, txt, 10);
    txt[1] = 0;
    GD.putstr(28, SCREEN_HEIGHT_CHAR-1, txt);
}

void CGame::updateLevel()
{
    char txt[3];
    itoa(level, txt, 10);
    txt[2] = 0;
    GD.putstr(39+(2-strlen(txt)), SCREEN_HEIGHT_CHAR-1, txt);
}

void CGame::reset()
{
    score = 0;
    lives = 2;
    level = 1;
    updateScore();
    updateLives();
    updateLevel();
}


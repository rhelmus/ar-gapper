#ifndef LEVEL_H
#define LEVEL_H

#include <stdint.h>

#include "constants.h"

enum EPathType
{
    PATH_NONE,

    PATH_HORIZONTAL,
    PATH_VERTICAL,

    PATH_CORNER_TOP_LEFT,
    PATH_CORNER_TOP_RIGHT,
    PATH_CORNER_BOTTOM_LEFT,
    PATH_CORNER_BOTTOM_RIGHT,

    PATH_TEE_VERT_LEFT,
    PATH_TEE_VERT_RIGHT,
    PATH_TEE_HORIZ_TOP,
    PATH_TEE_HORIZ_BOTTOM,

    PATH_CROSS
};

enum
{
    GRID_OFFSET_Y = 1,
    MIN_GRID_WIDTH = 4-1,
    MIN_GRID_HEIGHT = 3-1,
    MAX_GRID_WIDTH = SCREEN_WIDTH / CHAR_SIZE,
    MAX_GRID_HEIGHT = (SCREEN_HEIGHT / CHAR_SIZE) - GRID_OFFSET_Y - 1
};

class CLevel
{
    struct STile
    {
        uint8_t marked : 1;
        uint8_t enabled : 1;
    };

    uint8_t gridWidth, gridHeight;
    uint64_t markedColumns[MAX_GRID_WIDTH / MIN_GRID_WIDTH + 1];
    uint64_t markedRows[MAX_GRID_HEIGHT / MIN_GRID_HEIGHT + 1];
    STile tiles[MAX_GRID_WIDTH / MIN_GRID_WIDTH][MAX_GRID_HEIGHT / MIN_GRID_HEIGHT];

    void draw(void);
    void markTiles(uint8_t chx, uint8_t chy, EPathType path);
    void checkTile(uint8_t tx, uint8_t ty);

public:
    CLevel(void) { }

    void load(uint8_t level);
    void markChar(uint8_t x, uint8_t y);

    uint8_t getGridWidth(void) const { return gridWidth; }
    uint8_t getGridHeight(void) const { return gridHeight; }
    bool tileEnabled(uint8_t tx, uint8_t ty) const { return tiles[tx][ty].enabled; }
};

extern CLevel level;


EPathType getPxPathType(uint16_t x, uint16_t y);
inline uint8_t getChFromPx(uint16_t px) { return px / CHAR_SIZE; }
inline uint8_t getChTileWOffset(uint8_t ch) { return ch % level.getGridWidth(); }
inline uint8_t getChTileHOffset(uint8_t ch) { return (ch - GRID_OFFSET_Y) % level.getGridHeight(); }
inline uint8_t getTileXFromCh(uint8_t ch) { return ch / level.getGridWidth(); }
inline uint8_t getTileYFromCh(uint8_t ch) { return (ch - GRID_OFFSET_Y) / level.getGridHeight(); }
inline uint8_t getChXFromTile(uint8_t tx) { return tx * level.getGridWidth(); }
inline uint8_t getChYFromTile(uint8_t ty) { return ty * level.getGridHeight() + GRID_OFFSET_Y; }


#endif // LEVEL_H

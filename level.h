#ifndef LEVEL_H
#define LEVEL_H

#include <stdint.h>

enum EPathType
{
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

class CLevel
{
    uint8_t gridSize;

    void draw(void);

public:
    CLevel(void) { }

    void load(uint8_t level);

    uint8_t getGridSize(void) const { return gridSize; }
};

EPathType getPathType(uint16_t x, uint16_t y);

#endif // LEVEL_H

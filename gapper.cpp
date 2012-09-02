#include <SPI.h>
#include <GD.h>

#include "j1firmware/bg.h"
#include "constants.h"
#include "enemy.h"
#include "game.h"
#include "gfx.h"
#include "level.h"
#include "player.h"
#include "utils.h"

CGame game;
CLevel level;
CPlayer player(SPRITE_PLAYER);
CEnemy enemy(SPRITE_ENEMY);

const char bottomBarText[] PROGMEM = "      Score[      ]   Lives[ ]   Level[  ]";

enum
{
    TOP_BAR_COLOR = RGB(251, 84, 255),
    TOP_TEXT_COLOR = TOP_BAR_COLOR,
    TOP_BAR_BG_COLOR = RGB(78, 253, 253),

    MIDDLE_BG_COLOR = RGB(255, 255, 255),
    MIDDLE_TEXT_COLOR = RGB(0, 0, 0),

    BOTTOM_BG_COLOR = RGB(0, 0, 0),
    BOTTOM_TEXT_COLOR = RGB(255, 255, 255)
};

void setupGraphics()
{
    // Effective middle background is black (char 0)
    GD.wr16(RAM_PAL + (CHAR_BACKGROUND * 8), RGB(0, 0, 0));

    GD.wr16(RAM_PAL + (CHAR_TRANSPARENT * 8), TRANSPARENT);

    GD.wr16(RAM_PAL + (CHAR_TOPBAR * 8), TOP_BAR_COLOR);

    GD.copy(RAM_CHR + (CHAR_LINE_VERT * 16), lines_chr, sizeof(lines_chr));
    GD.copy(RAM_PAL + (CHAR_LINE_VERT * 8), lines_pal, sizeof(lines_pal));

    GD.copy(PALETTE4A, players_pal, sizeof(players_pal));
    GD.copy(RAM_SPRIMG, players_img, sizeof(players_img));
}

void setupScreenColors()
{
    GD.wr16(COMM+0, TOP_BAR_BG_COLOR); // background top
    GD.wr16(COMM+2, TOP_TEXT_COLOR); // text top
    GD.wr16(COMM+4, MIDDLE_BG_COLOR); // background middle
    GD.wr16(COMM+6, MIDDLE_TEXT_COLOR); // text middle
    GD.wr16(COMM+8, BOTTOM_BG_COLOR); // background bottom
    GD.wr16(COMM+10, BOTTOM_TEXT_COLOR); // text bottom
    GD.microcode(bg_code, sizeof(bg_code));
}

void setupBars()
{
    // top bar: 7 colored chars, "GAPPER" (6 chars), 7 colored chars
    uint8_t w = 20; // 7 + 6 + 7
    uint8_t x = (SCREEN_WIDTH_CHAR-w)/2;
    GD.fill(atxy(x, 0), CHAR_TOPBAR, 7);
    GD.putstr(x+7, 0, "GAPPER");
    GD.fill(atxy(x+13, 0), CHAR_TOPBAR, 7);

    // bottom bar
    GD.fill(atxy(0, SCREEN_HEIGHT_CHAR-1), CHAR_TRANSPARENT, SCREEN_WIDTH_CHAR);

    putstr_P(0, SCREEN_HEIGHT_CHAR-1, bottomBarText);
}

void setup()
{
    Serial.begin(115200);
    
    GD.begin();

    assetASCII();

    setupGraphics();
    setupScreenColors();
    setupBars();
       
    level.load(1);
    enemy.reset((48*8)+4, (30*8)+12); // UNDONE
    player.reset(4, 12); // UNDONE
    game.reset();
}

void loop()
{
    static uint32_t updelay = 0;

    const uint32_t curtime = millis();
    if (updelay < curtime)
    {
        updelay = curtime + 10;

        if (Serial.available() > 0)
        {
            const char b = Serial.read();
            Serial.print("Got: ");
            Serial.println(b);

            switch (b)
            {
            case 'w': player.setDesiredDirection(CPlayer::DIR_UP); break;
            case 'a': player.setDesiredDirection(CPlayer::DIR_LEFT); break;
            case 's': player.setDesiredDirection(CPlayer::DIR_DOWN); break;
            case 'd': player.setDesiredDirection(CPlayer::DIR_RIGHT); break;
            }
        }

        enemy.frame();
        player.frame();
    }
}

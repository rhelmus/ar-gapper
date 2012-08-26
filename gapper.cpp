#include <SPI.h>
#include <GD.h>

#include "j1firmware/bg.h"
#include "constants.h"
#include "gfx.h"
#include "level.h"
#include "player.h"
#include "utils.h"

CLevel level;
CPlayer player;

enum
{
    TOP_BAR_COLOR = RGB(251, 84, 255),
    TOP_TEXT_COLOR = TOP_BAR_COLOR,
    TOP_BAR_BG_COLOR = RGB(78, 253, 253)

    // UNDONE
};

void setupGraphics()
{
    // Effective middle background is black (char 0)
    GD.wr16(RAM_PAL + (CHAR_BACKGROUND * 8), RGB(0, 0, 0));

    // Char to mark tiles, make it transparent to see BG_COLOR
    GD.wr16(RAM_PAL + (CHAR_FILL * 8), TRANSPARENT);

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
    GD.wr16(COMM+4, RGB(255, 255, 255)); // background middle
    GD.wr16(COMM+6, RGB(0, 0, 0)); // text middle
    GD.wr16(COMM+8, RGB(0, 0, 0)); // background bottom
    GD.wr16(COMM+10, RGB(255, 255, 255)); // text bottom
    GD.microcode(bg_code, sizeof(bg_code));
}

void setupBars()
{
    // Top & bottom bars
    GD.fill(atxy(0, 0), CHAR_TOPBAR, SCREEN_WIDTH_CHAR);
    GD.fill(atxy(0, SCREEN_HEIGHT_CHAR-1), CHAR_FILL, SCREEN_WIDTH_CHAR);
    GD.putstr(10, 0, "Heuh");
    GD.putstr(20, SCREEN_HEIGHT_CHAR-1, "Heuh");
}

void setup()
{
    Serial.begin(115200);
    
    GD.begin();

    assetASCII();

    setupGraphics();
    setupScreenColors();
    setupBars();
    
    GD.__wstartspr(0);
//    draw_players(4, 4, 0, 0);
    draw_players(400-8, 300-8, 1, 0);
    GD.__end();
    

    level.load(1);
    player.reset();
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

        player.frame();
    }
}

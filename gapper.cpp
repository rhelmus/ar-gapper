#include <SPI.h>
#include <GD.h>

#include "gfx.h"
#include "level.h"
#include "player.h"

CLevel level;
CPlayer player;

void setup()
{
    Serial.begin(115200);
    
    GD.begin();

    GD.wr16(BG_COLOR, RGB(0, 0, 255));
    GD.wr16(RAM_PAL, 1<<15);
    
    GD.copy(RAM_CHR + 16, lines_chr, sizeof(lines_chr));
    GD.copy(RAM_PAL + 8, lines_pal, sizeof(lines_pal));

    GD.copy(PALETTE4A, players_pal, sizeof(players_pal));
    GD.copy(RAM_SPRIMG, players_img, sizeof(players_img));
    
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

#!/usr/bin/python

import Image
import gameduino.prep as gdprep
 
(dpic, dchr, dpal) = gdprep.encode(Image.open("lines.png"))
hdr = open("gfx.h", "w")
#gdprep.dump(hdr, "lines_pic", dpic)
gdprep.dump(hdr, "lines_chr", dchr)
gdprep.dump(hdr, "lines_pal", dpal)

ir = gdprep.ImageRAM(hdr)
players = gdprep.palettize(Image.open("players.png"), 4)
gdprep.dump(hdr, "players_pal", gdprep.getpal(players))
ir.addsprites("players", (16, 16), players, gdprep.PALETTE4A, center = (8, 8))
gdprep.dump(hdr, "players_img", ir.used())

(dpic, dchr, dpal) = gdprep.encode(Image.open("top-bar.png"))
gdprep.dump(hdr, "topbar_pic", dpic)
gdprep.dump(hdr, "topbar_chr", dchr)
gdprep.dump(hdr, "topbar_pal", dpal)

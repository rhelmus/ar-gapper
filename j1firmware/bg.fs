start-microcode bg

\ COMM interface:
\ COMM+0: background colour top
\ COMM+2: text colour top
\ COMM+4: background colour middle
\ COMM+6: text colour middle
\ COMM+8: background colour bottom
\ COMM+10: text colour bottom

: 1+    d# 1 + ;
: @     dup c@ swap 1+ c@ swab or ;
: !     over swab over 1+ c! c! ;
: setbg ( line -- )
        dup d# 0 = if COMM+0 @ BG_COLOR ! else
        dup d# 8 = if COMM+4 @ BG_COLOR ! else
        dup d# 288 = if COMM+8 @ BG_COLOR ! then then then \ 37-1*8=288
        drop ;
: setppal ( color -- )
    >r
    h# 80 h# 20
    begin
        r@ over d# 4 * d# 3 + d# 2 * h# 2000 + ! \ 0x2000 + 2 * (4 * i + 3)
        1+ 2dup=
    until
    drop drop r> ;

: setpalline ( line -- )
        dup d# 0 = if COMM+2 @ setppal else
        dup d# 8 = if COMM+6 @ setppal else
        dup d# 288 = if COMM+10 @ setppal then then then
        drop ;

: main
    begin
        YLINE c@
        dup setbg
        setpalline
    again
;

end-microcode

\ COMM+0-1 background color top
\ COMM+2-9 palettes top
\ COMM+10-11 background color center
\ COMM+12-19 palettes center
\ COMM+20-21 background color center
\ COMM+22-29 palettes center

start-microcode bg

: 1+    d# 1 + ;
: @     dup c@ swap 1+ c@ swab or ;
: !     over swab over 1+ c! c! ;
: setbg ( line -- )
        dup d# 8 < if COMM+0 @ BG_COLOR ! else
        dup d# 291 < if COMM+2 @ BG_COLOR ! else
        COMM+4 @ BG_COLOR ! then then
        drop ;
: setpal ( line -- )
        dup d# 8 < if COMM+6 @ h# 2000 d# 8 d# 2 * + !  then drop ;

: main
    begin
        YLINE c@
        dup setbg
        setpal
    again
;

end-microcode

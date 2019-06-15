#include <gb/gb.h>
#include <gb/font.h>
#include <stdio.h>
#include "backgroundtiles.c"
#include "backgroundmap.c"
#include "textmap.c"

void main() {
    font_t min_font;

    font_init();
    min_font = font_load(font_min); // 36 tiles
    font_set(min_font);

    set_bkg_data(37, 27, backgroundtiles);
    set_bkg_tiles(0, 0, 40, 18, backgroundmap);

    set_win_tiles(0, 0, 18, 1, textmap);
    move_win(7, 135);

    SHOW_BKG;
    SHOW_WIN;
    DISPLAY_ON;

    while (1) {
        scroll_bkg(1, 0);
        delay(75);
    }
}
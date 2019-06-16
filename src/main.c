#include <gb/gb.h>
#include <gb/font.h>
#include <gb/hardware.h>
#include <stdio.h>
#include "common.c"
#include "playersprites.c"
#include "backgroundtiles.c"
#include "backgroundone.c"
#include "backgroundtwo.c"
#include "backgroundthree.c"
#include "textmap.c"


// FUNCTION DECLARATIONS
void initGame();																// INITIALISE OUR GAME
void update();															        // UPDATE LOGIC
void updatebackground();                                                        // UPDATE BACKGROUND LOGIC
void draw();                                                                    // DRAW LOGIC

// VARIABLE DECLARATIONS - STORED IN RAM
font_t min_font;                                                                // FONT
UINT8 scrollIndex = 23;
UINT8 playerX = 15, playerY = 125;												// PLAYER CO-ORDINATES
UBYTE playerMovement = 0;                                                       // PLAYER MOVEMENT CHECK
UBYTE currentPlayerSpriteIndex = 0;                                             // PLAYER ANIMATION CONTROLLER
UINT8 maxBackgroundCount = 3;
UINT8 currentBackgroundIndex = 1;

void main() { 
    wait_vbl_done();
    initGame();

    // GAME LOOP
    while(1) {
        update();

        SHOW_WIN;
        SHOW_SPRITES;
        SHOW_BKG;

        draw();
        performantdelay(10);
    }
}

void initGame(){
    DISPLAY_ON;																	// TURNS ON THE GAMEBOY LCD
    font_init();                                                                // ENABLES FONTS
    NR52_REG = 0x8F;															// TURN SOUND ON
    NR51_REG = 0x11;															// ENABLE SOUND CHANNELS
    NR50_REG = 0x1F;															// VOLUME MAX = 0x77, MIN = 0x00	

    min_font = font_load(font_min);                                             // LOAD THE MIN FONT
    font_set(min_font);                                                         // USE MIN FONT WHICH IS 36 TILES

    set_win_tiles(0, 0, 32, 1, textmap);                                        // DRAW MY WATERMARK
    move_win(7, 135);                                                           // STOP THE WINDOW LAYER OVERWRITING SPRITES

    set_bkg_data(37, 7, backgroundtiles);

    set_sprite_data(0, 2, playersprites);                                       // LOAD PLAYER SPRITES INTO MEMORY
    set_sprite_tile(0, currentPlayerSpriteIndex);
    
    updatebackground();                                                         // LOAD INITIAL BACKGROUND SCENE INTO VRAM
}

void update() {
    UINT8 canPlayerMove = 1;
    playerMovement = 0;

    canPlayerMove = canplayermove(playerX, playerY, currentBackgroundIndex, scrollIndex, 23);
    if (canPlayerMove == 1) {
        if (joypad() & J_RIGHT) {
            // we only want to check the right for now
            playerMovement = 1;
            playerX+=8;
        }
    } else if (canPlayerMove == 2){
        currentBackgroundIndex++;
        updatebackground();
    } else if (canPlayerMove == 3) {
        if (scrollIndex > 0) {
            scroll_bkg(3, 0);
            scrollIndex--;
        }
    }
}

void updatebackground() {
    // fadeout the background
    fadeout();

    // check we don't go beyond the number of background maps
    if (currentBackgroundIndex > maxBackgroundCount) {
        currentBackgroundIndex = 1;
    }

    // move the player position back to start
    playerX = 15;
    playerY = 125;
    move_sprite(0, playerX, playerY);

    // move the background position back to the start
    scrollIndex = 23;
    move_bkg(0, 0);
    
    switch (currentBackgroundIndex) {
        case 2:
            HIDE_BKG;
            DISPLAY_OFF;
            VBK_REG = 1;
            VBK_REG = 0;
            set_bkg_data(37, 7, backgroundtiles);
            set_bkg_tiles(0, 0, 40, 18, background_2);
            DISPLAY_ON;
            SHOW_BKG;
            break;
        case 3:
            HIDE_BKG;
            DISPLAY_OFF;
            VBK_REG = 1;
            VBK_REG = 0;
            set_bkg_data(37, 7, backgroundtiles);
            set_bkg_tiles(0, 0, 40, 18, background_3);
            DISPLAY_ON;
            SHOW_BKG;
            break;
        case 1:
        default:
            HIDE_BKG;
            DISPLAY_OFF;
            VBK_REG = 1;
            VBK_REG = 0;
            set_bkg_data(37, 7, backgroundtiles);
            set_bkg_tiles(0, 0, 40, 18, background_1);
            DISPLAY_ON;
            SHOW_BKG;
            break;
    }

    // fade the background back in
    fadein();
}

void draw() {
    if (playerMovement == 1) {
        // move the sprite
        move_sprite(0, playerX, playerY);

        // handle sprite animation
        if (currentPlayerSpriteIndex == 0){
            currentPlayerSpriteIndex = 1;
        } else{
            currentPlayerSpriteIndex = 0;
        }

        set_sprite_tile(0, currentPlayerSpriteIndex);
    }
}
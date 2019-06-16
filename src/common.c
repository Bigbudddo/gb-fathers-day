#include <gb/gb.h>
#include <stdio.h>

UINT8 i, j;
UBYTE debug = 0;

UINT8 canplayermove(UINT8 playerx, UINT8 playery) {
    UINT16 indexTLx, indexTLy, tileindexTL;
    UINT8 result = 1;

    indexTLx = (playerx - 8) / 8;
    indexTLy = (playery - 16) / 8;
    tileindexTL = 20 * indexTLy + indexTLx;

    if (debug) {
        printf("%u %u\n",(UINT16)(playerx),(UINT16)(playery));
        printf("%u %u %u\n",(UINT16)indexTLx,(UINT16)indexTLy,(UINT16)tileindexTL);   
    } 

    if (playerx >= 150) {
        // edge of the map
        result = 2;
    }

    return result;
}

void performantdelay(UINT8 numloops){
    UINT8 ii;
    for(ii = 0; ii < numloops; ii++){
        wait_vbl_done();
    }     
}

void fadeout(){
	for(i = 0; i < 4; i++){
		switch(i){
			case 0:
				BGP_REG = 0xE4;
				break;
			case 1:
				BGP_REG = 0xF9;
				break;
			case 2:
				BGP_REG = 0xFE;
				break;
			case 3:
				BGP_REG = 0xFF;	
				break;						
		}
		performantdelay(10);
	}
}

void fadein(){
	for(i = 0; i < 3; i++){
		switch(i){
			case 0:
				BGP_REG = 0xFE;
				break;
			case 1:
				BGP_REG = 0xF9;
				break;
			case 2:
				BGP_REG = 0xE4;
				break;					
		}
		performantdelay(10);
	}
}
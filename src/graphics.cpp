#include "graphics.h"

//This will draw the common elements of the screen.
//Independantly of the ERS mode.
//This has nothing to do with the rev lights or the flag lights.

void drawMainCommon(){
    EVE_start_cmd_burst();
    
    EVE_cmd_dl_burst( DL_BEGIN | EVE_LINES );

    EVE_cmd_dl_burst( LINE_WIDTH(1U*16U) );
    EVE_cmd_dl_burst( COLOR_RGB(255, 255, 255) );

    //Top horizontal line
    EVE_cmd_dl_burst( VERTEX2F(0, 24) );
    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE, 24) );

    //Side vertical lines
    EVE_cmd_dl_burst( VERTEX2F(22, 24) );
    EVE_cmd_dl_burst( VERTEX2F(22, EVE_VSIZE) );

    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - 22, 24) );
    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - 22, EVE_VSIZE) );

    //Middle vertical lines
    EVE_cmd_dl_burst( VERTEX2F(186, 24) );
    EVE_cmd_dl_burst( VERTEX2F(186, EVE_VSIZE) );

    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - 186, 24) );
    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - 186, EVE_VSIZE) );

    //Middle top horizontal line
    EVE_cmd_dl_burst( VERTEX2F(22, 85) );
    EVE_cmd_dl_burst( VERTEX2F(186, 85) );

    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - 22, 85) );
    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - 186, 85) );

    //Middle bottom horizontal line
    EVE_cmd_dl_burst( VERTEX2F(22, EVE_VSIZE - 61) );
    EVE_cmd_dl_burst( VERTEX2F(186, EVE_VSIZE - 61) );

    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - 22, EVE_VSIZE - 61) );
    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - 186, EVE_VSIZE - 61) );

    //Bottom horizontal lines
    EVE_cmd_dl_burst( VERTEX2F(186, EVE_VSIZE - 36) );
    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - 186, EVE_VSIZE - 36) );

    EVE_cmd_dl_burst( VERTEX2F(186, EVE_VSIZE - 72) );
    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - 186, EVE_VSIZE - 72) );

    EVE_cmd_dl_burst( VERTEX2F(186, EVE_VSIZE - 108) );
    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - 186, EVE_VSIZE - 108) );

    EVE_cmd_dl_burst( VERTEX2F(186, EVE_VSIZE - 144) );
    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - 186, EVE_VSIZE - 144) );

    EVE_cmd_dl_burst( DL_END );

    EVE_end_cmd_burst();
}

void drawMainData(int speed, int lap, int gear){
    EVE_start_cmd_burst();
    EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));
    EVE_cmd_number_burst( EVE_HSIZE / 2, 76, 31, EVE_OPT_CENTER, gear);
    EVE_cmd_text_burst(EVE_HSIZE -  40, 11, 22, EVE_OPT_CENTER, "Lap");
    EVE_cmd_number_burst(EVE_HSIZE -  2, 11, 22, EVE_OPT_CENTERY | EVE_OPT_RIGHTX, lap);
    EVE_cmd_number_burst(25, 11, 22, EVE_OPT_CENTER, speed);
    EVE_end_cmd_burst();
}

void drawYellowFlag(){
    EVE_start_cmd_burst();
    EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
    EVE_cmd_dl_burst(COLOR_RGB(200, 180, 0));
    EVE_cmd_dl_burst(VERTEX2F(0, 0));
    EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE, EVE_VSIZE));
    EVE_cmd_dl_burst(DL_END);
    EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));
    EVE_cmd_text_burst(EVE_HSIZE / 2, EVE_VSIZE / 2, 31, EVE_OPT_CENTER, "Yellow Flag");
    EVE_end_cmd_burst();
}

void drawRedFlag(){
    EVE_start_cmd_burst();
    EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
    EVE_cmd_dl_burst(COLOR_RGB(255, 0, 0));
    EVE_cmd_dl_burst(VERTEX2F(0, 0));
    EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE, EVE_VSIZE));
    EVE_cmd_dl_burst(DL_END);
    EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));
    EVE_cmd_text_burst(EVE_HSIZE / 2, EVE_VSIZE / 2, 31, EVE_OPT_CENTER, "Yellow Flag");
    EVE_end_cmd_burst();
}
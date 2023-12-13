#include "graphics.h"

//This will draw the common elements of the screen.
//Independantly of the ERS mode.
//This has nothing to do with the rev lights or the flag lights.

/*
void drawInitLogo(){
    //EVE_memWrite_flash_buffer(0x800000, ptr, 1024);

    EVE_cmd_flashread(0x000000, 0x000000, 4096);

    EVE_start_cmd_burst();
    EVE_cmd_dl_burst(CMD_DLSTART);
    EVE_cmd_dl_burst(DL_CLEAR_COLOR_RGB | BLACK);
    EVE_cmd_dl_burst(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);
    EVE_cmd_dl_burst(DL_VERTEX_FORMAT);
    EVE_end_cmd_burst();

    EVE_cmd_dl( BITMAP_SOURCE(0x000000) );
    EVE_cmd_dl( BITMAP_LAYOUT(EVE_PALETTED, 128, 32) );
    EVE_cmd_dl( BITMAP_SIZE(EVE_NEAREST, EVE_BORDER, EVE_BORDER,32, 32) );
    EVE_cmd_dl( DL_BEGIN | EVE_BITMAPS );
    EVE_cmd_dl( VERTEX2F(0, 0) );
    EVE_cmd_dl( DL_END );

    EVE_start_cmd_burst();
    EVE_cmd_dl_burst(DL_DISPLAY); 
    EVE_cmd_dl_burst(CMD_SWAP);
    EVE_end_cmd_burst();
}
*/

//Draw main page common elements
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
void drawMainData(int speed, int lap, int gear, float deltaTime, float brakeBias, int battery, float lastLapFuel, float lastLapTime){
    EVE_start_cmd_burst();
    EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));

    //Speed
    EVE_cmd_text_burst(0, 9, 20, EVE_OPT_CENTERY, "SPEED");
    EVE_cmd_number_burst(60, 11, 24, EVE_OPT_CENTER, speed);

    //Laps
    EVE_cmd_text_burst(EVE_HSIZE -  2, 11, 20, EVE_OPT_CENTERY | EVE_OPT_RIGHTX, "LAP");
    EVE_cmd_number_burst(EVE_HSIZE -  45, 11, 24, EVE_OPT_CENTER, lap);

    //Gear
    EVE_cmd_number_burst( EVE_HSIZE / 2, 76, 31, EVE_OPT_CENTER, gear);

    //Delta
    EVE_cmd_dl_burst(COLOR_RGB(200, 0, 200));
    String deltaString = secondsToTime(deltaTime);
    EVE_cmd_text_burst( 186 - 4, 54, 31, EVE_OPT_CENTERY | EVE_OPT_RIGHTX, deltaString.c_str());

    //Brake Bias
    EVE_cmd_dl_burst(COLOR_RGB(150, 120, 10));
    EVE_cmd_text_burst( EVE_HSIZE - 104, 54, 31, EVE_OPT_CENTER, String(brakeBias).substring(0, String(brakeBias).indexOf('.') + 2).c_str());

    //Battery
    EVE_cmd_dl_burst(COLOR_RGB(0, 255, 0));
    EVE_cmd_text_burst( EVE_HSIZE / 2, EVE_VSIZE - 90, 30, EVE_OPT_CENTER, String(battery).c_str());
    EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));
    EVE_cmd_text_burst( 190, EVE_VSIZE - 100, 16, EVE_OPT_CENTERY, "BATT");

    //Last Lap Fuel
    EVE_cmd_dl_burst(COLOR_RGB(255, 0, 0));
    EVE_cmd_text_burst( EVE_HSIZE / 2, EVE_VSIZE - 54, 30, EVE_OPT_CENTER, String(lastLapFuel).c_str());
    EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));
    EVE_cmd_text_burst( 190, EVE_VSIZE - 64, 16, EVE_OPT_CENTERY, "LL");

    //Last Lap Time
    EVE_cmd_dl_burst(COLOR_RGB(10, 200, 200));
    EVE_cmd_text_burst( EVE_HSIZE - 104, EVE_VSIZE - 30, 30, EVE_OPT_CENTER, String(lastLapTime).c_str());
    EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));
    EVE_cmd_text_burst( EVE_HSIZE - 186 + 2, EVE_VSIZE - 61 + 8, 16, EVE_OPT_CENTERY, "LAST LAP");

    EVE_end_cmd_burst();
}

void drawYellowFlag(bool flagBlink){
    EVE_start_cmd_burst();
    EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
    if(flagBlink){
        EVE_cmd_dl_burst(COLOR_RGB(250, 180, 0));
    }
    else{
        EVE_cmd_dl_burst(COLOR_RGB(0, 0, 0));
    }
    EVE_cmd_dl_burst(VERTEX2F(0, 0));
    EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE, EVE_VSIZE));
    EVE_cmd_dl_burst(DL_END);
     if(flagBlink){
        EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));
    }
    else{
        EVE_cmd_dl_burst(COLOR_RGB(250, 180, 0));
    }
    EVE_cmd_text_burst(EVE_HSIZE / 2, EVE_VSIZE / 2, 31, EVE_OPT_CENTER, "Yellow Flag");
    EVE_end_cmd_burst();
}

void drawRedFlag(bool flagBlink){
    EVE_start_cmd_burst();
    EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
    if(flagBlink){
        EVE_cmd_dl_burst(COLOR_RGB(200, 0, 0));
    }
    else{
        EVE_cmd_dl_burst(COLOR_RGB(0, 0, 0));
    }
    EVE_cmd_dl_burst(VERTEX2F(0, 0));
    EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE, EVE_VSIZE));
    EVE_cmd_dl_burst(DL_END);
    if(flagBlink){
        EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));
    }
    else{
        EVE_cmd_dl_burst(COLOR_RGB(200, 0, 0));
    }
    EVE_cmd_text_burst(EVE_HSIZE / 2, EVE_VSIZE / 2, 31, EVE_OPT_CENTER, "Red Flag");
    EVE_end_cmd_burst();
}

void drawERSMode0(){

}

//This function convert a float delta time ( negative or positive ) to a string.
String secondsToTime(float seconds){
    String timeString = "";
    int minutes = 0;
    int secondsInt = 0;
    int milliseconds = 0;
    if(seconds < 0){
        timeString += "-";
        seconds *= -1;
    }
    else{
        timeString += "+";
    }
    minutes = seconds / 60;
    secondsInt = seconds - (minutes * 60);
    milliseconds = (seconds - (int)seconds) * 1000;
    timeString += String(secondsInt) + "." + String(milliseconds);
    return timeString;
}
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

const int top_vertical_y = 24;
const int side_horizontal_x = 22;
const int middle_vertical_x = 186;
const int middle_top_horizontal_y = 61 + top_vertical_y;
const int middle_bottom_horizontal_y = EVE_VSIZE - 61;
const int middle_horizontal_space_y = 36;

void initRefreshDisplay(){
    EVE_start_cmd_burst();
    EVE_cmd_dl_burst(CMD_DLSTART); // Instruct the co-processor to start a new display list
    EVE_cmd_dl_burst(DL_CLEAR_COLOR_RGB | BLACK); // Set the default clear color to black
    EVE_cmd_dl_burst(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG); // Clear the screen - this and the previous prevent artifacts between lists, Attributes are the color, stencil and tag buffers
    EVE_cmd_dl_burst(DL_VERTEX_FORMAT);
    EVE_end_cmd_burst();
}

//Draw main page common elements
void drawMainCommon(){
    EVE_start_cmd_burst();
    
    EVE_cmd_dl_burst( DL_BEGIN | EVE_LINES );

    EVE_cmd_dl_burst( LINE_WIDTH(1U*16U) );
    EVE_cmd_dl_burst( WHITE );

    //Top horizontal line
    EVE_cmd_dl_burst( VERTEX2F(0, top_vertical_y) );
    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE, top_vertical_y) );

    //Side vertical lines
    EVE_cmd_dl_burst( VERTEX2F(side_horizontal_x, top_vertical_y) );
    EVE_cmd_dl_burst( VERTEX2F(side_horizontal_x, EVE_VSIZE) );

    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - side_horizontal_x, top_vertical_y) );
    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - side_horizontal_x, EVE_VSIZE) );

    //Middle vertical lines
    EVE_cmd_dl_burst( VERTEX2F(middle_vertical_x, top_vertical_y) );
    EVE_cmd_dl_burst( VERTEX2F(middle_vertical_x, EVE_VSIZE) );

    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - middle_vertical_x, top_vertical_y) );
    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - middle_vertical_x, EVE_VSIZE) );

    //Middle top horizontal line
    EVE_cmd_dl_burst( VERTEX2F(side_horizontal_x, middle_top_horizontal_y) );
    EVE_cmd_dl_burst( VERTEX2F(middle_vertical_x, middle_top_horizontal_y) );

    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - side_horizontal_x, middle_top_horizontal_y) );
    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - middle_vertical_x, middle_top_horizontal_y) );

    //Middle bottom horizontal line
    EVE_cmd_dl_burst( VERTEX2F(side_horizontal_x, middle_bottom_horizontal_y) );
    EVE_cmd_dl_burst( VERTEX2F(middle_vertical_x, middle_bottom_horizontal_y) );

    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - side_horizontal_x, middle_bottom_horizontal_y) );
    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - middle_vertical_x, middle_bottom_horizontal_y) );

    //Bottom horizontal lines
    EVE_cmd_dl_burst( VERTEX2F(middle_vertical_x, EVE_VSIZE - middle_horizontal_space_y) );
    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - middle_vertical_x, EVE_VSIZE - middle_horizontal_space_y) );

    EVE_cmd_dl_burst( VERTEX2F(middle_vertical_x, EVE_VSIZE - 2 * middle_horizontal_space_y) );
    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - middle_vertical_x, EVE_VSIZE - 2 * middle_horizontal_space_y) );

    EVE_cmd_dl_burst( VERTEX2F(middle_vertical_x, EVE_VSIZE - 3 * middle_horizontal_space_y) );
    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - middle_vertical_x, EVE_VSIZE - 3 * middle_horizontal_space_y) );

    EVE_cmd_dl_burst( VERTEX2F(middle_vertical_x, EVE_VSIZE - 4 * middle_horizontal_space_y) );
    EVE_cmd_dl_burst( VERTEX2F(EVE_HSIZE - middle_vertical_x, EVE_VSIZE - 4 * middle_horizontal_space_y) );

    EVE_cmd_dl_burst( DL_END );

    EVE_end_cmd_burst();
}

//The function above draws lines at this coordinates:
//Top horizontal line: (0, 24) to (480, 24)
//Side vertical lines: (22, 24) to (22, 272) and (458, 24) to (458, 272)
//Middle vertical lines: (186, 24) to (186, 272) and (294, 24) to (294, 272)
//Middle top horizontal line: (22, 85) to (186, 85) and (294, 85) to (458, 85)
//Middle bottom horizontal line: (22, 211) to (186, 211) and (294, 211) to (458, 211)
//Bottom horizontal lines: (186, 236) to (294, 236) and (186, 200) to (294, 200) and (186, 164) to (294, 164) and (186, 128) to (294, 128)

//Draw the main data of the screen
void drawMainData(gameDataContext_t gameContext){
    EVE_start_cmd_burst();
    EVE_cmd_dl_burst(WHITE);

    //Speed
    EVE_cmd_text_burst(0, 9, 20, EVE_OPT_CENTERY, "SPEED");
    EVE_cmd_number_burst(60, 11, 24, EVE_OPT_CENTER, gameContext.speed);

    //Laps
    EVE_cmd_text_burst(EVE_HSIZE -  2, 11, 20, EVE_OPT_CENTERY | EVE_OPT_RIGHTX, "LAP");
    EVE_cmd_number_burst(EVE_HSIZE -  45, 11, 24, EVE_OPT_CENTER, gameContext.lap);

    //Gear
    EVE_cmd_number_burst( EVE_HSIZE / 2, 76, 31, EVE_OPT_CENTER, gameContext.gear);

    //Delta
    EVE_cmd_dl_burst(DELTA_COLOR);
    String deltaString = secondsToTime(gameContext.delta);
    EVE_cmd_text_burst( middle_vertical_x - 4, 54, 31, EVE_OPT_CENTERY | EVE_OPT_RIGHTX, deltaString.c_str());

    //Brake Bias
    EVE_cmd_dl_burst(BRAKE_BIAS_COLOR);
    EVE_cmd_text_burst( EVE_HSIZE - 104, 54, 31, EVE_OPT_CENTER, String(gameContext.brakeBias).substring(0, String(gameContext.brakeBias).indexOf('.') + 2).c_str());

    //Battery
    EVE_cmd_dl_burst(GREEN);
    EVE_cmd_text_burst( EVE_HSIZE / 2, EVE_VSIZE - 90, 30, EVE_OPT_CENTER, String(gameContext.battery).c_str());
    EVE_cmd_dl_burst(WHITE);
    EVE_cmd_text_burst( middle_vertical_x + 4, EVE_VSIZE - 100, 16, EVE_OPT_CENTERY, "BATT");

    //Last Lap Fuel
    EVE_cmd_dl_burst(RED);
    EVE_cmd_text_burst( EVE_HSIZE / 2, EVE_VSIZE - 54, 30, EVE_OPT_CENTER, String(gameContext.lastLapFuel).c_str());
    EVE_cmd_dl_burst(WHITE);
    EVE_cmd_text_burst( middle_vertical_x + 4, EVE_VSIZE - 64, 16, EVE_OPT_CENTERY, "LL");

    //Last Lap Time
    EVE_cmd_dl_burst(LAST_LAP_COLOR);
    EVE_cmd_text_burst( EVE_HSIZE - 104, EVE_VSIZE - 30, 30, EVE_OPT_CENTER, String(gameContext.lastLapTime).c_str());
    EVE_cmd_dl_burst(WHITE);
    EVE_cmd_text_burst( EVE_HSIZE - middle_vertical_x + 2, EVE_VSIZE - 61 + 8, 16, EVE_OPT_CENTERY, "LAST LAP");

    //DRS
    //TODO: Draw DRS

    //ERS Level
    //TODO: Draw ERS Level

    EVE_end_cmd_burst();
}

//Draw the yellow flag
void drawYellowFlag(bool flagBlink){
    EVE_start_cmd_burst();
    EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
    if(flagBlink){
        EVE_cmd_dl_burst(YELLOW_FLAG_COLOR);
    }
    else{
        EVE_cmd_dl_burst(BLACK);
    }
    EVE_cmd_dl_burst(VERTEX2F(0, 0));
    EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE, EVE_VSIZE));
    EVE_cmd_dl_burst(DL_END);
     if(flagBlink){
        EVE_cmd_dl_burst(WHITE);
    }
    else{
        EVE_cmd_dl_burst(YELLOW_FLAG_COLOR);
    }
    EVE_cmd_text_burst(EVE_HSIZE / 2, EVE_VSIZE / 2, 31, EVE_OPT_CENTER, "Yellow Flag");
    EVE_end_cmd_burst();
}

//Draw the red flag
void drawRedFlag(bool flagBlink){
    EVE_start_cmd_burst();
    EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
    if(flagBlink){
        EVE_cmd_dl_burst(RED_FLAG_COLOR);
    }
    else{
        EVE_cmd_dl_burst(BLACK);
    }
    EVE_cmd_dl_burst(VERTEX2F(0, 0));
    EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE, EVE_VSIZE));
    EVE_cmd_dl_burst(DL_END);
    if(flagBlink){
        EVE_cmd_dl_burst(WHITE);
    }
    else{
        EVE_cmd_dl_burst(RED_FLAG_COLOR);
    }
    EVE_cmd_text_burst(EVE_HSIZE / 2, EVE_VSIZE / 2, 31, EVE_OPT_CENTER, "Red Flag");
    EVE_end_cmd_burst();
}

//Draw the ERS mode 0
void drawERSMode(gameDataContext_t gameContext){
    EVE_start_cmd_burst();

    if(gameContext.ERSMode == 0){
        //Brake temperatures
        EVE_cmd_dl_burst(WHITE);
        EVE_cmd_text_burst( 22 + 4, 85 + 4 + 10, 20, EVE_OPT_CENTERY, String(gameContext.frontLeftBrakeTemp).c_str());
        EVE_cmd_text_burst( 458 - 4, 85 + 4 + 10, 20, EVE_OPT_CENTERY, String(gameContext.frontRightBrakeTemp).c_str());
        EVE_cmd_text_burst( 22 + 4, EVE_VSIZE - 61 - 4 - 10, 20, EVE_OPT_CENTERY, String(gameContext.rearLeftBrakeTemp).c_str());
        EVE_cmd_text_burst( 458 - 4, EVE_VSIZE - 61 - 4 - 10, 20, EVE_OPT_CENTERY, String(gameContext.rearRightBrakeTemp).c_str());

        //Fuel target
        EVE_cmd_dl_burst(WHITE);
        EVE_cmd_text_burst( EVE_HSIZE / 2, EVE_VSIZE - 36/2, 20, EVE_OPT_CENTER, String(gameContext.fuelTarget).c_str());
        EVE_cmd_dl_burst(WHITE);
        EVE_cmd_text_burst( middle_vertical_x + 4, EVE_VSIZE - 36 + 4 - 8, 16, EVE_OPT_CENTER, "F");
    }

    //Tyre temperatures
    int tyre_temp_x_offset = 20;
    if(gameContext.ERSMode == 0) EVE_cmd_dl_burst(COLOR_RGB(0, 250, 154));
    else EVE_cmd_dl_burst(WHITE);
    EVE_cmd_text_burst( middle_vertical_x - 4 - tyre_temp_x_offset, 85 + 4 + 10, 20, EVE_OPT_CENTERY, String(gameContext.frontLeftTyreTemp).c_str());
    EVE_cmd_text_burst( EVE_HSIZE - middle_vertical_x + 4, 85 + 4 + 10, 20, EVE_OPT_CENTERY, String(gameContext.frontRightTyreTemp).c_str());
    EVE_cmd_text_burst( middle_vertical_x - 4 - tyre_temp_x_offset, EVE_VSIZE - 61 - 4 - 10, 20, EVE_OPT_CENTERY, String(gameContext.rearLeftTyreTemp).c_str());
    EVE_cmd_text_burst( EVE_HSIZE - middle_vertical_x + 4, EVE_VSIZE - 61 - 4 - 10, 20, EVE_OPT_CENTERY, String(gameContext.rearRightTyreTemp).c_str());

    //ERS mode
    //Draw the background color of the ERS mode
    if(gameContext.ERSMode != 0){
        switch(gameContext.ERSMode){
            case 1:
                EVE_cmd_dl_burst(WHITE);
                break;
            case 2:
                EVE_cmd_dl_burst(GREEN);
                break;
            case 3:
                EVE_cmd_dl_burst(BLUE);
                break;
            case 4:
            case 5:
            case 6:
                EVE_cmd_dl_burst(COLOR_RGB(238, 130, 238));
                break;
        }
        EVE_cmd_dl(DL_BEGIN | EVE_RECTS);
        EVE_cmd_dl_burst(VERTEX2F(middle_vertical_x + 1, EVE_VSIZE - 36*4 + 1));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE - middle_vertical_x - 1, EVE_VSIZE - 36*3 - 1));
        EVE_cmd_dl_burst(DL_END);
    }

    //Draw the text of the ERS mode
    if(gameContext.ERSMode == 1) EVE_cmd_dl_burst(BLACK);
    else EVE_cmd_dl_burst(WHITE);
    EVE_cmd_text_burst( EVE_HSIZE / 2, EVE_VSIZE - (3 * middle_horizontal_space_y + middle_horizontal_space_y / 2), 20, EVE_OPT_CENTER, ERSModeText(gameContext.ERSMode).c_str());

    EVE_end_cmd_burst();
}

//Draw the FPS and the delta time between frames
void drawFPS(uint32_t deltaTime){
    String refreshDelta = String(deltaTime) + " ms " + String(1000.0/(deltaTime)) + " fps";
    EVE_start_cmd_burst();
    EVE_cmd_dl_burst(WHITE);
    EVE_cmd_text_burst(EVE_HSIZE / 2, 15, 22, EVE_OPT_CENTER, refreshDelta.c_str());
    EVE_end_cmd_burst();
}

//This function convert a float delta time ( negative or positive ) to a string.
String secondsToTime(float seconds){
    String timeString = "";
    int minutes = 0, secondsInt = 0, milliseconds = 0;
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

//This function returns the ERS mode as a string
String ERSModeText(int ERSMode){
    switch(ERSMode){
        case 0:
            return "0 CHAR";
        case 1:
            return "1 MINUS";
        case 2:
            return "2 PLUS";
        case 3:
            return "3 MAX";
        case 4:
            return "4 OVRTK";
        case 5:
            return "5 QUALI";
        case 6:
            return "6 HOT";
        default:
            return "ERR";
    }
}
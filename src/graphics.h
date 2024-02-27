//This file handles the drawings on the screen.
//The screen is 480x272 pixels.


#ifndef graphics
#define graphics

#define RED     0xff0000UL
#define ORANGE  0xffa500UL
#define GREEN   0x00ff00UL
#define BLUE    0x0000ffUL
#define BLUE_1  0x5dade2L
#define YELLOW  0xffff00UL
#define MAGENTA 0xff00ffUL
#define PURPLE  0x800080UL
#define WHITE   0xffffffUL
#define BLACK   0x000000UL
#define PINK    0xff5090

#define REFRESH_RATE 60.0
#define REFRESH_RATE_MS 1000.0/REFRESH_RATE

#include <Arduino.h>
#include "EVE.h"
#include "gameDataInput.h"

//void drawInitLogo();

//Clear the screen and start a new display list
void initRefreshDisplay();

//Draw main page common elements
void drawMainCommon();

//Draw main page data
void drawMainData(gameDataContext_t gameContext);

//Draw the yellow flag
void drawYellowFlag(bool flagBlink);

//Draw the red flag
void drawRedFlag(bool flagBlink);

//Draw FPS and delta between frames
void drawFPS(uint32_t deltaTime);

//Convert seconds to time
String secondsToTime(float seconds);

#endif
#include <Arduino.h>
#include <SPI.h>
#include "EVE.h"
#include "rev_lights.h"
#include "graphics.h"
#include "gameDataInput.h"

int compteur = 0;
uint32_t ms = 0;
uint32_t msRefreshDisplay = 0;

CRGB leds[NUM_LEDS];

bool flagBlink = false;

gameDataContext_t gameContext;

uint8_t ptr[4096] = {128};

bool newData = false;
char receivedChars[BUFFER_SIZE];
char tempChars[BUFFER_SIZE];

void setup() {

  gameContext = {0}; //Game context initialization

  Serial.begin(115200); //Serial initialization
  
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS); //LED initialization

  //Display wiring initialization
  pinMode(EVE_CS, OUTPUT);
  digitalWrite(EVE_CS, HIGH);
  pinMode(EVE_PDN, OUTPUT);
  digitalWrite(EVE_PDN, LOW);

  SPI.begin(); //Sets up the SPI to run in Mode 0 and 1 MHz
  SPI.beginTransaction(SPISettings(8UL * 1000000UL, MSBFIRST, SPI_MODE0));

  //Make sure the display is correctly initialized
  if(E_OK == EVE_init()){ 
    Serial.println("EVE init successfull");
    delay(20);
  }
  else{
    Serial.println("EVE init failed");
    while(1);
  }
}

void loop() {

  //Serial communication with the game
  recvWithStartEndMarkers(receivedChars, &newData);
  if (newData == true) {
    strcpy(tempChars, receivedChars);
    parseReceivedData(tempChars, &gameContext);
    newData = false;
    rev_lights_rpm(leds, gameContext.gear, gameContext.rpm);
  }

  //Display refresh
  if(millis() > msRefreshDisplay + REFRESH_RATE_MS){
    
    /*
    //Print FPS and delta between frames
    uint32_t deltaTime = millis() - msRefreshDisplay;
    drawFPS(deltaTime);
    */

    msRefreshDisplay = millis(); //Refresh time update
    
    initRefreshDisplay(); //Display cleaning and new display list

    drawMainCommon(); //Draw common elements
    drawMainData(gameContext); //Draw common data
    drawERSMode(gameContext); //Draw ERS mode
    
    switch(gameContext.flag){

      case 0: //No flag
          flag_lights(leds, CRGB(0, 0, 0));
          FastLED.show();
        break;
      case 1: //Yellow flag
        if(millis() > ms + 1000){
          ms = millis();
          if(flagBlink){
            flagBlink = false;
            flag_lights(leds, yellow_flag_light);
          }
          else{
            flagBlink = true;
            flag_lights(leds, CRGB(0, 0, 0));
          }
          FastLED.show();
        }
        drawYellowFlag(flagBlink);
        break;
      case 2: //Red flag
        if(millis() > ms + 1000){
          ms = millis();
          if(flagBlink){
            flagBlink = false;
            flag_lights(leds, red_flag_light);
          }
          else{
            flagBlink = true;
            flag_lights(leds, CRGB(0, 0, 0));
          }
          FastLED.show();
        }
        drawRedFlag(flagBlink);
        break;
    }
    
    EVE_start_cmd_burst();
    EVE_cmd_dl_burst(DL_DISPLAY); //Mark the end of the display-list
    EVE_cmd_dl_burst(CMD_SWAP); //Make this list active
    EVE_end_cmd_burst();
  }
}

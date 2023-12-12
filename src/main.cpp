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

  gameContext = {0, 0, 0, 0, 0.0};

  //Serial initialization
  Serial.begin(115200);

  //LED initialization
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);

  //Display initialization
  pinMode(EVE_CS, OUTPUT);
  digitalWrite(EVE_CS, HIGH);
  pinMode(EVE_PDN, OUTPUT);
  digitalWrite(EVE_PDN, LOW);

  SPI.begin(); /* sets up the SPI to run in Mode 0 and 1 MHz */
  SPI.beginTransaction(SPISettings(8UL * 1000000UL, MSBFIRST, SPI_MODE0));

  if(E_OK == EVE_init()){ /* make sure the init finished correctly */
    Serial.println("EVE init successfull");
  }
  else{
    Serial.println("EVE init failed");
    while(1);
  }
  delay(20);
}

void loop() {

  recvWithStartEndMarkers(receivedChars, &newData);
  if (newData == true) {
    strcpy(tempChars, receivedChars);
    parseReceivedData(tempChars, &gameContext);
    newData = false;
    rev_lights_rpm(leds, gameContext.gear, gameContext.rpm);
  } 

  if(millis() > msRefreshDisplay + REFRESH_RATE_MS){
    int deltaTime = millis() - msRefreshDisplay;
    msRefreshDisplay = millis();
    EVE_start_cmd_burst();
    EVE_cmd_dl_burst(CMD_DLSTART); /* instruct the co-processor to start a new display list */
    EVE_cmd_dl_burst(DL_CLEAR_COLOR_RGB | BLACK); /* set the default clear color to white */
    EVE_cmd_dl_burst(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG); /* clear the screen - this and the previous prevent artifacts between lists, Attributes are the color, stencil and tag buffers */
    EVE_cmd_dl_burst(DL_VERTEX_FORMAT);
    EVE_end_cmd_burst();

    drawMainCommon();
    drawMainData(gameContext.speed, gameContext.lap, gameContext.gear, gameContext.delta, gameContext.brakeBias, gameContext.battery);

    //Print FPS and delta between frames
    String refreshDelta = String(deltaTime) + " ms " + String(1000.0/(deltaTime)) + " fps";
    EVE_start_cmd_burst();
    EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));
    EVE_cmd_text_burst(EVE_HSIZE / 2, 15, 22, EVE_OPT_CENTER, refreshDelta.c_str());
    EVE_end_cmd_burst();


    switch(gameContext.flag){
      case 0:
          flag_lights(leds, CRGB(0, 0, 0));
          FastLED.show();
        break;
      case 1:
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
      case 2:
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
    EVE_cmd_dl_burst(DL_DISPLAY); /* mark the end of the display-list */
    EVE_cmd_dl_burst(CMD_SWAP); /* make this list active */
    EVE_end_cmd_burst();
  }
}

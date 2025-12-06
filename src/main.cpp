#include "EVE.h"
#include "gameDataInput.h"
#include "graphics.h"
#include "rev_lights.h"
#include <Arduino.h>
#include <SPI.h>

int compteur = 0;
uint32_t ms = 0;
uint32_t msRefreshDisplay = 0;

CRGB leds[NUM_LEDS];

bool flagBlink = false;

// Popup variables
bool popupActive = false;
uint32_t popupStartTime = 0;
char popupTitle[32];
char popupValue[32];
int prevBrakeBias = -1; // Initialize with invalid value to avoid startup popup
                        // if possible, or handle in setup
float prevFuelTarget = -1.0;

gameDataContext_t gameContext;

uint8_t ptr[4096] = {128};

bool newData = false;
char receivedChars[BUFFER_SIZE];
char tempChars[BUFFER_SIZE];

void setup() {

  gameContext = {0}; // Game context initialization
  prevBrakeBias = gameContext.brakeBias;
  prevFuelTarget = gameContext.fuelTarget;

  Serial.begin(115200); // Serial initialization

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS); // LED initialization

  // Display wiring initialization
  pinMode(EVE_CS, OUTPUT);
  digitalWrite(EVE_CS, HIGH);
  pinMode(EVE_PDN, OUTPUT);
  digitalWrite(EVE_PDN, LOW);

  SPI.begin(); // Sets up the SPI to run in Mode 0 and 1 MHz
  SPI.beginTransaction(SPISettings(8UL * 1000000UL, MSBFIRST, SPI_MODE0));

  // Make sure the display is correctly initialized
  if (E_OK == EVE_init()) {
    Serial.println("EVE init successfull");
    delay(20);
  } else {
    Serial.println("EVE init failed");
    while (1)
      ;
  }

  // Startup Sequence
  drawStartup();

  // LED Chase
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;
    FastLED.show();
    delay(50);
  }
  delay(500);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
  delay(500);
}

void loop() {

  // Serial communication with the game
  recvWithStartEndMarkers(receivedChars, &newData);
  if (newData == true) {
    strcpy(tempChars, receivedChars);
    parseReceivedData(tempChars, &gameContext);
    newData = false;
    rev_lights_rpm(leds, gameContext.gear, gameContext.rpm);

    // Check for changes for popup
    if (gameContext.brakeBias != prevBrakeBias) {
      popupActive = true;
      popupStartTime = millis();
      strcpy(popupTitle, "BRAKE BIAS");
      snprintf(popupValue, sizeof(popupValue), "%d", gameContext.brakeBias);
      prevBrakeBias = gameContext.brakeBias;
    }

    // Check for fuel target changes
    // Note: comparing floats directly is risky, but for a setting that changes
    // in steps it might be ok. Better to check if diff is significant.
    if (fabsf(gameContext.fuelTarget - prevFuelTarget) > 0.01) {
      popupActive = true;
      popupStartTime = millis();
      strcpy(popupTitle, "FUEL TARGET");
      snprintf(popupValue, sizeof(popupValue), "%.2f", gameContext.fuelTarget);
      prevFuelTarget = gameContext.fuelTarget;
    }
  }

  // Display refresh
  if (millis() > msRefreshDisplay + REFRESH_RATE_MS) {

    /*
    //Print FPS and delta between frames
    uint32_t deltaTime = millis() - msRefreshDisplay;
    drawFPS(deltaTime);
    */

    msRefreshDisplay = millis(); // Refresh time update

    initRefreshDisplay(); // Display cleaning and new display list

    drawMainCommon();          // Draw common elements
    drawMainData(gameContext); // Draw common data
    drawERSMode(gameContext);  // Draw ERS mode

    switch (gameContext.flag) {

    case 0: // No flag
      flag_lights(leds, CRGB(0, 0, 0));
      FastLED.show();
      break;
    case 1: // Yellow flag
      if (millis() > ms + 1000) {
        ms = millis();
        if (flagBlink) {
          flagBlink = false;
          flag_lights(leds, yellow_flag_light);
        } else {
          flagBlink = true;
          flag_lights(leds, CRGB(0, 0, 0));
        }
        FastLED.show();
      }
      drawYellowFlag(flagBlink);
      break;
    case 2: // Red flag
      if (millis() > ms + 1000) {
        ms = millis();
        if (flagBlink) {
          flagBlink = false;
          flag_lights(leds, red_flag_light);
        } else {
          flagBlink = true;
          flag_lights(leds, CRGB(0, 0, 0));
        }
        FastLED.show();
      }
      drawRedFlag(flagBlink);
      break;
    }

    // Draw Popup if active
    if (popupActive) {
      if (millis() - popupStartTime < 2000) { // Show for 2 seconds
        drawPopup(popupTitle, popupValue);
      } else {
        popupActive = false;
      }
    }

    EVE_start_cmd_burst();
    EVE_cmd_dl_burst(DL_DISPLAY); // Mark the end of the display-list
    EVE_cmd_dl_burst(CMD_SWAP);   // Make this list active
    EVE_end_cmd_burst();
  }
}

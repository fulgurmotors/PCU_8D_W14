#include "flag_logic.h"

FlagManager::FlagManager() : flagBlink(false), lastBlinkTime(0) {}

void FlagManager::update(const gameDataContext_t &context, CRGB *leds) {
  switch (context.flag) {
  case 0: // No flag
    flag_lights(leds, CRGB(0, 0, 0));
    FastLED.show(); // Assuming we need to show explicitly or if it's handled
                    // elsewhere. In main.cpp it was called here.
    break; // No drawing needed for no flag, or cleared by default refresh?
           // main.cpp just broke. But loop does initRefreshDisplay() then
           // draw... Wait, drawYellowFlag/RedFlag are called *inside* the
           // switch case in main.cpp. So I should probably move the drawing
           // logic here or expose a draw() method. Re-reading main.cpp: it
           // calls initRefreshDisplay, drawMainCommon, etc BEFORE the switch.
           // THEN switch(flag) -> updates leds AND calls
           // drawYellowFlag/RedFlag. So update() here should do both? Or split?
           // The request said "move flag handling".
           // The LED update needs to happen (FastLED.show() is called).
           // The screen drawing needs to happen inside the display list
           // construction. So update() probably should be called inside the
           // display refresh loop? But LED updates run independently of display
           // refresh in theory? In main.cpp, everything is inside `if (millis()
           // > msRefreshDisplay + REFRESH_RATE_MS)`. So LEDs and Display are
           // updated at the same rate. I will put both in update() for now, as
           // it matches main.cpp structure.
    break;

  case 1: // Yellow flag
    if (millis() > lastBlinkTime + 1000) {
      lastBlinkTime = millis();
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
    if (millis() > lastBlinkTime + 1000) {
      lastBlinkTime = millis();
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
}

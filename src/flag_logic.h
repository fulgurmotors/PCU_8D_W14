#ifndef FLAG_LOGIC_H
#define FLAG_LOGIC_H

#include "gameDataInput.h"
#include "graphics.h"
#include "rev_lights.h"
#include <Arduino.h>
#include <FastLED.h>

class FlagManager {
public:
  FlagManager();
  void update(const gameDataContext_t &context, CRGB *leds);

private:
  bool flagBlink;
  uint32_t lastBlinkTime;
};

#endif

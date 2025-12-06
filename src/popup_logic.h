#ifndef POPUP_LOGIC_H
#define POPUP_LOGIC_H

#include "gameDataInput.h"
#include "graphics.h"
#include <Arduino.h>

class PopupManager {
public:
  PopupManager();
  void update(const gameDataContext_t &context);
  void draw();

private:
  bool popupActive;
  uint32_t popupStartTime;
  char popupTitle[32];
  char popupValue[32];
  int prevBrakeBias;
  float prevFuelTarget;
};

#endif

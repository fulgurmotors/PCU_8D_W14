#include "popup_logic.h"

PopupManager::PopupManager()
    : popupActive(false), popupStartTime(0), prevBrakeBias(-1),
      prevFuelTarget(-1.0) {
  memset(popupTitle, 0, sizeof(popupTitle));
  memset(popupValue, 0, sizeof(popupValue));
}

void PopupManager::update(const gameDataContext_t &context) {
  bool newPopup = false;

  // Initialize previous values on first valid data if they are sentinel values
  // But logic in main.cpp was just simple check.
  // If context is zero-inited, bias is 0.

  // Check for brake bias changes
  if (context.brakeBias != prevBrakeBias) {
    // First run or actual change?
    // If prevBrakeBias is -1 (init), we might update without popup,
    // OR we might want to avoid popup on startup.
    // main.cpp initialized prevBrakeBias with gameContext.brakeBias (which was
    // 0) in setup. We can handle that by setting it to a special value or just
    // updating it silently if it's the very first implementation. However,
    // sticking to main.cpp logic closely:

    // If it's the first time update is called, prev is -1.
    // If context.brakeBias is 0 (default), then -1 != 0 -> popup "0".
    // To avoid startup popup, maybe we sync first?
    // But main.cpp logic was:
    // prevBrakeBias = gameContext.brakeBias; // in setup, so it matches.

    // Let's assume the user calls update() in loop, so we need a way to 'init'
    // or we accept one popup. For now, I will implement logic as is, but maybe
    // check if prev is -1.

    if (prevBrakeBias != -1) {
      popupActive = true;
      popupStartTime = millis();
      strcpy(popupTitle, "BRAKE BIAS");
      snprintf(popupValue, sizeof(popupValue), "%d", context.brakeBias);
    }
    prevBrakeBias = context.brakeBias;
  }

  // Check for fuel target changes
  if (fabsf(context.fuelTarget - prevFuelTarget) > 0.01) {
    if (prevFuelTarget != -1.0) {
      popupActive = true;
      popupStartTime = millis();
      strcpy(popupTitle, "FUEL TARGET");
      snprintf(popupValue, sizeof(popupValue), "%.2f", context.fuelTarget);
    }
    prevFuelTarget = context.fuelTarget;
  }
}

void PopupManager::draw() {
  if (popupActive) {
    if (millis() - popupStartTime < 2000) { // Show for 2 seconds
      drawPopup(popupTitle, popupValue);
    } else {
      popupActive = false;
    }
  }
}

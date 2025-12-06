#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include "../core/DataStore.h"
#include <Arduino.h>

class UIRenderer {
public:
  UIRenderer();
  void begin();
  void draw();

private:
  uint32_t lastDrawTime;

  // Popup state
  bool popupActive;
  uint32_t popupStartTime;
  char popupTitle[32];
  char popupValue[32];
  float prevBrakeBias;
  float prevFuelTarget;

  void checkPopupTriggers(const TelemetryPacket &data);
  void drawPopup();

  void initDisplayList();
  void drawCommonElements();
  void drawData(const TelemetryPacket &data);
  void drawERS(const TelemetryPacket &data);
  void drawFlag(int flag);
  void drawStartup(); // Make public if needed for setup

  // Helpers
  void secondsToTime(float seconds, char *buffer);
  const char *ersModeText(int mode);

public:
  void showStartupSequence();
};

#endif

#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include "../core/ConfigManager.h"
#include "../core/DataStore.h"
#include <Arduino.h>

class UIRenderer {
public:
  UIRenderer();
  void begin();
  void draw();

private:
  uint32_t lastDrawTime;
  ConfigManager configManager;

  // State
  int currentPageId;
  const Popup *activePopup;
  uint32_t popupStartTime;

  // Previous values for triggers
  float prevBrakeBias;
  float prevFuelTarget;

  void checkPopupTriggers(const TelemetryPacket &data);
  void drawPopup();

  void initDisplayList();
  void drawPage(const Page *page, const TelemetryPacket &data);
  void drawElement(const UIElement &elem, const TelemetryPacket &data);
  void drawFlag(int flag);
  void drawStartup();

  // Helpers
  void secondsToTime(float seconds, char *buffer);
  const char *ersModeText(int mode);

public:
  void showStartupSequence();
};

#endif

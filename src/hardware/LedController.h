#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include "../core/ConfigManager.h"
#include "../core/DataStore.h"
#include <FastLED.h>

#define NUM_LEDS 21
#define LED_DATA_PIN 4

class LedController {
public:
  LedController();
  void begin();
  void update();
  void turnOff();

private:
  CRGB leds[NUM_LEDS];
  uint32_t lastBlinkTime;
  bool blinkState;
  ConfigManager configManager;

  void updateRevLights(int rpm, int gear);
  void updateFlagLights(int flag);
  void turnOnRevLights(int nbr);
  void setFlagLights(CRGB color);
};

#endif

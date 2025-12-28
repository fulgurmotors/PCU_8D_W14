#include "LedController.h"
#include <array>

// Using the same LED colors logic but now dynamic
#define led_brightness 32
#define led_saturation 255
// Only used for fallbacks
#define red_rev_light CHSV(0, led_saturation, led_brightness)
#define yellow_flag_light CHSV(20, led_saturation, led_brightness)
#define red_flag_light CHSV(0, led_saturation, led_brightness)

LedController::LedController() : lastBlinkTime(0), blinkState(false) {}

void LedController::begin() {
  FastLED.addLeds<WS2812, LED_DATA_PIN, GRB>(leds, NUM_LEDS);
  turnOff();

  // Load config if not already loaded by UIRenderer
  if (!configManager.loadConfig("/config.json")) {
    // Defaults already set
  }
}

void LedController::turnOff() {
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CRGB::Black;
  FastLED.show();
}

void LedController::update() {
  const TelemetryPacket &data = DataStore::getInstance().getTelemetry();

  updateRevLights(data.rpm, data.gear);
  updateFlagLights(data.flag);

  FastLED.show();
}

void LedController::updateRevLights(int rpm, int gear) {
  if (gear == -1) {
    turnOnRevLights(0);
    return;
  }

  // Rev curve - could also be in config but keeping hardcoded for now or
  // expanding later
  static const int rpm_ranges[8][2] = {{0, 12000}, {0, 12000}, {0, 12000},
                                       {0, 12000}, {0, 12000}, {0, 12000},
                                       {0, 12000}, {0, 12000}};

  int g = (gear >= 0 && gear <= 7) ? gear : 0;
  int r = map(rpm, rpm_ranges[g][0], rpm_ranges[g][1], 0, 15);
  turnOnRevLights(r);
}

void LedController::turnOnRevLights(int nbr) {
  const LedConfig &cfg = configManager.getLedConfig();

  if (nbr < 0)
    nbr = 0;
  if (nbr > cfg.revLightsCount)
    nbr = cfg.revLightsCount;

  for (int i = 0; i < cfg.revLightsCount; i++) {
    int ledIndex = cfg.revLightsStart + i;

    if (ledIndex >= NUM_LEDS)
      continue;

    if (i < nbr) {
      if (i < (int)cfg.revColors.size()) {
        uint32_t c = cfg.revColors[i];
        leds[ledIndex] = CRGB((c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF);
      } else {
        leds[ledIndex] = CRGB::Red;
      }
    } else {
      leds[ledIndex] = CRGB::Black;
    }
  }
}

void LedController::updateFlagLights(int flag) {
  if (flag == 0) {
    setFlagLights(CRGB::Black);
    return;
  }

  // Blink logic
  if (millis() - lastBlinkTime > 500) {
    lastBlinkTime = millis();
    blinkState = !blinkState;
  }

  if (blinkState) {
    if (flag == 1)
      setFlagLights(yellow_flag_light);
    else if (flag == 2)
      setFlagLights(red_flag_light);
  } else {
    setFlagLights(CRGB::Black);
  }
}

void LedController::setFlagLights(CRGB color) {
  const LedConfig &cfg = configManager.getLedConfig();

  for (int i = 0; i < cfg.flagCount; i++) {
    int leftIdx = cfg.flagLeftStart + i;
    int rightIdx = cfg.flagRightStart + i;

    if (leftIdx < NUM_LEDS)
      leds[leftIdx] = color;
    if (rightIdx < NUM_LEDS)
      leds[rightIdx] = color;
  }
}

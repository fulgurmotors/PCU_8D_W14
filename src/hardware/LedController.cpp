#include "LedController.h"
#include <array>

#define REV_LIGHT_START 3
#define FLAG_LIGHT_COUNT_BY_SIDE 3
#define FLAG_LIGHT_LEFT_START 0
#define FLAG_LIGHT_RIGHT_START 18

// Using the same colors as before
#define led_brightness 32
#define led_saturation 255
#define red_rev_light CHSV(0, led_saturation, led_brightness)
#define green_rev_light CHSV(96, led_saturation, led_brightness)
#define blue_rev_light CHSV(160, led_saturation, led_brightness)
#define yellow_flag_light CHSV(20, led_saturation, led_brightness)
#define red_flag_light CHSV(0, led_saturation, led_brightness)

CRGB rev_lights_color[15] = {red_rev_light,   red_rev_light,   red_rev_light,
                             red_rev_light,   red_rev_light,   green_rev_light,
                             green_rev_light, green_rev_light, green_rev_light,
                             green_rev_light, blue_rev_light,  blue_rev_light,
                             blue_rev_light,  blue_rev_light,  blue_rev_light};

LedController::LedController() : lastBlinkTime(0), blinkState(false) {}

void LedController::begin() {
  FastLED.addLeds<WS2812, LED_DATA_PIN, GRB>(leds, NUM_LEDS);
  turnOff();
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
    // Reverse gear logic (if any specific) or just off
    turnOnRevLights(0);
    return;
  }

  // Rev curve
  static const int rpm_ranges[8][2] = {{0, 12000}, {0, 12000}, {0, 12000},
                                       {0, 12000}, {0, 12000}, {0, 12000},
                                       {0, 12000}, {0, 12000}};

  int g = (gear >= 0 && gear <= 7) ? gear
                                   : 0; // Default to 0 range if out of bounds
  int r = map(rpm, rpm_ranges[g][0], rpm_ranges[g][1], 0, 15);
  turnOnRevLights(r);
}

void LedController::turnOnRevLights(int nbr) {
  if (nbr < 0)
    nbr = 0;
  if (nbr > 15)
    nbr = 15;

  for (int i = 0; i < 15; i++) {
    if (i < nbr) {
      leds[i + REV_LIGHT_START] = rev_lights_color[i];
    } else {
      leds[i + REV_LIGHT_START] = CRGB::Black;
    }
  }
}

void LedController::updateFlagLights(int flag) {
  if (flag == 0) {
    setFlagLights(CRGB::Black);
    return;
  }

  // Blink logic
  if (millis() - lastBlinkTime > 500) { // 500ms blink
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
  for (int i = FLAG_LIGHT_LEFT_START;
       i < FLAG_LIGHT_LEFT_START + FLAG_LIGHT_COUNT_BY_SIDE; i++) {
    leds[i] = color;
  }
  for (int i = FLAG_LIGHT_RIGHT_START;
       i < FLAG_LIGHT_RIGHT_START + FLAG_LIGHT_COUNT_BY_SIDE; i++) {
    leds[i] = color;
  }
}

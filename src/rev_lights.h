//This file handles the rev lights and the flag lights.
//The rev lights are the 15 LEDs on top of the screen.
//The flag lights are the 3 LEDs on each side of the screen.

#ifndef rev_lights
#define rev_lights

#include <FastLED.h>

#define NUM_LEDS 21
#define DATA_PIN 4

#define REV_LIGHT_START 3
#define REV_LIGHT_COUNT 15
#define FLAG_LIGHT_COUNT_BY_SIDE 3
#define FLAG_LIGHT_LEFT_START 0
#define FLAG_LIGHT_RIGHT_START 18

#define led_brightness 32
#define led_saturation 255

#define red_rev_light CHSV(0, led_saturation, led_brightness)
#define green_rev_light CHSV(96, led_saturation, led_brightness)
#define blue_rev_light CHSV(160, led_saturation, led_brightness)
#define yellow_flag_light CHSV(20, led_saturation, led_brightness)
#define red_flag_light CHSV(0, led_saturation, led_brightness)

void turn_on_rev_lights(CRGB* leds, int nbr);
void flag_lights(CRGB* leds, CRGB color);
int rev_lights_rpm(CRGB* leds, int gear, int rpm);

#endif
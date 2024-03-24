#include "rev_lights.h"
#include <array>

CRGB rev_lights_color[NUM_LEDS] = {red_rev_light,
                                   red_rev_light,
                                   red_rev_light,
                                   red_rev_light,
                                   red_rev_light,
                                   green_rev_light,
                                   green_rev_light,
                                   green_rev_light,
                                   green_rev_light,
                                   green_rev_light,
                                   blue_rev_light,
                                   blue_rev_light,
                                   blue_rev_light,
                                   blue_rev_light,
                                   blue_rev_light};

void turn_on_rev_lights(CRGB* leds, int nbr){
    if(nbr < 0) nbr = 0;
    else if(nbr > 15) nbr = 15;
    
    for(int i = 0;i < 15;i++){
        if(i < nbr){
        leds[i + REV_LIGHT_START] = rev_lights_color[i];
        }
        else{
        leds[i + REV_LIGHT_START] = CRGB(0, 0, 0);
        }
    }
    FastLED.show();
}

int rev_lights_rpm(CRGB* leds, int gear, int rpm){
  std::array<int, 2> rev_lights_rpm[8] = {{0, 12000}, {0, 12000}, {0, 12000}, {0, 12000}, {0, 12000}, {0, 12000}, {0, 12000}, {0, 12000}};

  if(gear >= 0 && gear <= 7){
    int r = map(rpm, rev_lights_rpm[gear][0], rev_lights_rpm[gear][1], 0, 15);
    turn_on_rev_lights(leds, r);
    return 0;
  }
  else if (gear == -1){
    return 0;
  }

  return -1;
}

void flag_lights(CRGB* leds, CRGB color){
    for(int i = FLAG_LIGHT_LEFT_START;i < FLAG_LIGHT_LEFT_START + FLAG_LIGHT_COUNT_BY_SIDE;i++){
      leds[i] = color;
    }
    for(int i = FLAG_LIGHT_RIGHT_START;i < FLAG_LIGHT_RIGHT_START + FLAG_LIGHT_COUNT_BY_SIDE;i++){
      leds[i] = color;
    }
    FastLED.show();
}
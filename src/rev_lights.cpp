#include "rev_lights.h"

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

void rev_lights_rpm(CRGB* leds, int gear, int rpm){
  int max = 0;
  int min = 0;

  switch(gear){
    case 0:
      min = 0;
      max = 12000;
      break;
    case 1:
      min = 0;
      max = 12000;
      break;
    case 2:
      min = 0;
      max = 12000;
      break;
    case 3:
      min = 0;
      max = 12000;
      break;
    case 4:
      min = 0;
      max = 12000;
      break;
    case 5:
      min = 0;
      max = 12000;
      break;
    case 6:
      min = 0;
      max = 12000;
      break;
    case 7:
      min = 0;
      max = 12000;
      break;
  }

  int r = map(rpm, min, max, 0, 15);

  turn_on_rev_lights(leds, r);
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
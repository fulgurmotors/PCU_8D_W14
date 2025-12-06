#include "EVE.h"
#include "core/SerialProtocol.h"
#include "hardware/LedController.h"
#include "ui/UIRenderer.h"
#include <Arduino.h>

// Components
SerialProtocol serialProtocol;
UIRenderer uiRenderer;
LedController ledController;

uint32_t lastDisplayUpdate = 0;
#define REFRESH_RATE_MS 16 // ~60Hz

void setup() {
  Serial.begin(115200);

  uiRenderer.begin();
  ledController.begin();

  uiRenderer.showStartupSequence();
}

void loop() {
  // 1. Process Incoming Data
  serialProtocol.update();

  // 2. Update Peripherals
  // LEDs update as fast as possible or on data change?
  // LedController checks DataStore status or we can poll it.
  // Putting it in the display loop or separate?
  // FastLED disabling interrupts can mess with Serial sometimes, but on
  // Teensy 4.1 it's usually fine. Let's update LEDs every loop or throttle it.
  ledController.update();

  // 3. Update Display
  if (millis() - lastDisplayUpdate > REFRESH_RATE_MS) {
    lastDisplayUpdate = millis();
    uiRenderer.draw();
  }
}

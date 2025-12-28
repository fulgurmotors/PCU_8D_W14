#include "UIRenderer.h"
#include "EVE.h"
#include <SPI.h>

#define EVE_CS 10
#define EVE_PDN 8

// EVE Screen size
#define EVE_HSIZE 480
#define EVE_VSIZE 272

UIRenderer::UIRenderer()
    : lastDrawTime(0), currentPageId(0), activePopup(nullptr),
      popupStartTime(0), prevBrakeBias(-1), prevFuelTarget(-1) {}

void UIRenderer::begin() {
  pinMode(EVE_CS, OUTPUT);
  digitalWrite(EVE_CS, HIGH);
  pinMode(EVE_PDN, OUTPUT);
  digitalWrite(EVE_PDN, LOW);

  SPI.begin();
  SPI.beginTransaction(SPISettings(8UL * 1000000UL, MSBFIRST, SPI_MODE0));

  if (E_OK == EVE_init()) {
    Serial.println("EVE init success");
  } else {
    Serial.println("EVE init failed");
  }

  // Load config
  if (!configManager.loadConfig("/config.json")) {
    Serial.println("Failed to load /config.json, using defaults or empty");
  } else {
    Serial.print("Loaded Config: ");
    Serial.println(configManager.getDashboardName());
  }
}

void UIRenderer::showStartupSequence() {
  EVE_start_cmd_burst();
  EVE_cmd_dl_burst(CMD_DLSTART);
  EVE_cmd_dl_burst(DL_CLEAR_COLOR_RGB | 0x000000);
  EVE_cmd_dl_burst(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);
  EVE_cmd_dl_burst(DL_VERTEX_FORMAT);

  String title = configManager.getDashboardName();
  if (title == "Unknown")
    title = "MERCEDES-AMG";

  EVE_cmd_text_burst(EVE_HSIZE / 2, EVE_VSIZE / 2 - 20, 31, EVE_OPT_CENTER,
                     title.c_str());
  EVE_cmd_text_burst(EVE_HSIZE / 2, EVE_VSIZE / 2 + 20, 29, EVE_OPT_CENTER,
                     configManager.getVersion().c_str());

  EVE_cmd_dl_burst(DL_DISPLAY);
  EVE_cmd_dl_burst(CMD_SWAP);
  EVE_end_cmd_burst();

  delay(2000);
}

void UIRenderer::draw() {
  const TelemetryPacket &data = DataStore::getInstance().getTelemetry();
  checkPopupTriggers(data);

  initDisplayList();

  if (data.flag != 0) {
    drawFlag(data.flag);
  } else {
    // Draw Current Page
    const Page *page = configManager.getPage(currentPageId);
    if (!page && !configManager.getPages().empty()) {
      // Fallback to first page if current ID invalid
      page = &configManager.getPages()[0];
    }

    if (page) {
      drawPage(page, data);
    }

    // Draw ERS overlay if needed (or move to Config)
    drawERS(data);
  }

  if (activePopup) {
    drawPopup();
  }

  EVE_start_cmd_burst();
  EVE_cmd_dl_burst(DL_DISPLAY);
  EVE_cmd_dl_burst(CMD_SWAP);
  EVE_end_cmd_burst();
}

void UIRenderer::initDisplayList() {
  EVE_start_cmd_burst();
  EVE_cmd_dl_burst(CMD_DLSTART);
  EVE_cmd_dl_burst(DL_CLEAR_COLOR_RGB | 0x000000);
  EVE_cmd_dl_burst(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);
  EVE_cmd_dl_burst(DL_VERTEX_FORMAT);
  EVE_end_cmd_burst();
}

void UIRenderer::drawPage(const Page *page, const TelemetryPacket &data) {
  EVE_start_cmd_burst();
  for (const auto &elem : page->elements) {
    drawElement(elem, data);
  }
  EVE_end_cmd_burst();
}

void UIRenderer::drawElement(const UIElement &elem,
                             const TelemetryPacket &data) {
  EVE_cmd_dl_burst(COLOR_RGB((elem.color >> 16) & 0xFF,
                             (elem.color >> 8) & 0xFF, elem.color & 0xFF));

  if (elem.type == UI_ELEMENT_LINE) {
    EVE_cmd_dl_burst(DL_BEGIN | EVE_LINES);
    EVE_cmd_dl_burst(LINE_WIDTH(elem.width * 16));
    EVE_cmd_dl_burst(VERTEX2F(elem.x, elem.y));
    EVE_cmd_dl_burst(VERTEX2F(elem.x2, elem.y2));
    EVE_cmd_dl_burst(DL_END);
  } else if (elem.type == UI_ELEMENT_RECT) {
    EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
    EVE_cmd_dl_burst(LINE_WIDTH(elem.width * 16));
    EVE_cmd_dl_burst(VERTEX2F(elem.x, elem.y));
    EVE_cmd_dl_burst(VERTEX2F(elem.x2, elem.y2));
    EVE_cmd_dl_burst(DL_END);
  } else if (elem.type == UI_ELEMENT_TEXT) {
    EVE_cmd_text_burst(elem.x, elem.y, elem.font, elem.align,
                       elem.text.c_str());
  } else if (elem.type == UI_ELEMENT_NUMBER) {
    char buffer[32];
    if (elem.dataSource == "speed") {
      EVE_cmd_number_burst(elem.x, elem.y, elem.font, elem.align,
                           (int)data.speed);
    } else if (elem.dataSource == "gear") {
      if (data.gear == 0)
        EVE_cmd_text_burst(elem.x, elem.y, elem.font, elem.align, "N");
      else if (data.gear == -1)
        EVE_cmd_text_burst(elem.x, elem.y, elem.font, elem.align, "R");
      else
        EVE_cmd_number_burst(elem.x, elem.y, elem.font, elem.align,
                             (int)data.gear);

    } else if (elem.dataSource == "rpm") {
      EVE_cmd_number_burst(elem.x, elem.y, elem.font, elem.align,
                           (int)data.rpm);
    } else if (elem.dataSource == "delta") {
      secondsToTime(data.delta, buffer);
      EVE_cmd_text_burst(elem.x, elem.y, elem.font, elem.align, buffer);
    } else if (elem.dataSource == "brakeBias") {
      snprintf(buffer, 32, elem.format.c_str(), data.brakeBias);
      EVE_cmd_text_burst(elem.x, elem.y, elem.font, elem.align, buffer);
    } else if (elem.dataSource == "fuelTarget") {
      snprintf(buffer, 32, elem.format.c_str(), data.fuelTarget);
      EVE_cmd_text_burst(elem.x, elem.y, elem.font, elem.align, buffer);
    }
  }
}

void UIRenderer::drawERS(const TelemetryPacket &data) {
  // Can also be moved to config if we define specific ERS elements
}

void UIRenderer::drawFlag(int flag) {
  EVE_start_cmd_burst();
  EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
  if (flag == 1)
    EVE_cmd_dl_burst(COLOR_RGB(255, 255, 0)); // Yellow
  else if (flag == 2)
    EVE_cmd_dl_burst(COLOR_RGB(255, 0, 0)); // Red

  EVE_cmd_dl_burst(VERTEX2F(0, 0));
  EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE, EVE_VSIZE));
  EVE_cmd_dl_burst(DL_END);

  EVE_cmd_dl_burst(COLOR_RGB(0, 0, 0)); // Black Text
  EVE_cmd_text_burst(EVE_HSIZE / 2, EVE_VSIZE / 2, 31, EVE_OPT_CENTER,
                     flag == 1 ? "YELLOW FLAG" : "RED FLAG");
  EVE_end_cmd_burst();
}

void UIRenderer::checkPopupTriggers(const TelemetryPacket &data) {
  bool triggered = false;
  String triggerName = "";

  if (data.brakeBias != prevBrakeBias && prevBrakeBias != -1) {
    triggerName = "brakeBias";
    triggered = true;
  }
  prevBrakeBias = data.brakeBias;

  if (fabs(data.fuelTarget - prevFuelTarget) > 0.01 && prevFuelTarget != -1) {
    triggerName = "fuelTarget";
    triggered = true;
  }
  prevFuelTarget = data.fuelTarget;

  if (triggered) {
    const Popup *popup = configManager.getPopup(triggerName);
    if (popup) {
      activePopup = popup;
      popupStartTime = millis();
    }
  }

  if (activePopup && millis() - popupStartTime > activePopup->duration) {
    activePopup = nullptr;
  }
}

void UIRenderer::drawPopup() {
  if (!activePopup)
    return;

  // Draw popup background (generic dark box)
  EVE_start_cmd_burst();
  EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
  EVE_cmd_dl_burst(COLOR_RGB(50, 50, 50));
  EVE_cmd_dl_burst(VERTEX2F(100, 50));
  EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE - 100, EVE_VSIZE - 50));
  EVE_cmd_dl_burst(DL_END);
  EVE_end_cmd_burst();

  EVE_start_cmd_burst();
  for (const auto &elem : activePopup->elements) {
    drawElement(elem, DataStore::getInstance().getTelemetry());
  }
  EVE_end_cmd_burst();
}

void UIRenderer::secondsToTime(float seconds, char *buffer) {
  char sign = (seconds < 0) ? '-' : '+';
  if (seconds < 0)
    seconds = -seconds;
  int min = (int)seconds / 60;
  int sec = (int)seconds % 60;
  int ms = (int)((seconds - (int)seconds) * 1000);
  snprintf(buffer, 32, "%c%d.%03d", sign, sec, ms);
}

const char *UIRenderer::ersModeText(int mode) { return ""; }

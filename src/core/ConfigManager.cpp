#include "ConfigManager.h"
#include <SD.h>
#include <SPI.h>

// EVE Color macros (simplified)
#define COLOR_RGB(r, g, b) ((uint32_t)((r << 16) | (g << 8) | b))

ConfigManager::ConfigManager() {
  dashboardName = "Unknown";
  version = "0.0";
  // Initialize default LED config
  ledConfig.revLightsStart = 3;
  ledConfig.revLightsCount = 15;
  ledConfig.flagLeftStart = 0;
  ledConfig.flagRightStart = 18;
  ledConfig.flagCount = 3;
}

const std::vector<Page> &ConfigManager::getPages() const { return pages; }

const std::vector<Popup> &ConfigManager::getPopups() const { return popups; }

const LedConfig &ConfigManager::getLedConfig() const { return ledConfig; }

const Page *ConfigManager::getPage(int id) const {
  for (const auto &page : pages) {
    if (page.id == id)
      return &page;
  }
  return nullptr;
}

const Popup *ConfigManager::getPopup(const String &trigger) const {
  for (const auto &popup : popups) {
    if (popup.trigger == trigger)
      return &popup;
  }
  return nullptr;
}

String ConfigManager::getDashboardName() const { return dashboardName; }

String ConfigManager::getVersion() const { return version; }

bool ConfigManager::loadConfig(const char *filename) {
  if (!SD.begin(BUILTIN_SDCARD)) { // Teensy 4.1 builtin SD
    Serial.println("SD initialization failed!");
    return false;
  }

  File file = SD.open(filename);
  if (!file) {
    Serial.printf("Failed to open %s\n", filename);
    return false;
  }

  JsonDocument doc; // ArduinoJson v7
  DeserializationError error = deserializeJson(doc, file);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    file.close();
    return false;
  }

  dashboardName = doc["meta"]["name"].as<String>();
  version = doc["meta"]["version"].as<String>();

  // Parse Pages
  pages.clear();
  JsonArray pagesArray = doc["pages"];
  for (JsonObject pageObj : pagesArray) {
    Page page;
    page.id = pageObj["id"];
    page.name = pageObj["name"].as<String>();

    JsonArray elements = pageObj["elements"];
    for (JsonObject elem : elements) {
      page.elements.push_back(parseElement(elem));
    }
    pages.push_back(page);
  }

  // Parse Popups
  popups.clear();
  JsonArray popupsArray = doc["popups"];
  for (JsonObject popupObj : popupsArray) {
    Popup popup;
    popup.trigger = popupObj["trigger"].as<String>();
    popup.id = popupObj["id"].as<String>();
    popup.duration = popupObj["duration"] | 2000;

    JsonArray elements = popupObj["elements"];
    for (JsonObject elem : elements) {
      popup.elements.push_back(parseElement(elem));
    }
    popups.push_back(popup);
  }

  // Parse LEDs
  if (doc.containsKey("leds")) {
    JsonObject leds = doc["leds"];
    JsonObject revLights = leds["revLights"];
    ledConfig.revLightsStart = revLights["startLed"] | 3;
    ledConfig.revLightsCount = revLights["count"] | 15;

    ledConfig.revColors.clear();
    JsonArray colors = revLights["colors"];
    for (JsonVariant v : colors) {
      ledConfig.revColors.push_back(parseColor(v.as<const char *>()));
    }

    JsonObject flags = leds["flags"];
    ledConfig.flagLeftStart = flags["leftStart"] | 0;
    ledConfig.flagRightStart = flags["rightStart"] | 18;
    ledConfig.flagCount = flags["count"] | 3;
  }

  file.close();
  Serial.println("Config loaded successfully");
  return true;
}

UIElement ConfigManager::parseElement(JsonObject elem) {
  UIElement uiElem;
  uiElem.type = parseType(elem["type"]);
  uiElem.id = elem["id"].as<String>();
  uiElem.x = elem["x"] | 0;
  uiElem.y = elem["y"] | 0;
  uiElem.x2 = elem["x2"] | 0;
  uiElem.y2 = elem["y2"] | 0;
  uiElem.font = elem["font"] | 31; // Default font

  uiElem.color = parseColor(elem["color"]);
  uiElem.align = parseAlign(elem["align"]);

  uiElem.text = elem["text"].as<String>();
  uiElem.dataSource = elem["dataSource"].as<String>();
  uiElem.format = elem["format"].as<String>();
  if (uiElem.format.isEmpty())
    uiElem.format = "%d";

  uiElem.width = elem["width"] | 1;
  return uiElem;
}

uint32_t ConfigManager::parseColor(const char *colorStr) {
  if (!colorStr)
    return 0xFFFFFF; // Default white

  String s = String(colorStr);
  s.toLowerCase();

  if (s == "white")
    return 0xFFFFFF;
  if (s == "black")
    return 0x000000;
  if (s == "red")
    return 0xFF0000;
  if (s == "green")
    return 0x00FF00;
  if (s == "blue")
    return 0x0000FF;
  if (s == "yellow")
    return 0xFFFF00;
  if (s == "magenta")
    return 0xFF00FF;
  if (s == "cyan")
    return 0x00FFFF;
  if (s == "orange")
    return 0xFFA500;
  if (s == "purple")
    return 0x800080;

  // Hex parsing #RRGGBB
  if (s.startsWith("#")) {
    long number = strtol(s.substring(1).c_str(), NULL, 16);
    return (uint32_t)number;
  }

  return 0xFFFFFF;
}

int ConfigManager::parseAlign(const char *alignStr) {
  if (!alignStr)
    return 0; // Top-Left default? Or use EVE defaults

  // EVE options need to be mapped.
  // Usually defined in EVE.h but we don't want to include full EVE here if we
  // can avoid it, or we recreate the enums. For now, let's use simple logic and
  // map to standard EVE values. OPT_CENTER = 1536, OPT_CENTERX = 512,
  // OPT_CENTERY = 1024, OPT_RIGHTX = 2048

  String s = String(alignStr);
  s.toLowerCase();

  // These values should match EVE library constants
  const int OPT_CENTER = 1536;
  const int OPT_CENTERX = 512;
  const int OPT_CENTERY = 1024;
  const int OPT_RIGHTX = 2048;

  if (s == "center")
    return OPT_CENTER;
  if (s == "center_x")
    return OPT_CENTERX;
  if (s == "center_y")
    return OPT_CENTERY;
  if (s == "right")
    return OPT_RIGHTX |
           OPT_CENTERY; // Usually what people mean by "Right aligned text"

  return 0; // Top-Left
}

UIElementType ConfigManager::parseType(const char *typeStr) {
  if (!typeStr)
    return UI_ELEMENT_UNKNOWN;
  String s = String(typeStr);

  if (s == "text")
    return UI_ELEMENT_TEXT;
  if (s == "number")
    return UI_ELEMENT_NUMBER;
  if (s == "line")
    return UI_ELEMENT_LINE;
  if (s == "rect")
    return UI_ELEMENT_RECT;

  return UI_ELEMENT_UNKNOWN;
}

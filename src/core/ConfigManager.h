#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>

enum UIElementType {
  UI_ELEMENT_TEXT,
  UI_ELEMENT_NUMBER,
  UI_ELEMENT_LINE,
  UI_ELEMENT_RECT,
  UI_ELEMENT_UNKNOWN
};

struct UIElement {
  UIElementType type;
  String id;
  int x, y;   // Position
  int x2, y2; // For lines/rects
  int font;
  uint32_t color;
  int align;         // EVE options
  String text;       // For static text
  String dataSource; // Key for TelemetryPacket data
  String format;     // printf format for numbers
  int width;         // Line width
};

struct Page {
  int id;
  String name;
  std::vector<UIElement> elements;
};

struct Popup {
  String trigger; // Data source name that triggers this
  String id;
  int duration; // ms
  std::vector<UIElement> elements;
};

struct LedConfig {
  int revLightsStart;
  int revLightsCount;
  int flagLeftStart;
  int flagRightStart;
  int flagCount;
  std::vector<uint32_t> revColors;
};

class ConfigManager {
public:
  ConfigManager();
  bool loadConfig(const char *filename);

  // Getters
  const std::vector<Page> &getPages() const;
  const std::vector<Popup> &getPopups() const;
  const LedConfig &getLedConfig() const;

  const Page *getPage(int id) const;
  const Popup *getPopup(const String &trigger) const;

  String getDashboardName() const;
  String getVersion() const;

private:
  std::vector<Page> pages;
  std::vector<Popup> popups;
  LedConfig ledConfig;

  String dashboardName;
  String version;

  uint32_t parseColor(const char *colorStr);
  int parseAlign(const char *alignStr);
  UIElementType parseType(const char *typeStr);
  UIElement parseElement(JsonObject elem);
};

#endif

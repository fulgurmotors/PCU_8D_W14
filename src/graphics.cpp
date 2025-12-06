#include "graphics.h"

// This will draw the common elements of the screen.
// Independantly of the ERS mode.
// This has nothing to do with the rev lights or the flag lights.

// Draw startup screen
void drawStartup() {
  EVE_start_cmd_burst();
  EVE_cmd_dl_burst(CMD_DLSTART);
  EVE_cmd_dl_burst(DL_CLEAR_COLOR_RGB | BLACK);
  EVE_cmd_dl_burst(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);
  EVE_cmd_dl_burst(DL_VERTEX_FORMAT);

  EVE_cmd_text_burst(EVE_HSIZE / 2, EVE_VSIZE / 2 - 20, 31, EVE_OPT_CENTER,
                     "MERCEDES-AMG");
  EVE_cmd_text_burst(EVE_HSIZE / 2, EVE_VSIZE / 2 + 20, 29, EVE_OPT_CENTER,
                     "PETRONAS FORMULA ONE TEAM");

  EVE_cmd_dl_burst(DL_DISPLAY);
  EVE_cmd_dl_burst(CMD_SWAP);
  EVE_end_cmd_burst();
}

void drawPopup(const char *title, const char *value) {
  EVE_start_cmd_burst();

  // Draw a semi-transparent background or just a solid box
  EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
  EVE_cmd_dl_burst(COLOR_RGB(50, 50, 50));
  EVE_cmd_dl_burst(VERTEX2F(100, 50));
  EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE - 100, EVE_VSIZE - 50));
  EVE_cmd_dl_burst(DL_END);

  // Draw Border
  EVE_cmd_dl_burst(DL_BEGIN | EVE_LINES);
  EVE_cmd_dl_burst(LINE_WIDTH(3 * 16));
  EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));
  EVE_cmd_dl_burst(VERTEX2F(100, 50));
  EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE - 100, 50));
  EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE - 100, 50));
  EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE - 100, EVE_VSIZE - 50));
  EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE - 100, EVE_VSIZE - 50));
  EVE_cmd_dl_burst(VERTEX2F(100, EVE_VSIZE - 50));
  EVE_cmd_dl_burst(VERTEX2F(100, EVE_VSIZE - 50));
  EVE_cmd_dl_burst(VERTEX2F(100, 50));
  EVE_cmd_dl_burst(DL_END);

  // Draw Text
  EVE_cmd_text_burst(EVE_HSIZE / 2, 80, 29, EVE_OPT_CENTER, title);
  EVE_cmd_text_burst(EVE_HSIZE / 2, EVE_VSIZE / 2 + 10, 31, EVE_OPT_CENTER,
                     value);

  EVE_end_cmd_burst();
}

// Clear the screen and start a new display list
void initRefreshDisplay() {
  EVE_start_cmd_burst();
  EVE_cmd_dl_burst(
      CMD_DLSTART); // Instruct the co-processor to start a new display list
  EVE_cmd_dl_burst(DL_CLEAR_COLOR_RGB |
                   BLACK); // Set the default clear color to black
  EVE_cmd_dl_burst(DL_CLEAR | CLR_COL | CLR_STN |
                   CLR_TAG); // Clear the screen - this and the previous prevent
                             // artifacts between lists, Attributes are the
                             // color, stencil and tag buffers
  EVE_cmd_dl_burst(DL_VERTEX_FORMAT);
  EVE_end_cmd_burst();
}

// Draw main page common elements
void drawMainCommon() {
  EVE_start_cmd_burst();

  EVE_cmd_dl_burst(DL_BEGIN | EVE_LINES);

  EVE_cmd_dl_burst(LINE_WIDTH(1U * 16U));
  EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));

  // Top horizontal line
  EVE_cmd_dl_burst(VERTEX2F(0, 24));
  EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE, 24));

  // Side vertical lines
  EVE_cmd_dl_burst(VERTEX2F(22, 24));
  EVE_cmd_dl_burst(VERTEX2F(22, EVE_VSIZE));

  EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE - 22, 24));
  EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE - 22, EVE_VSIZE));

  // Middle vertical lines
  EVE_cmd_dl_burst(VERTEX2F(186, 24));
  EVE_cmd_dl_burst(VERTEX2F(186, EVE_VSIZE));

  EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE - 186, 24));
  EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE - 186, EVE_VSIZE));

  // Middle top horizontal line
  EVE_cmd_dl_burst(VERTEX2F(22, 85));
  EVE_cmd_dl_burst(VERTEX2F(186, 85));

  EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE - 22, 85));
  EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE - 186, 85));

  // Middle bottom horizontal line
  EVE_cmd_dl_burst(VERTEX2F(22, EVE_VSIZE - 61));
  EVE_cmd_dl_burst(VERTEX2F(186, EVE_VSIZE - 61));

  EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE - 22, EVE_VSIZE - 61));
  EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE - 186, EVE_VSIZE - 61));

  // Bottom horizontal lines
  EVE_cmd_dl_burst(VERTEX2F(186, EVE_VSIZE - 36));
  EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE - 186, EVE_VSIZE - 36));

  EVE_cmd_dl_burst(VERTEX2F(186, EVE_VSIZE - 72));
  EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE - 186, EVE_VSIZE - 72));

  EVE_cmd_dl_burst(VERTEX2F(186, EVE_VSIZE - 108));
  EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE - 186, EVE_VSIZE - 108));

  EVE_cmd_dl_burst(VERTEX2F(186, EVE_VSIZE - 144));
  EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE - 186, EVE_VSIZE - 144));

  EVE_cmd_dl_burst(DL_END);

  EVE_end_cmd_burst();
}

// The function above draws lines at this coordinates:
// Top horizontal line: (0, 24) to (480, 24)
// Side vertical lines: (22, 24) to (22, 272) and (458, 24) to (458, 272)
// Middle vertical lines: (186, 24) to (186, 272) and (294, 24) to (294, 272)
// Middle top horizontal line: (22, 85) to (186, 85) and (294, 85) to (458, 85)
// Middle bottom horizontal line: (22, 211) to (186, 211) and (294, 211) to
// (458, 211) Bottom horizontal lines: (186, 236) to (294, 236) and (186, 200)
// to (294, 200) and (186, 164) to (294, 164) and (186, 128) to (294, 128)

// Draw the main data of the screen
void drawMainData(gameDataContext_t gameContext) {
  char buffer[32];
  EVE_start_cmd_burst();
  EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));

  // Speed
  EVE_cmd_text_burst(0, 9, TEXT_SIZE, EVE_OPT_CENTERY, "SPEED");
  EVE_cmd_number_burst(60, 11, 24, EVE_OPT_CENTER, gameContext.speed);

  // Laps
  EVE_cmd_text_burst(EVE_HSIZE - 2, 11, TEXT_SIZE,
                     EVE_OPT_CENTERY | EVE_OPT_RIGHTX, "LAP");
  EVE_cmd_number_burst(EVE_HSIZE - 25, 11, 24, EVE_OPT_CENTERY | EVE_OPT_RIGHTX,
                       gameContext.lap);

  // Gear
  switch (gameContext.gear) {
  case 0:
    EVE_cmd_text_burst(EVE_HSIZE / 2, 76, 31, EVE_OPT_CENTER, "N");
    break;
  case -1:
    EVE_cmd_text_burst(EVE_HSIZE / 2, 76, 31, EVE_OPT_CENTER, "R");
    break;
  default:
    EVE_cmd_number_burst(EVE_HSIZE / 2, 76, 31, EVE_OPT_CENTER,
                         gameContext.gear);
    break;
  }

  // Delta
  EVE_cmd_dl_burst(COLOR_RGB(200, 0, 200));
  secondsToTime(gameContext.delta, buffer);
  EVE_cmd_text_burst(186 - 4, 54, 31, EVE_OPT_CENTERY | EVE_OPT_RIGHTX, buffer);

  // Brake Bias
  EVE_cmd_dl_burst(COLOR_RGB(150, 120, 10));
  // String(gameContext.brakeBias).substring(0,
  // String(gameContext.brakeBias).indexOf('.') + 2).c_str() Assuming brakeBias
  // is int based on struct, but code treated it as float-like string
  // manipulation? Struct says int brakeBias. So just print it. If it was float,
  // we'd use %.1f
  snprintf(buffer, sizeof(buffer), "%d", gameContext.brakeBias);
  EVE_cmd_text_burst(EVE_HSIZE - 104, 54, 31, EVE_OPT_CENTER, buffer);

  // Battery
  EVE_cmd_dl_burst(COLOR_RGB(0, 255, 0));
  snprintf(buffer, sizeof(buffer), "%d", gameContext.battery);
  EVE_cmd_text_burst(EVE_HSIZE / 2, EVE_VSIZE - 90, TEXT_SIZE_LARGE,
                     EVE_OPT_CENTER, buffer);
  EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));
  EVE_cmd_text_burst(186 + 4, EVE_VSIZE - 100, 16, EVE_OPT_CENTERY, "BATT");

  // Last Lap Fuel
  EVE_cmd_dl_burst(COLOR_RGB(255, 0, 0));
  snprintf(buffer, sizeof(buffer), "%.2f", gameContext.lastLapFuel);
  EVE_cmd_text_burst(EVE_HSIZE / 2, EVE_VSIZE - 54, TEXT_SIZE_LARGE,
                     EVE_OPT_CENTER, buffer);
  EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));
  EVE_cmd_text_burst(186 + 4, EVE_VSIZE - 64, 16, EVE_OPT_CENTERY, "LL");

  // Last Lap Time
  EVE_cmd_dl_burst(COLOR_RGB(10, 200, 200));
  snprintf(buffer, sizeof(buffer), "%.2f", gameContext.lastLapTime);
  EVE_cmd_text_burst(EVE_HSIZE - 104, EVE_VSIZE - 30, TEXT_SIZE_LARGE,
                     EVE_OPT_CENTER, buffer);
  EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));
  EVE_cmd_text_burst(EVE_HSIZE - 186 + 4, EVE_VSIZE - 61 + 8, 16,
                     EVE_OPT_CENTERY, "LAST LAP");

  // DRS
  // TODO: Draw DRS

  // ERS Level
  // TODO: Draw ERS Level

  EVE_end_cmd_burst();
}

// Draw the yellow flag
void drawYellowFlag(bool flagBlink) {
  EVE_start_cmd_burst();
  EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
  if (flagBlink) {
    EVE_cmd_dl_burst(COLOR_RGB(250, 180, 0));
  } else {
    EVE_cmd_dl_burst(COLOR_RGB(0, 0, 0));
  }
  EVE_cmd_dl_burst(VERTEX2F(0, 0));
  EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE, EVE_VSIZE));
  EVE_cmd_dl_burst(DL_END);
  if (flagBlink) {
    EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));
  } else {
    EVE_cmd_dl_burst(COLOR_RGB(250, 180, 0));
  }
  EVE_cmd_text_burst(EVE_HSIZE / 2, EVE_VSIZE / 2, 31, EVE_OPT_CENTER,
                     "Yellow Flag");
  EVE_end_cmd_burst();
}

// Draw the red flag
void drawRedFlag(bool flagBlink) {
  EVE_start_cmd_burst();
  EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
  if (flagBlink) {
    EVE_cmd_dl_burst(COLOR_RGB(200, 0, 0));
  } else {
    EVE_cmd_dl_burst(COLOR_RGB(0, 0, 0));
  }
  EVE_cmd_dl_burst(VERTEX2F(0, 0));
  EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE, EVE_VSIZE));
  EVE_cmd_dl_burst(DL_END);
  if (flagBlink) {
    EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));
  } else {
    EVE_cmd_dl_burst(COLOR_RGB(200, 0, 0));
  }
  EVE_cmd_text_burst(EVE_HSIZE / 2, EVE_VSIZE / 2, 31, EVE_OPT_CENTER,
                     "Red Flag");
  EVE_end_cmd_burst();
}

// Draw the ERS mode 0
void drawERSMode(gameDataContext_t gameContext) {
  char buffer[32];
  EVE_start_cmd_burst();

  if (gameContext.ERSMode == 0) {
    // Brake temperatures
    EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));
    snprintf(buffer, sizeof(buffer), "%d", gameContext.frontLeftBrakeTemp);
    EVE_cmd_text_burst(22 + 4, 85 + 4 + TEXT_SIZE_LARGE / 2, TEXT_SIZE_LARGE,
                       EVE_OPT_CENTERY, buffer);
    snprintf(buffer, sizeof(buffer), "%d", gameContext.frontRightBrakeTemp);
    EVE_cmd_text_burst(458 - 4, 85 + 4 + TEXT_SIZE_LARGE / 2, TEXT_SIZE_LARGE,
                       EVE_OPT_RIGHTX | EVE_OPT_CENTERY, buffer);
    snprintf(buffer, sizeof(buffer), "%d", gameContext.rearLeftBrakeTemp);
    EVE_cmd_text_burst(22 + 4, EVE_VSIZE - 61 - 4 - TEXT_SIZE_LARGE / 2,
                       TEXT_SIZE_LARGE, EVE_OPT_CENTERY, buffer);
    snprintf(buffer, sizeof(buffer), "%d", gameContext.rearRightBrakeTemp);
    EVE_cmd_text_burst(458 - 4, EVE_VSIZE - 61 - 4 - TEXT_SIZE_LARGE / 2,
                       TEXT_SIZE_LARGE, EVE_OPT_RIGHTX | EVE_OPT_CENTERY,
                       buffer);

    // Fuel target
    EVE_cmd_dl_burst(COLOR_RGB(255, 0, 0));
    snprintf(buffer, sizeof(buffer), "%.2f", gameContext.fuelTarget);
    EVE_cmd_text_burst(EVE_HSIZE / 2, EVE_VSIZE - 36 / 2, TEXT_SIZE_LARGE,
                       EVE_OPT_CENTER, buffer);
    EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));
    EVE_cmd_text_burst(186 + 4, EVE_VSIZE - 36 + 8, 16, EVE_OPT_CENTERY, "F");
  }

  // Tyre temperatures
  if (gameContext.ERSMode == 0)
    EVE_cmd_dl_burst(COLOR_RGB(0, 250, 154));
  else
    EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));

  snprintf(buffer, sizeof(buffer), "%d", gameContext.frontLeftTyreTemp);
  EVE_cmd_text_burst(186 - 4, 85 + 4 + TEXT_SIZE_LARGE / 2, TEXT_SIZE_LARGE,
                     EVE_OPT_RIGHTX | EVE_OPT_CENTERY, buffer);
  snprintf(buffer, sizeof(buffer), "%d", gameContext.frontRightTyreTemp);
  EVE_cmd_text_burst(EVE_HSIZE - 186 + 4, 85 + 4 + TEXT_SIZE_LARGE / 2,
                     TEXT_SIZE_LARGE, EVE_OPT_CENTERY, buffer);
  snprintf(buffer, sizeof(buffer), "%d", gameContext.rearLeftTyreTemp);
  EVE_cmd_text_burst(186 - 4, EVE_VSIZE - 61 - 4 - TEXT_SIZE_LARGE / 2,
                     TEXT_SIZE_LARGE, EVE_OPT_RIGHTX | EVE_OPT_CENTERY, buffer);
  snprintf(buffer, sizeof(buffer), "%d", gameContext.rearRightTyreTemp);
  EVE_cmd_text_burst(EVE_HSIZE - 186 + 4,
                     EVE_VSIZE - 61 - 4 - TEXT_SIZE_LARGE / 2, TEXT_SIZE_LARGE,
                     EVE_OPT_CENTERY, buffer);

  // ERS mode
  // Draw the background color of the ERS mode
  if (gameContext.ERSMode != 0) {
    switch (gameContext.ERSMode) {
    case 1:
      EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));
      break;
    case 2:
      EVE_cmd_dl_burst(COLOR_RGB(0, 255, 0));
      break;
    case 3:
      EVE_cmd_dl_burst(COLOR_RGB(0, 0, 255));
      break;
    case 4:
    case 5:
    case 6:
      EVE_cmd_dl_burst(COLOR_RGB(238, 130, 238));
      break;
    }
    EVE_cmd_dl(DL_BEGIN | EVE_RECTS);
    EVE_cmd_dl_burst(VERTEX2F(186 + 1, EVE_VSIZE - 36 * 4 + 1));
    EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE - 186 - 1, EVE_VSIZE - 36 * 3 - 1));
    EVE_cmd_dl_burst(DL_END);
  }

  // Draw the text of the ERS mode
  if (gameContext.ERSMode == 1)
    EVE_cmd_dl_burst(COLOR_RGB(0, 0, 0));
  else
    EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));
  EVE_cmd_text_burst(EVE_HSIZE / 2, EVE_VSIZE - (3 * 36 + 36 / 2),
                     TEXT_SIZE_LARGE, EVE_OPT_CENTER,
                     ERSModeText(gameContext.ERSMode));

  EVE_end_cmd_burst();
}

// Draw the FPS and the delta time between frames
void drawFPS(uint32_t deltaTime) {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%lu ms %.1f fps", deltaTime,
           1000.0 / deltaTime);
  EVE_start_cmd_burst();
  EVE_cmd_dl_burst(COLOR_RGB(255, 255, 255));
  EVE_cmd_text_burst(EVE_HSIZE / 2, 15, 22, EVE_OPT_CENTER, buffer);
  EVE_end_cmd_burst();
}

// This function convert a float delta time ( negative or positive ) to a
// string.
void secondsToTime(float seconds, char *buffer) {
  int minutes = 0;
  int secondsInt = 0;
  int milliseconds = 0;
  char sign = '+';

  if (seconds < 0) {
    sign = '-';
    seconds *= -1;
  }

  minutes = (int)seconds / 60;
  secondsInt = (int)seconds - (minutes * 60);
  milliseconds = (int)((seconds - (int)seconds) * 1000);

  snprintf(buffer, 32, "%c%d.%03d", sign, secondsInt, milliseconds);
}

// This function returns the ERS mode as a string
const char *ERSModeText(int ERSMode) {
  switch (ERSMode) {
  case 0:
    return "0 CHAR";
  case 1:
    return "1 MINUS";
  case 2:
    return "2 PLUS";
  case 3:
    return "3 MAX";
  case 4:
    return "4 OVRTK";
  case 5:
    return "5 QUALI";
  case 6:
    return "6 HOT";
  default:
    return "ERR";
  }
}
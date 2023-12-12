#ifndef gameDataInput
#define gameDataInput

#include <Arduino.h>
#include "gameDataContext.h"

#define BUFFER_SIZE 32
#define START_MARK '<'
#define END_MARK '>'


void recvWithStartEndMarkers(char* receivedChars, bool* newData);
void parseReceivedData(char* tempChars, gameDataContext_p gameData);

#endif
#include "gameDataInput.h"

//============

/*

void loop() {
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        showParsedData();
        newData = false;
    }
}
*/

void recvWithStartEndMarkers(char* receivedChars, bool* newData) {
    static bool recvInProgress = false;
    static byte index = 0;
    char rc;

    while (Serial.available() > 0 && *newData == false) {
        rc = Serial.read();
        if (recvInProgress == true) {
            if (rc != END_MARK) {
                receivedChars[index] = rc;
                index++;
                if (index >= BUFFER_SIZE) {
                    index = BUFFER_SIZE - 1;
                }
            }
            else {
                receivedChars[index] = '\0'; // terminate the string
                recvInProgress = false;
                index = 0;
                *newData = true;
            }
        }

        else if (rc == START_MARK) {
            recvInProgress = true;
        }
    }
}

void parseReceivedData(char* tempChars, gameDataContext_p gameData){
    char * strtokIndx = (char*) malloc(BUFFER_SIZE * sizeof(char));
    char* dataCode = (char*) malloc(BUFFER_SIZE * sizeof(char));
    char* dataValue = (char*) malloc(BUFFER_SIZE * sizeof(char));

    strtokIndx = strtok(tempChars,","); // get the first part (dataCode)
    strcpy(dataCode, strtokIndx);
 
    strtokIndx = strtok(NULL, ",");     //This continues from the previous pointer (get the dataValue)
    strcpy(dataValue, strtokIndx);

    String dataCodeString = String(dataCode).substring(0, 3);


    if(!strcmp(dataCodeString.c_str(), "spd")){
        gameData->speed = atoi(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), "rpm")){
        gameData->rpm = atoi(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), "lap")){
        gameData->lap = atoi(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), "dlt")){
        gameData->delta = atof(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), "gea")){
        gameData->gear = atoi(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), "flg")){
        gameData->flag = atoi(dataValue);
    }
    else{
        Serial.println("Unknown data code");
    }
}
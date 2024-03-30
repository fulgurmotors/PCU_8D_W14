#include "gameDataInput.h"

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
    char dataCode[BUFFER_SIZE];
    char dataValue[BUFFER_SIZE];

    char* strtokIndx = strtok(tempChars,","); // get the first part (dataCode)
    strcpy(dataCode, strtokIndx);
 
    strtokIndx = strtok(NULL, ",");     //This continues from the previous pointer (get the dataValue)
    strcpy(dataValue, strtokIndx);

    String dataCodeString = String(dataCode).substring(0, 3);

    if(dataCodeString == SPEED_CODE){
        gameData->speed = strtol(dataValue, NULL, 10);
    }
    else if(dataCodeString == RPM_CODE){
        gameData->rpm = strtol(dataValue, NULL, 10);
    }
    else if(dataCodeString == LAP_CODE){
        gameData->lap = strtol(dataValue, NULL, 10);
    }
    else if(dataCodeString == DELTA_CODE){
        gameData->delta = strtof(dataValue, NULL);
    }
    else if(dataCodeString == GEAR_CODE){
        if(dataValue[0] != 'N' && dataValue[0] != 'R'){
            gameData->gear = strtol(dataValue, NULL, 10);
        }
        else if(dataValue[0] == 'N'){
            gameData->gear = 0;
        }
        else if(dataValue[0] == 'R'){
            gameData->gear = -1;
        }
    }
    else if(dataCodeString == FLAG_CODE){
        gameData->flag = strtol(dataValue, NULL, 10);
    }
    else if(dataCodeString == FRONT_LEFT_TYRE_TEMP_CODE){
        gameData->frontLeftTyreTemp = strtol(dataValue, NULL, 10);
    }
    else if(dataCodeString == FRONT_RIGHT_TYRE_TEMP_CODE){
        gameData->frontRightTyreTemp = strtol(dataValue, NULL, 10);
    }
    else if(dataCodeString == REAR_LEFT_TYRE_TEMP_CODE){
        gameData->rearLeftTyreTemp = strtol(dataValue, NULL, 10);
    }
    else if(dataCodeString == REAR_RIGHT_TYRE_TEMP_CODE){
        gameData->rearRightTyreTemp = strtol(dataValue, NULL, 10);
    }
    else if(dataCodeString == FRONT_LEFT_BRAKE_TEMP_CODE){
        gameData->frontLeftBrakeTemp = strtol(dataValue, NULL, 10);
    }
    else if(dataCodeString == FRONT_RIGHT_BRAKE_TEMP_CODE){
        gameData->frontRightBrakeTemp = strtol(dataValue, NULL, 10);
    }
    else if(dataCodeString == REAR_LEFT_BRAKE_TEMP_CODE){
        gameData->rearLeftBrakeTemp = strtol(dataValue, NULL, 10);
    }
    else if(dataCodeString == REAR_RIGHT_BRAKE_TEMP_CODE){
        gameData->rearRightBrakeTemp = strtol(dataValue, NULL, 10);
    }
    else if(dataCodeString == PIT_LIMITER_CODE){
        gameData->pitLimiter = strtol(dataValue, NULL, 10);
    }
    else if(dataCodeString == DRS_CODE){
        gameData->DRS = strtol(dataValue, NULL, 10);
    }
    else if(dataCodeString == BRAKE_BIAS_CODE){
        gameData->brakeBias = strtol(dataValue, NULL, 10);
    }
    else if(dataCodeString == ERS_MODE_CODE){
        gameData->ERSMode = strtol(dataValue, NULL, 10);
    }
    else if(dataCodeString == ERS_LEVEL_CODE){
        gameData->ERSLevel = strtof(dataValue, NULL);
    }
    else if(dataCodeString == LAST_LAP_FUEL_CODE){
        gameData->lastLapFuel = strtof(dataValue, NULL);
    }
    else if(dataCodeString == ESTIMATED_LAP_TIME_CODE){
        gameData->EstimatedLapTime = strtof(dataValue, NULL);
    }
    else if(dataCodeString == BATTERY_CODE){
        gameData->battery = strtol(dataValue, NULL, 10);
    }
    else if(dataCodeString == LAST_LAP_TIME_CODE){
        gameData->lastLapTime = strtof(dataValue, NULL);
    }
    else if(dataCodeString == LAST_LAP_TIME_CODE){
        gameData->lastLapTime = strtof(dataValue, NULL);
    }
    else if(dataCodeString == FUEL_TARGET_CODE){
        gameData->fuelTarget = strtof(dataValue, NULL);
    }
    else{
        Serial.println("Unknown data code");
    }
}
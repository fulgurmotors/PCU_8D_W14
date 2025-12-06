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
    if (strtokIndx == NULL) return;
    strcpy(dataCode, strtokIndx);
 
    strtokIndx = strtok(NULL, ",");     //This continues from the previous pointer (get the dataValue)
    if (strtokIndx == NULL) return;
    strcpy(dataValue, strtokIndx);

    // No need to create a substring, just compare the first 3 chars
    // We can use strncmp for safer comparison or just strcmp if we are sure about null termination

    if(strncmp(dataCode, SPEED_CODE, 3) == 0){
        gameData->speed = strtol(dataValue, NULL, 10);
    }
    else if(strncmp(dataCode, RPM_CODE, 3) == 0){
        gameData->rpm = strtol(dataValue, NULL, 10);
    }
    else if(strncmp(dataCode, LAP_CODE, 3) == 0){
        gameData->lap = strtol(dataValue, NULL, 10);
    }
    else if(strncmp(dataCode, DELTA_CODE, 3) == 0){
        gameData->delta = strtof(dataValue, NULL);
    }
    else if(strncmp(dataCode, GEAR_CODE, 3) == 0){
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
    else if(strncmp(dataCode, FLAG_CODE, 3) == 0){
        gameData->flag = strtol(dataValue, NULL, 10);
    }
    else if(strncmp(dataCode, FRONT_LEFT_TYRE_TEMP_CODE, 3) == 0){
        gameData->frontLeftTyreTemp = strtol(dataValue, NULL, 10);
    }
    else if(strncmp(dataCode, FRONT_RIGHT_TYRE_TEMP_CODE, 3) == 0){
        gameData->frontRightTyreTemp = strtol(dataValue, NULL, 10);
    }
    else if(strncmp(dataCode, REAR_LEFT_TYRE_TEMP_CODE, 3) == 0){
        gameData->rearLeftTyreTemp = strtol(dataValue, NULL, 10);
    }
    else if(strncmp(dataCode, REAR_RIGHT_TYRE_TEMP_CODE, 3) == 0){
        gameData->rearRightTyreTemp = strtol(dataValue, NULL, 10);
    }
    else if(strncmp(dataCode, FRONT_LEFT_BRAKE_TEMP_CODE, 3) == 0){
        gameData->frontLeftBrakeTemp = strtol(dataValue, NULL, 10);
    }
    else if(strncmp(dataCode, FRONT_RIGHT_BRAKE_TEMP_CODE, 3) == 0){
        gameData->frontRightBrakeTemp = strtol(dataValue, NULL, 10);
    }
    else if(strncmp(dataCode, REAR_LEFT_BRAKE_TEMP_CODE, 3) == 0){
        gameData->rearLeftBrakeTemp = strtol(dataValue, NULL, 10);
    }
    else if(strncmp(dataCode, REAR_RIGHT_BRAKE_TEMP_CODE, 3) == 0){
        gameData->rearRightBrakeTemp = strtol(dataValue, NULL, 10);
    }
    else if(strncmp(dataCode, PIT_LIMITER_CODE, 3) == 0){
        gameData->pitLimiter = strtol(dataValue, NULL, 10);
    }
    else if(strncmp(dataCode, DRS_CODE, 3) == 0){
        gameData->DRS = strtol(dataValue, NULL, 10);
    }
    else if(strncmp(dataCode, BRAKE_BIAS_CODE, 3) == 0){
        gameData->brakeBias = strtol(dataValue, NULL, 10);
    }
    else if(strncmp(dataCode, ERS_MODE_CODE, 3) == 0){
        gameData->ERSMode = strtol(dataValue, NULL, 10);
    }
    else if(strncmp(dataCode, ERS_LEVEL_CODE, 3) == 0){
        gameData->ERSLevel = strtof(dataValue, NULL);
    }
    else if(strncmp(dataCode, LAST_LAP_FUEL_CODE, 3) == 0){
        gameData->lastLapFuel = strtof(dataValue, NULL);
    }
    else if(strncmp(dataCode, ESTIMATED_LAP_TIME_CODE, 3) == 0){
        gameData->EstimatedLapTime = strtof(dataValue, NULL);
    }
    else if(strncmp(dataCode, BATTERY_CODE, 3) == 0){
        gameData->battery = strtol(dataValue, NULL, 10);
    }
    else if(strncmp(dataCode, LAST_LAP_TIME_CODE, 3) == 0){
        gameData->lastLapTime = strtof(dataValue, NULL);
    }
    else if(strncmp(dataCode, FUEL_TARGET_CODE, 3) == 0){
        gameData->fuelTarget = strtof(dataValue, NULL);
    }
    else{
        // Serial.println("Unknown data code");
    }
}
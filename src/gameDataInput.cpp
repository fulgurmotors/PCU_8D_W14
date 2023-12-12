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
    char * strtokIndx = (char*) malloc(BUFFER_SIZE * sizeof(char));
    char* dataCode = (char*) malloc(BUFFER_SIZE * sizeof(char));
    char* dataValue = (char*) malloc(BUFFER_SIZE * sizeof(char));

    strtokIndx = strtok(tempChars,","); // get the first part (dataCode)
    strcpy(dataCode, strtokIndx);
 
    strtokIndx = strtok(NULL, ",");     //This continues from the previous pointer (get the dataValue)
    strcpy(dataValue, strtokIndx);

    String dataCodeString = String(dataCode).substring(0, 3);


    if(!strcmp(dataCodeString.c_str(), SPEED_CODE)){
        gameData->speed = atoi(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), RPM_CODE)){
        gameData->rpm = atoi(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), LAP_CODE)){
        gameData->lap = atoi(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), DELTA_CODE)){
        gameData->delta = atof(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), GEAR_CODE)){
        gameData->gear = atoi(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), FLAG_CODE)){
        gameData->flag = atoi(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), FRONT_LEFT_TYRE_TEMP_CODE)){
        gameData->frontLeftTyreTemp = atoi(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), FRONT_RIGHT_TYRE_TEMP_CODE)){
        gameData->frontRightTyreTemp = atoi(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), REAR_LEFT_TYRE_TEMP_CODE)){
        gameData->rearLeftTyreTemp = atoi(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), REAR_RIGHT_TYRE_TEMP_CODE)){
        gameData->rearRightTyreTemp = atoi(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), FRONT_LEFT_BRAKE_TEMP_CODE)){
        gameData->frontLeftBrakeTemp = atoi(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), FRONT_RIGHT_BRAKE_TEMP_CODE)){
        gameData->frontRightBrakeTemp = atoi(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), REAR_LEFT_BRAKE_TEMP_CODE)){
        gameData->rearLeftBrakeTemp = atoi(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), REAR_RIGHT_BRAKE_TEMP_CODE)){
        gameData->rearRightBrakeTemp = atoi(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), PIT_LIMITER_CODE)){
        gameData->pitLimiter = atoi(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), DRS_CODE)){
        gameData->DRS = atoi(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), BRAKE_BIAS_CODE)){
        gameData->brakeBias = atoi(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), ERS_MODE_CODE)){
        gameData->ERSMode = atoi(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), ERS_LEVEL_CODE)){
        gameData->ERSLevel = atof(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), FUEL_PER_LAP_CODE)){
        gameData->fuelPerLap = atof(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), ESTIMATED_LAP_TIME_CODE)){
        gameData->EstimatedLapTime = atof(dataValue);
    }
    else if(!strcmp(dataCodeString.c_str(), BATTERY_CODE)){
        gameData->battery = atoi(dataValue);
    }
    else{
        Serial.println("Unknown data code");
    }
}
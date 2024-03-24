#ifndef gameDataInput
#define gameDataInput

#include <Arduino.h>

#define BUFFER_SIZE 32
#define START_MARK '<'
#define END_MARK '>'

#define SPEED_CODE "SPD"
#define GEAR_CODE "GEA"
#define RPM_CODE "RPM"
#define LAP_CODE "LAP"
#define DELTA_CODE "DEL"
#define ESTIMATED_LAP_TIME_CODE "ELT"
#define FLAG_CODE "FLG"
#define FRONT_LEFT_TYRE_TEMP_CODE "FLT"
#define FRONT_RIGHT_TYRE_TEMP_CODE "FRT"
#define REAR_LEFT_TYRE_TEMP_CODE "RLT"
#define REAR_RIGHT_TYRE_TEMP_CODE "RRT"
#define FRONT_LEFT_BRAKE_TEMP_CODE "FLB"
#define FRONT_RIGHT_BRAKE_TEMP_CODE "FRB"
#define REAR_LEFT_BRAKE_TEMP_CODE "RLB"
#define REAR_RIGHT_BRAKE_TEMP_CODE "RRB"
#define PIT_LIMITER_CODE "PLM"
#define DRS_CODE "DRS"
#define BRAKE_BIAS_CODE "BRB"
#define ERS_MODE_CODE "ERM"
#define ERS_LEVEL_CODE "ERL"
#define LAST_LAP_FUEL_CODE "LLF"
#define BATTERY_CODE "BAT"
#define LAST_LAP_TIME_CODE "LLT"
#define FUEL_TARGET_CODE "FTG"

typedef struct gameDataContext_t{
    int speed;
    char gear;
    int rpm;
    int lap;

    float delta; //Delta time
    float EstimatedLapTime; //Estimated lap time
    float lastLapTime; //Last lap time

    int flag; //0 = green, 1 = yellow, 2 = red

    //Tyre temperatures
    int frontLeftTyreTemp;
    int frontRightTyreTemp;
    int rearLeftTyreTemp;
    int rearRightTyreTemp;

    //Brake temperatures
    int frontLeftBrakeTemp;
    int frontRightBrakeTemp;
    int rearLeftBrakeTemp;
    int rearRightBrakeTemp;

    bool pitLimiter;
    bool DRS;
    int brakeBias;
    
    int ERSMode;
    float ERSLevel;

    float lastLapFuel;

    int battery;

    float fuelTarget;

} gameDataContext_t;

typedef gameDataContext_t *gameDataContext_p;

void recvWithStartEndMarkers(char* receivedChars, bool* newData);
void parseReceivedData(char* tempChars, gameDataContext_p gameData);

#endif
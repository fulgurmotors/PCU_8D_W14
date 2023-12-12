#ifndef gameDataContext
#define gameDataContext

typedef struct{
    int speed;
    int gear;
    int rpm;
    int lap;
    float delta;
    int flag; //0 = green, 1 = yellow, 2 = red
} gameDataContext_t;

typedef gameDataContext_t *gameDataContext_p;



#endif
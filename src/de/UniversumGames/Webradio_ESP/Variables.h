#ifndef Variables
#define Variables

#include <Arduino.h>

extern const char *SSID = "yourSSID";
extern const char *PASSWORD = "yourPASSWORD";

extern bool next_Buton = false;
extern bool previous = false;
extern bool play_pause = false;
extern bool volumeUP = false;
extern bool volumeDOWN = false;

extern bool any()
{
    return (next_Buton || previous || play_pause || volumeUP || volumeDOWN) ? true : false;
}

extern bool anyImportant()
{
    return (next_Buton || previous || play_pause) ? true : false;
}

#endif
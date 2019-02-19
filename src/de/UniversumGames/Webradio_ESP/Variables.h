#ifndef Variables
#define Variables

#include <Arduino.h>

const char *SSID = "yourSSID";
const char *PASSWORD = "yourPASSWORD";

bool next_Buton = false;
bool previous = false;
bool play_pause = false;
bool volumeUP = false;
bool volumeDOWN = false;

extern bool any()
{
    return (next_Buton || previous || play_pause || volumeUP || volumeDOWN) ? true : false;
}

extern bool anyImportant()
{
    return (next_Buton || previous || play_pause) ? true : false;
}

#endif
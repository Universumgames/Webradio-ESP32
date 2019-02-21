#ifndef Radiostation_h
#define Radiostation_h

#include <Arduino.h>
#include "URL.h"
#include "RadioType.h"

class Radiostation{
    public:
        Radiostation(URL _url, RadioType _type){
            url = &_url;
            type = _type;
        }

        URL *getURL(){
            return url;
        }

        RadioType getRadioType(){
            return type;
        }
    private:
        URL *url;
        RadioType type;
};

extern Radiostation EinsLive(URL("dg-wdr-http-dus-dtag-cdn.cast.addradio.de", "/wdr/1live/live/mp3/128/stream.mp3", 80), MP3Stream);
#endif
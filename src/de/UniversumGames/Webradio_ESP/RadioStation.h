#ifndef Radiostation_h
#define Radiostation_h

#include <Arduino.h>
#include "URL.h"
#include "RadioType.h"

class Radiostation{
    public:
        Radiostation(){}
        Radiostation(URL _url, RadioType _type){
            url = _url;
            type = _type;
        }

        URL getURL(){
            return url;
        }

        RadioType getRadioType(){
            return type;
        }
    private:
        URL url;
        RadioType type;
};
#endif
#ifndef MP3Decoder_h
#define MP3Decoder_h

#include <Arduino.h>
#include "URL.h"
#include <VS1053.h>
#include <SPI.h>

class MP3Decoder{
    public:
        MP3Decoder(){
            volumePercent = 95;
            VS1053 _player(32,33,35);
            player = &_player;
            
        }

        void begin(){
            SPI.begin();
            Serial.println("Begin with VS1053");
            player->begin();
            Serial.println("Switching to MP3");
            player->switchToMp3Mode();
            Serial.println("Setting volume");
            player->setVolume(volumePercent);
        }

        void volumeUp(){
            volumePercent+=2;
            player->setVolume(volumePercent);
        }

        void volumeDown(){
            volumePercent-=2;
            player->setVolume(volumePercent);
        }

        void play(uint8_t buffer[32], uint8_t bytesRead){
            player->playChunk(buffer, bytesRead);
        }
    private:
        VS1053 *player;
        int volumePercent;
};
#endif
#ifndef MP3Decoder_h
#define MP3Decoder_h

#include <Arduino.h>
#include "URL.h"
#include <VS1053.h>

class MP3Decoder{
    public:
        MP3Decoder(VS1053 *_player){
            volumePercent = 95;
            player = _player;
            player->begin();
            player->switchToMp3Mode();
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
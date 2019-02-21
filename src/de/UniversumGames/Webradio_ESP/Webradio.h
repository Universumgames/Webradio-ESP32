#ifndef Webradio_h
#define Webradio_h

#include <Arduino.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <HTTPClient.h>
#include <VS1053.h>
#include <EEPROM.h>
#include "List.h"
#include "Radiostation.h"

const VS1053 player(32,33,35);

class Webradio{
    
    public:
        Webradio(){}

        void init(){
            //volume = EEPROM.readInt(eepromVolumeAdr);
            //currentStationId = EEPROM.readUInt(eepromStationIdAdr);
            Serial.println("adding station");
            stations.add(Radiostation_h::EinsLive);
            
            Serial.println("spi");
            SPI.begin();
            delay(10);
            Serial.println("player begin");
            player.begin();
            Serial.println("mp3 mode");
            player.switchToMp3Mode();
            Serial.println("volume");
            player.setVolume(volume);
        }

        void initPlaying(){
            connectToStation(currentStationId);
        }

        void volumeUp(){
            if(volume < 100){
                volume++;
            }
            updateVolume();
        }

        void volumeDown(){
            if(volume > 0){
                volume--;
            }
            updateVolume();
        }

        void connectToStation(uint id){
            if(id < stations.getLength()){
                currentStation = stations.get(id);
                currentStationId = id;
                updateStationId();
                connect();
            }
        }

        void nextStation(){
            if(currentStationId == stations.getLength()){
                currentStationId = 0;
            }else{
                currentStationId++;
            }
            connectToStation(currentStationId);
        }

        void previousStation(){
            if (currentStationId == 0)
            {
                currentStationId = stations.getLength();
            }
            else
            {
                currentStationId--;
            }
            connectToStation(currentStationId);
        }

        void play(){
            if(!initPlay){
                initPlaying();
            }
            playBool = true;
        }

        void pause(){
            playBool = false;
        }

        void addStation(Radiostation station){
            stations.add(station);
        }

        void core0Loop()
        {
            if (client.available() > 0 && connected && playBool)
            {
                size_t read = client.read(buffer, 32);
                player.playChunk(buffer, read);
            }else{
                delay(10);
            }
        }

      private:
        const int eepromVolumeAdr = 0;
        const int eepromStationIdAdr = 1;
        bool initPlay = false;
        bool playBool = true;
        bool connected = false;
        int volume = 0;
        VS1053 player = Webradio_h::player;
        Radiostation *currentStation;
        uint currentStationId = 0;
        uint8_t buffer[32];
        List<Radiostation> stations;
        WiFiClient client;

        void updateVolume(){
            //EEPROM.writeInt(eepromVolumeAdr, volume);
            player.setVolume(volume);
        }

        void updateStationId(){
            //EEPROM.writeUInt(eepromStationIdAdr, currentStationId);
        }

        void connect(){
            const int length = currentStation->getURL()->host.length();
            char *array;
            currentStation->getURL()->host.toCharArray(array, length);
            if(client.connect(array, currentStation->getURL()->port)){
                connected = true;
            }

            client.print(String("GET ") + currentStation->getURL()->path + " HTTP/1.1\r\n" +
                         "Host: " + currentStation->getURL()->host + "\r\n" +
                         "Connection: close\r\n\r\n");
        }

        void disconnect(){
            client.stop();
            connected = false;
        }
};

extern Webradio webradio;

#endif
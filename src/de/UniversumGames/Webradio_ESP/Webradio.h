#ifndef Webradio_h
#define Webradio_h

//#include <Arduino.h>
//#include <HTTPClient.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include "VS1053.h"
//#include <EEPROM.h>
//#include "List.h"
#include "Radiostation.h"
#include "URL.h"

URL einsLiveURL("dg-wdr-http-dus-dtag-cdn.cast.addradio.de", "/wdr/1live/live/mp3/128/stream.mp3", 80);
Radiostation EinsLive(einsLiveURL, MP3Stream);

class Webradio
{
  public:
    Webradio() {}

    void begin(VS1053 *_player)
    {
        player = _player;
    }

    void init()
    {
        //volume = EEPROM.readInt(eepromVolumeAdr);
        //currentStationId = EEPROM.readUInt(eepromStationIdAdr);
        Serial.println("adding station");
        addStation(Webradio_h::EinsLive);
        /*Serial.println("spi");
        SPI.begin();
        delay(100);
        Serial.println("player begin");
        player->begin();
        Serial.println("mp3 mode");
        player->switchToMp3Mode();
        Serial.println("volume");
        player->setVolume(volume);*/
    }

    void initPlaying()
    {
        connectToStation(currentStationId);
    }

    void volumeUp()
    {
        if (volume < 100)
        {
            volume++;
        }
        updateVolume();
    }

    void volumeDown()
    {
        if (volume > 0)
        {
            volume--;
        }
        updateVolume();
    }

    void connectToStation(int id)
    {
        if (id < stationssize && id >= 0)
        {
            Serial.println("less than size, more than 0");
            //currentStation = stations.get(id);
            if(!ItemisNull(&stations[id])){
                currentStation = stations[id];
                currentStationId = id;
                updateStationId();
                connect();
            }else{
                connectToStation(id-1);
            }
        }else{
            currentStation = Webradio_h::EinsLive;
            currentStationId = 0;
            updateStationId();
            connect();
        }
    }

    void nextStation()
    {
        if (currentStationId == sizeof(stations))
        {
            currentStationId = 0;
        }
        else
        {
            currentStationId++;
            if(ItemisNull(&stations[currentStationId])){
                nextStation();
            }
        }
        connectToStation(currentStationId);
    }

    void previousStation()
    {
        if (currentStationId == 0)
        {
            currentStationId = sizeof(stations);
        }
        else
        {
            currentStationId--;
            if(ItemisNull(&stations[currentStationId])){
                previousStation();
            }
        }
        connectToStation(currentStationId);
    }

    void play()
    {
        if (!initPlay)
        {
            initPlaying();
        }
        playBool = true;
    }

    void pause()
    {
        playBool = false;
    }

    void addStation(Radiostation station)
    {
        Radiostation stations2[stationssize+1];
        for(int i = 0; i< stationssize; i++){
            stations2[i] = stations[i];
        }
        stationssize++;
        stations = stations2;
        //stations = new Radiostation[stationssize];
        stations[stationssize-1] = station;
    }

    void core0Loop()
    {
        uint8_t buffer[32];
        if (client.available() > 0 && client.connected() && playBool)
        {
            size_t read = client.read(buffer, 32);
            player->playChunk(buffer, read);
        }
    }
    WiFiClient client;
  private:
    const int eepromVolumeAdr = 0;
    const int eepromStationIdAdr = 1;
    bool initPlay = false;
    bool playBool = true;
    bool connected = client.connected();
    int volume = 0;
    VS1053 *player;
    Radiostation currentStation;
    int currentStationId = 0;
    Radiostation *stations;
    int stationssize = 0;
    
    //HTTPClient httpclient;
    

    void updateVolume()
    {
        //EEPROM.writeInt(eepromVolumeAdr, volume);
        player->setVolume(volume);
    }

    void updateStationId()
    {
        //EEPROM.writeUInt(eepromStationIdAdr, currentStationId);
    }

    void connect()
    {
        //const int length = currentStation->getURL()->host.length();
        //char *array;
        //currentStation->getURL()->host.toCharArray(array, length);
        currentStation = Webradio_h::EinsLive;

        const char *host = currentStation.getURL().host;
        const char *path = currentStation.getURL().path;
        uint16_t port = currentStation.getURL().port;
        Serial.println("connect");
        /*httpclient.begin(client,"http://" + String(host) + ":" + String(port) + String(path));
        if(httpclient.GET() == HTTP_CODE_OK){
            connected = true;
            playBool = true;
            client = httpclient.getStream();
            Serial.println("connected");
        }else{
            Serial.println("not connected");
        }*/
        if (client.connect(host, port))
        {
            connected = true;
            playBool = true;
            Serial.println("Connected");
        }else{
            Serial.println("not connected");
            return;
        }

        const String get = String("GET ") + path + " HTTP/1.1\r\n" +
                           "Host: " + host + "\r\n" +
                           "Connection: close\r\n\r\n";
        client.print(get);
    }

    void disconnect()
    {
        client.stop();
        connected = false;
    }

    bool ItemisNull(Radiostation *station){
        Serial.println("is null?");
        return station->getURL().isNotDefined();
    }
};
#endif

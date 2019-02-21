#ifndef RadioStation_h
#define RadioStation_h

#include <Arduino.h>
#include "RadioType.h"
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <WiFi.h>
#include <VS1053.h>
#include <SPI.h>
#include "Queue.h"
#include "Song.h"
#include "Variables.h"
#include "useful.h"
#include "MP3Decoder.h"
#include "URL.h"

//#define TestWithoutVS1053

class RadioStation{
    public:
        /*RadioStation(String url1, RadioType type1, VS1003 *player1){
            url = url1;
            type = type1;
            player = player1;
        }*/

        RadioStation(URL url1, RadioType type1, MP3Decoder *decoder1)
        {
            url = &url1;
            type = type1;
            decoder = decoder1;
        }

        void connect(){
            if(type == MP3Stream){
                initMP3Stream();
            }else if (type == M3U8){
                //get_m3u8_master();
            }
            connected = true;
        }

        void disconnect(){
            if(connected){
                if(type == MP3Stream) {
                }else if(type == M3U8){

                }
            connected = false;
            }
            //player->stopSong();
        }

        void play(){
            playBool = true;
            if(type == MP3Stream){
                playMP3Stream();
            }else if(type == M3U8){
                //getNewM3U8Packet();
            }
        }

        void pause(){
            playBool = false;
        }

        RadioType getType(){
            return type;
        }

        /*void buffering()
        {
            if(type == M3U8){
                if (tsPlaylist->getSize() <= 5){
                    getTSPlaylist();
                }
                downloadBuffer();
            }
        }*/

      private:
        bool playBool = true;
        bool connected = false;
        MP3Decoder *decoder;
        URL *url;
        //MP3 Stream
        
        RadioType type;
        WiFiClient client;
        void initMP3Stream(){
            const int length = url->host.length();
            char array[length];
            url->host.toCharArray(array, length);
            if(client.connect(array, url->port)){
                connected = true;
                Serial.println("Connected to " + String(url->host));
            }
            client.print(String("GET ") + url->path + " HTTP/1.1\r\n" +
               "Host: " + String(url->host) + "\r\n" + 
               "Connection: close\r\n\r\n");
        }

        void playMP3Stream(){

            if (playBool && connected)
            {
                uint8_t buffer[32];
               if(client.available() > 0){
                   uint8_t read = client.read(buffer, 32);
                   decoder->play(buffer, read);
               }
            }
        }

    //M3U8 playlist "Stream"
        /*String master_m3u8[];
        String ts_playlist_Link;
        Queue<String> *tsPlaylist;
        Queue<Song> *songbuffer;
        //Queue<uint8_t> *m3ubuffer;
        size_t TSLenght = 0;
        WiFiClient *tsStream;
        void get_m3u8_master()
        {
            client.begin(url);
            int httpCode = client.GET();
            //client.get(url);
            String m3u8 = "";
            if(httpCode > 0){
                m3u8 = client.getString();
            }
            String *sep[useful::countChar(m3u8, '\n')];
            useful::split(m3u8, '\n', sep);
           
            ts_playlist_Link = master_m3u8[2];
            client.end();
            getTSPlaylist();
        }

        void getTSPlaylist(){
            client.begin(ts_playlist_Link);
            int httpCode = client.GET();
            String content = "";
            if(httpCode > 0){
                content = client.getString();
            }
            String sep[useful::countChar(content, '\n')];
            for(int i = 0; i < sizeof(sep); i++){
                String line = sep[i];
                if(!line.startsWith("#")){
                    bool alreadContained = false;
                    String *content[100];
                    tsPlaylist->getArray(content);
                    for(int i = 0; i < tsPlaylist->getSize(); i++){
                        if(content[i] == &line) alreadContained = true;
                    }
                    if(!alreadContained) tsPlaylist->enqueue(line);
                }
            }
            client.end();
        }

        void downloadBuffer(){
            if(ESP.getFreeHeap() >= 90000){
                if(!tsPlaylist->isEmpty()){
                    client.begin(tsPlaylist->front());
                    tsPlaylist->dequeue();
                    int httpCode = client.GET();
                    TSLenght = client.getSize();
                    tsStream = client.getStreamPtr();
                    Queue<uint8_t> *tsSongBuffer;
                    if(httpCode > 0){
                        while (client.connected() && (TSLenght > 0 || TSLenght == -1))
                        {
                            size_t size = stream->available();

                            if (size)
                            {
                                uint8_t tsbuffer[128];
                                int c = stream->readBytes(tsbuffer, ((size > sizeof(tsbuffer)) ? sizeof(tsbuffer) : size));

                                if (len > 0) len -= c;
                                for(int i = 0; i < sizeof(tsbuffer); i++){
                                    tsSongBuffer->enqueue(tsbuffer[i]);
                                }
                            
                            }
                        }
                        Song song(tsSongBuffer);
                        songbuffer->enqueue(song);
                    }
                    
                }
            }
        }

        void getNewM3U8Packet(){
            uint8_t data[32];
            for (int i = 0; i < sizeof(data); i++)
            {
                data[i] = songbuffer->front().getQueue()->front();
                songbuffer->front().getQueue()->dequeue();
            }
            if(songbuffer->front().isOver()) songbuffer->dequeue();
            #ifndef TestWithoutVS1053
                player->playChunk(data, 32);
            #endif
        }*/
};
#endif
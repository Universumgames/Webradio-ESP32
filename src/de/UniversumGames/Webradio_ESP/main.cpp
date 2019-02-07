/*
* This code is provided to you by UniversumGames
* This project is free to use 
* This code is coded by a single person
* I will not take care of your decive if it will be damaged
* Use this on your own risk
* Define and change the codepieces you want and/or have to change
* Have fun with this project
* If you have issues with this project, you can probably report an issue on GitHub
*/

#include <WiFi.h>
#include <WiFiMulti.h>
//#include <VS103.h>
#include "RadioStation.h"
#include "RadioType.h"
#include "Variables.h"
#include <SPI.h>
//#include "ESPVS1003.h"
#include "MP3Decoder.h"
#include "WiFiVariables.h"


//#define PCStreamerStreamFileURL "192.168.178.28/"               //if you have a programm wich provides a m3u(8) playlist in your network, define here the URL
//#define PCStreamerStreamType M3U8
#define ssid1 WiFiVariables::ssid                                      //your ssid
#define pass1 WiFiVariables::password                                  //your password

//#define USE_BUTTON_INTERRUPTS                                   //if you want to use buttons/etc. as interrupt
#define SerialUSE                                               //if you want a serial output
//#define USE_VS1053                                              //you use the VS1053 or VS1003? don´t uncomment
//#define USE_VS1003
#define USE_MP3Decoder
#define VS1003_DCS 4
#define VS1003_DREQ 32
#define VS1003_CS 15 //5
#define VS1003_RST 25
#define SPI_CS VS1003_CS
#define SPI_SCK 14   //14
#define SPI_MISO 12 //12
#define SPI_MOSI 13 //13
MP3Decoder decoder(SPI_MISO, SPI_MOSI, SPI_CS, SPI_SCK, VS1003_DCS, VS1003_DREQ, VS1003_RST);
#ifdef MP3Decoder

#endif

#ifdef USE_VS1003
#define VS1003_DCS 4
#define VS1003_DREQ 34
#define VS1003_CS SS //5
#define VS1003_RST 25
#define SPI_CS VS1003_CS
#define SPI_SCK SCK   //22
#define SPI_MISO MISO //23
#define SPI_MOSI MOSI //27
VS1003 player(VS1003_CS, VS1003_DCS, VS1003_DREQ, VS1003_RST);
//MP3Decoder decoder(SPI_MISO, SPI_MOSI, SPI_CS, SPI_SCK, VS1003_DCS, VS1003_DREQ, VS1003_RST);
#endif

#ifdef USE_VS1053                                               //if you use the VS1053

#define VS1053_DCS 4
#define VS1053_DREQ 34
#define VS1053_CS SS //5
//#define VS1053_RST 25
#define SPI_CS VS1053_CS
#define SPI_SCK SCK //22
#define SPI_MISO MISO //23
#define SPI_MOSI MOSI //27
VS1053 player(VS1053_CS, VS1053_DCS, VS1053_DREQ);
#endif

RadioStation EinsLive(
    //"https://1liveuni-lh.akamaihd.net/i/1LIVE_HDS@179577/master.m3u8"
    "http://dg-wdr-http-fra-dtag-cdn.cast.addradio.de/wdr/1live/live/mp3/128/stream.mp3"
    //"62.27.60.49/wdr/1live/live/mp3/128/stream.mp3"
    ,
    MP3Stream, &decoder);
    
#ifdef PCStreamerStreamFileURL 
  #ifdef PCStreamerStreamType
    RadioStation PC(String(PCStreamerStreamFileURL), PCStreamerStreamType, &player);
  #endif
#endif

RadioStation *radioStations[] = {
  &EinsLive
  #ifdef PCStreamerStreamFileURL
    ,&PC
  #endif
};

RadioStation *currentStation;
int currentStationID = 0;


#ifdef USE_BUTTON_INTERRUPTS                                    //if you even use something like buttoninterrupts
  #define NEXT_BUTTON_pin 17                                    //connect the button output to this pin on your esp
  #define PREVIOUS_BUTTON_pin 16                                //connect the button output to this pin on your esp
  #define PLAY_PAUSE_BUTTON_pin 4                              //connect the button output to this pin on your esp
  #define VOLUME_UP_BUTTON_pin 0                               //connect the button output to this pin on your esp
  #define VOLUME_DOWN_BUTTON_pin 2                             //connect the button output to this pin on your esp
#endif

#ifdef SerialUSE
  #define USE_Serial Serial                                     //serial port you awant to use
#endif

WiFiMulti wifiMulti;
TaskHandle_t core0;
#ifndef USE_VS1053
  int volumePercent = 80;
#endif

void connectToStation(int station){
  #ifdef SerialUSE
    USE_Serial.println("Connect to station " + String(station));
  #endif
  if(station < sizeof(radioStations)){
    currentStationID = station;
    //currentStation->disconnect();
    currentStation = radioStations[station];
    currentStation->connect();
    #ifdef SerialUSE
      USE_Serial.println("Connected to Station " + String(currentStationID));
    #endif
  }else{
    #ifdef SerialUSE
      USE_Serial.println("Station " + String(station) + "not available");
  #endif
  }
}

void playSegment(){
  #if defined(USE_VS1053)  || defined(USE_VS1003)
    currentStation->play();
  #endif
}


#ifdef USE_BUTTON_INTERRUPTS
  void NEXT_BUTTON_pressed(){
    Variables::next_Buton = true;
  } 

  void PREVIOUS_BUTTON_pressed(){
    Variables::previous = true;
  }

  void PLAY_PAUSE_BUTTON_pressed(){
    Variables::play_pause = true;
  }

  void VOLUME_UP_BUTTON_pressed(){
    Variables::volumeUP = true;
    #ifndef USE_VS1053
    if(volumePercent + 2 <= 100){
      volumePercent+=2;
    }else{
      volumePercent = 100;
    }
    #else
      player.setVolume((player.getVolume()+2<=100 )? player.getVolume()+2 : player.getVolume());
    #endif
  }

  void VOLUME_DOWN_BUTTON_pressed(){
    Variables::volumeDOWN = true;
    #ifndef USE_VS1053
    if (volumePercent - 2 >= 0){
      volumePercent -= 2;
    }else{
      volumePercent = 0;
    }
    #else
      player.setVolume((player.getVolume() - 2 >= 0) ? player.getVolume() - 2 : player.getVolume());
    #endif
  }

#endif

  //ESP32 Core 0 loop
  void looop(void *parameter)
  {
    while (true)
    {
      if (currentStation->getType() == M3U8)
      {
        #ifdef SerialUSE
          USE_Serial.println("Buffering M3U8");
        #endif
        currentStation->buffering();
      }
      else
      {
        delay(100);
      }
    }
  }

  void setup()
  {
#ifdef SerialUSE
    USE_Serial.begin(9600);
    USE_Serial.println("Starting webradio");
  #endif
    Serial.println((decoder.connected()) ? "true" : "false");

#ifdef USE_BUTTON_INTERRUPTS
    #ifdef SerialUSE
      USE_Serial.println("Attaching interrupts");
    #endif
    attachInterrupt(NEXT_BUTTON_pin, NEXT_BUTTON_pressed, RISING);
    attachInterrupt(PREVIOUS_BUTTON_pin, PREVIOUS_BUTTON_pressed, RISING);
    attachInterrupt(PLAY_PAUSE_BUTTON_pin, PLAY_PAUSE_BUTTON_pressed, RISING);
    attachInterrupt(VOLUME_UP_BUTTON_pin, VOLUME_UP_BUTTON_pressed, RISING);
    attachInterrupt(VOLUME_DOWN_BUTTON_pin, VOLUME_DOWN_BUTTON_pressed, RISING);
    #ifdef SerialUSE
      USE_Serial.println("All button-interrupts attached");
    #endif
  #endif

  #ifdef SerialUSE
    USE_Serial.println("Adding WiFi");
  #endif

    wifiMulti.addAP(ssid1, pass1);
    //wifiMulti.addAP("GamingPC-Tom", "]W83443g");
    if (wifiMulti.run() == WL_CONNECTED)
    {
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    }
    //WiFi.begin("GamingPC-Tom", "]W83443g");

#ifdef SerialUSE
        USE_Serial.println("WiFi´s added");
  #endif

  #ifdef USE_MP3Decoder
        //SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS);
        decoder.begin();
        decoder.testConnection();
  #endif

#pragma region 
  #ifdef USE_VS1003
    #ifdef SerialUSE
      USE_Serial.println("Configure everything for VS1003");
    #endif

    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS);
    #ifdef SerialUSE
      USE_Serial.println("SPI ready");
    #endif

    player.begin();
    #ifdef SerialUSE
      USE_Serial.println("player ready");
    #endif

    /*#ifdef SerialUSE
      USE_Serial.println("Switching to MP3Mode");
    #endif

    player.switchToMp3Mode();
    #ifdef SerialUSE
      USE_Serial.println("Switched to mp3 mode");
    #endif*/

    player.setVolume(0);
    #ifdef SerialUSE
      USE_Serial.println("Volume at 100%");
    #endif
    #ifdef SerialUSE
      USE_Serial.println("VS1053 configuration successful");
    #endif
#endif

#ifdef USE_VS1053
#ifdef SerialUSE
        USE_Serial.println("Configure everything for VS1053");
#endif

        SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS);
#ifdef SerialUSE
        USE_Serial.println("SPI ready");
#endif

        player.begin();
  //const char* h;
  //if(player.testComm(h)){
#ifdef SerialUSE
    USE_Serial.println("player ready");
#endif

#ifdef SerialUSE
      USE_Serial.println("Switching to MP3Mode");
#endif

      player.switchToMp3Mode();
#ifdef SerialUSE
      USE_Serial.println("Switched to mp3 mode");
    #endif

    player.setVolume(21);
    #ifdef SerialUSE
      USE_Serial.println("Volume at 100%");
      #endif
      if(player.isChipConnected()){
      #ifdef SerialUSE
        USE_Serial.println("VS1053 configuration successful");
      #endif
    }else{
      #ifdef SerialUSE
        USE_Serial.println("VS1053 configuration NOT successful");
      #endif
    }
#endif

#pragma endregion

    connectToStation(0);

  #ifdef USE_Serial
    USE_Serial.println("Start Threading");
  #endif
  xTaskCreatePinnedToCore(
    looop,
    "Buffering",
    1000,
    NULL,
    1,
    &core0,
    0);
}

//ESP32 Core 1 loop
void loop()
{
  //if (wifiMulti.run() == WL_CONNECTED)
  if(WiFi.status() == WL_CONNECTED)
  {
    #ifdef USE_Serial
      //USE_Serial.println("WiFi connected");
    #endif
    if (Variables::next_Buton)
    {
      #ifdef USE_Serial
        USE_Serial.println("Switch to next station");
      #endif
      currentStationID = (currentStationID >= sizeof(radioStations)) ? 0 : currentStationID + 1;
      connectToStation(currentStationID);
    }
    else if (Variables::previous)
    {
      #ifdef USE_Serial
        USE_Serial.println("Switch to previous station");
      #endif
      currentStationID = (currentStationID <= 0) ? sizeof(radioStations) : currentStationID - 1;
      connectToStation(currentStationID);
    }
    #ifdef USE_Serial
      //USE_Serial.println("Play Segment");
    #endif
    playSegment();
    #ifdef USE_Serial
      //USE_Serial.println("Played Segment");
    #endif
  }
  else
  {
    if (wifiMulti.run() == WL_CONNECTED)
    {
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    }
#ifdef SerialUSE
    USE_Serial.println("Any of the passed WiFi´s is connected");
#endif
  }

  #ifdef SerialUSE
    USE_Serial.println("Currently playing station " + String(currentStationID));
  #endif
}


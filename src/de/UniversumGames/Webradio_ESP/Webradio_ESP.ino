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

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include "RadioStation.h"
#include "RadioType.h"
#include "Variables.h"
#include <SPI.h>
#include "MP3Decoder.h"
#include "WiFiVariables.h"
#include "URL.h"

//#define PCStreamerStreamFileURL "192.168.178.28/"               //if you have a programm wich provides a m3u(8) playlist in your network, define here the URL
//#define PCStreamerStreamType M3U8
#define ssid1 WiFiVariables::ssid                                      //your ssid
#define pass1 WiFiVariables::password                                  //your password

//#define USE_BUTTON_INTERRUPTS                                   //if you want to use buttons/etc. as interrupt
#define SerialUSE                                               //if you want a serial output

#define VS1003_XDCS 33
#define VS1003_DREQ 35
#define VS1003_XCS 32 
//#define VS1003_RST EN
#define SPI_CS VS1003_XCS
#define SPI_SCK 18   
#define SPI_MISO 19 
#define SPI_MOSI 23 
MP3Decoder decoder;



RadioStation EinsLive(
    URL("dg-wdr-http-dus-dtag-cdn.cast.addradio.de", "/wdr/1live/live/mp3/128/stream.mp3", 80),
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

void connectToStation(int station){
  #ifdef SerialUSE
    USE_Serial.println("Connect to station " + String(station));
  #endif
  if(station < sizeof(radioStations)){
    currentStationID = station;
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
    decoder.volumeUp();
  }

  void VOLUME_DOWN_BUTTON_pressed(){
    Variables::volumeDOWN = true;
    decoder.volumeDown();
  }

#endif

  //ESP32 Core 0 loop
  void looop(void *parameter)
  {
    while (true)
    {
      /*if (currentStation->getType() == M3U8)
      {
        #ifdef SerialUSE
          USE_Serial.println("Buffering M3U8");
        #endif
        currentStation->buffering();
      }
      else
      {
        delay(100);
      }*/
    }
  }

  void setup()
  {
#ifdef SerialUSE
    USE_Serial.begin(9600);
    USE_Serial.println("Starting webradio");
  #endif
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
    while(wifiMulti.run() != WL_CONNECTED){
      delay(100);
      Serial.print(".");
    }
    if (wifiMulti.run() == WL_CONNECTED)
    {
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    }

#ifdef SerialUSE
        USE_Serial.println("WiFi´s added");
  #endif
    decoder.begin();
    connectToStation(0);

  #ifdef USE_Serial
    USE_Serial.println("Start Threading");
  #endif
  /*xTaskCreatePinnedToCore(
    looop,
    "Buffering",
    1000,
    NULL,
    1,
    &core0,
    0);*/
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
    currentStation->play();
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


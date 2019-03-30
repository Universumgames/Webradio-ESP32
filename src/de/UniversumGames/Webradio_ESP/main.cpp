#include <Arduino.h>
#include <VS1053.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <SPI.h>
#include "WiFiVariables.h"
#include "Webradio.h"
#include "Radiostation.h"
#include "RadioType.h"
#include <SPI.h>
#include "VS1053.h"
#include "URL.h"
#include <WiFiClient.h>

#define ssid1 WiFiVariables::_ssid
#define pass1 WiFiVariables::_password

#define volume_UP 16
#define volume_DOWN 17
#define station_NEXT 21
#define station_PRE 15
#define play_pause 5

Radiostation stations[] = {Webradio_h::EinsLive};
VS1053 player(32, 33, 35);
Webradio webradio;

bool play = false;
bool up = false;
bool down = false;
bool next = false;
bool pre = false;
int volume = 50;

long lasthandle = 0;

void playpause(){
    Serial.println("playpause");
    play = true;
}

void volumeUp()
{
    Serial.println("up");
    up = true;
}

void volumeDown()
{
    Serial.println("down");
    down = true;
}

void stationNext()
{
    Serial.println("next");
    next = true;
}

void stationPre()
{
    Serial.println("pre");
    pre = true;
}

void handlePresses(){
    if(millis() - lasthandle > 300){
    if(play){
        play = false;
        webradio.playpause();
    }else if(up){
        up = false;
        volume += 2;
        player.setVolume(volume);
        Serial.println("upped");
    }else if(down){
        down = false;
        volume-=2;
        player.setVolume(volume);
        Serial.println("downed");
    }else if(next){
        next = false;
        webradio.nextStation();
    }else if(pre){
        pre = false;
        webradio.previousStation();
    }
    lasthandle = millis();
    }
}

void connectToWiFi(){
    WiFi.begin(ssid1, pass1);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED && i < 5)
    {
        delay(500);
        Serial.print(".");
        i++;
    }
    if(i>= 5) ESP.restart();
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void buttonInterruptRead(){
    //if(lastButtonPress + 200 < millis()){
        bool pressed = false;
        if(digitalRead(play_pause)){
            playpause();
            pressed = true;
        }
        /*if(digitalRead(station_NEXT)){
            stationNext();
            pressed = true;
        }
        if(digitalRead(stationPre)){
            stationPre();
            pressed = true;
        }*/
        if(digitalRead(volume_UP)){
            volumeUp();
            pressed = true;
        }
        if(digitalRead(volume_DOWN)){
            volumeDown();
            pressed = true;
        }
        /*if(pressed){
            lastButtonPress = millis();
            webradio.pause();
            webradio.play();
        }*/
    //}

}


void setup(){
    Serial.begin(9600);

    Serial.println();
    Serial.println("Starting webradio");

    Serial.println("Init \"Interrupts\"");
    pinMode(play_pause, INPUT_PULLDOWN);
    pinMode(station_NEXT, INPUT_PULLDOWN);
    pinMode(station_PRE, INPUT_PULLDOWN);
    pinMode(volume_UP, INPUT_PULLDOWN);
    pinMode(volume_DOWN, INPUT_PULLDOWN);
    attachInterrupt(play_pause, playpause, RISING);
    attachInterrupt(station_NEXT, stationNext, RISING);
    attachInterrupt(station_PRE, stationPre, RISING);
    attachInterrupt(volume_UP, volumeUp, RISING);
    attachInterrupt(volume_DOWN, volumeDown, RISING);
    Serial.println("Interrupts initialized");

    Serial.println("Init Decoder");
    SPI.begin();
    player.begin();
    player.setVolume(95);
    player.switchToMp3Mode();
    

    webradio.begin(&player);
    webradio.init();
    Serial.println("Decoder initialized");
    
    Serial.println("Connect to WIFI");
    connectToWiFi();

    Serial.println("Start playing"); 
    webradio.play();

    lasthandle = millis();
}

void loop(){
    webradio.core0Loop();
    handlePresses();
}





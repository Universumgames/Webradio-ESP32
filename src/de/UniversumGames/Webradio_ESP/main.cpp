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

Radiostation stations[] = {Webradio_h::EinsLive};
VS1053 player(32, 33, 35);
Webradio webradio;

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

WiFiClient client;

void setup(){
    Serial.begin(9600);

    Serial.println();

    SPI.begin();
    player.begin();
    player.switchToMp3Mode();
    player.setVolume(95);
    
    Serial.println("init");
    webradio.begin(&player);
    
    Serial.println("init");
    webradio.init();
    
    Serial.println("WIFI");
    connectToWiFi();

    Serial.println("Play"); 
    webradio.play();
}

void loop(){
    webradio.core0Loop();
}


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

VS1053 player(32, 33, 35);
Radiostation stations[] = {Webradio_h::EinsLive};
Webradio webradio;

TaskHandle_t core0;

void connectToWiFi(){
    WiFi.begin(ssid1, pass1);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED && i < 20)
    {
        delay(500);
        Serial.print(".");
        i++;
    }
    if(i>= 20) ESP.restart();
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void secondLoop(void * parameter)
{
    while(true){
        //webradio.core0Loop();
    }
}

void initSecondCore(){
    xTaskCreatePinnedToCore(
        secondLoop,
        "Player",
        10000,
        NULL,
        0,
        &core0,
        0);
}

void setup(){
    Serial.begin(9600);

    delay(500);
    Serial.println();

    Serial.println("second core");
    //initSecondCore();
    SPI.begin();
    
    Serial.println("init");
    webradio.begin(&player);
    /*Serial.println("add stations");
    for(int i = 0; i < sizeof(stations); i++){
        webradio.addStation(stations[i]);
    }*/
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


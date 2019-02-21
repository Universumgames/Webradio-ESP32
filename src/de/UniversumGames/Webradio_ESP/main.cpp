#include <Arduino.h>
#include <WiFi.h>
#include "WiFiVariables.h"
#include "Webradio.h"
#include <VS1053.h>
#include <SPI.h>

#define ssid1 WiFiVariables::_ssid
#define pass1 WiFiVariables::_password

Webradio webradio  = Webradio_h::webradio;
TaskHandle_t core0;

void connectToWiFi(){
    WiFi.begin(ssid1, pass1);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED && i < 200)
    {
        delay(100);
        Serial.print(".");
        i++;
    }
    if(i>= 200) ESP.restart();
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void secondLoop(void *parameter)
{
    while(true){
        webradio.core0Loop();
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

    Serial.println("second core");
    initSecondCore();
    
    Serial.println("init");
    webradio.init();
    
    Serial.println("WIFI");
    connectToWiFi();

    Serial.println("Play"); 
    webradio.play();
}

void loop(){

}
Webradio-ESP32
==============

First of all, this project is not finished, I have some problems getting the
VS1053/1003 running.... Mybe you can help me... XD
If I get this project finished, this is a very simple
webradio-player with the ESP32 and the VS1003/1053 for everyone.

Getting started
===============

If this project is finished one timee soon, you dont have to make much changes
in code to connect to your WiFi and add your own Webradiostations.

Code to be edited by you
--------------

Variables.h:
enter the SSID and Password for your WiFi
```
6   extern const char *ssid = "yourSSID";
7   extern const char *password = "yourPASSWORD";
```

Webradio_ESP32.ino:
change
```
26   #define ssid1 WiFiVariables::ssid                                      //your ssid
27   #define pass1 WiFiVariables::password                                  //your password
```
to
```
26   #define ssid1 Variables::ssid                                      //your ssid
27   #define pass1 Variables::password                                  //your password
```
If you have a "webradio" on your pc define url and type of strem (at the moment you can choose between Mp3Stream and M3U8 Playlist)
```
24  //#define PCStreamerStreamFileURL "192.168.178.28/"               //if you have a programm wich provides a m3u(8) playlist in your network, define here the URL
25  //#define PCStreamerStreamType M3U8
```
add or remove webradiostations before round about line 80, example:
```
RadioStation name(url, type of stream (MP3Stream/M3u8), &decoder);
```
and put it into the array "radiostations[]"
```
RadioStation *radioStations[] = {&name, &name2};
```

How to connect the VS1053/1003
----------
If you had already looking through the code you have probably seen some defined Variables:
```
48  #define VS1003_DCS 4            //pin 4
49  #define VS1003_DREQ 34          //pin 34
50  #define VS1003_CS SS            //pin 5
51  #define VS1003_RST 25           //pin 25
52  #define SPI_CS VS1003_CS
53  #define SPI_SCK SCK             //pin 22
54  #define SPI_MISO MISO           //pin 23
55  #define SPI_MOSI MOSI           //pin 27
```

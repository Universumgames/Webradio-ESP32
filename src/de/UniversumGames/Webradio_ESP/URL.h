#ifndef URL_h
#define URL_h

#include <Arduino.h>

class URL{
    public:
        URL(){}
        URL(const char *_host, const char *_path, uint16_t _port){
            host = _host;
            path = _path;
            port = _port;
        }
        const char *host;
        const char *path = "";
        uint16_t port = 0;

        void println(){
            Serial.print(host);
            Serial.print(path);
            Serial.print(":");
            Serial.println(String(port));
        }

        bool isNotDefined(){
            return host == "";
        }
};
#endif
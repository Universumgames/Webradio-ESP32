#ifndef URL_h
#define URL_h

#include <Arduino.h>

class URL{
    public:
        URL(String _host, String _path, int _port){
            host = _host;
            path = _path;
            port = _port;
        }
        String host;
        String path;
        int port;
};
#endif
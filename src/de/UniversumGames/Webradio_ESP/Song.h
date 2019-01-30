#ifndef Song_h
#define Song_h

#include "Queue.h"

#define Type uint8_t

long nr = 0;

class Song{
    public:
        Song(Queue<Type> * content){
            song = content;
            id = nr;
            nr++;
        }
        bool isOver(){
            return song->isEmpty();
        }

        long getID(){
            return id;
        }
        //void setSongContent(Queue<uint8_t> *content){
        //    song = content;
        //}

        Queue<Type> *getQueue()
        {
            return song;
        }

    private:
      Queue<Type> *song;
      long id;

};
#endif
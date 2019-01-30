#ifndef useful_h
#define useful_h

#include <Arduino.h>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class useful{
    public:
        static void split(String toSplit, char splitter, String *container[]){
            char array[toSplit.length()];
            toSplit.toCharArray(array, toSplit.length());
            for(int i = 0, j = 0; i < sizeof(array); i++){
                String content = "";
                while(array[j] != splitter && j <= toSplit.length()){
                    content += String(array[j]);
                    j++;
                }
            }
        }

        static int countChar(String content, char toCount){
            int counter = 0;
            char array[content.length()];
            content.toCharArray(array, content.length());
            for(int i = 0; i < content.length(); i++){
                if(array[i] == toCount) counter++;
            }
            return counter;
        }

        static byte bitToByte(bool bits[8]){
            byte dataByte;
            for(int i = 0; i < 8; i++){
                dataByte |= bits[i];
            }
            return dataByte;
        }
};

#endif
#ifndef Queue_h
#define Queue_h

#include <Arduino.h>
#include <iostream>
#include <typeinfo>
#include <array>
using namespace std;

template <typename T>
class Queue{
    public:
        Queue();
        void enqueue(T data){
                Node *newNode;
                newNode->data = data;
                //newNode->next = NULL;
                if(head == NULL){
                    head = newNode;
                    tail = newNode;
                }else{
                    tail->next = newNode;
                }
                size++;
        }
        void dequeue(){
            if(head == tail){
                head = NULL;
                tail == NULL;
            }else if(head != NULL){
                head = head->next;
            }
            size--;
        }
        T front(){
            return head->data;
        }
        bool isEmpty(){
            return head == NULL;
        }

        int getSize(){
            return size;
        }

        void getArray(T *array[100]){
            //T array[size];
            Node *next = head;
            for(int i = 0; i < size; i++){
                array[i] = &next->data;
                next = next->next;
            }
        }
    private:
        struct Node
        {
          T data;
          Node *next;
        };
        
        Node *head;
        Node *tail;
        int size;
};

#endif
#ifndef List_h
#define List_h

#include <Arduino.h>
#include <iostream>
#include <typeinfo>
#include <array>

template <typename T>
class List{
    public:
        List();

        T *get(int i){
            toFirst();
            for(;i>0; i--){
                next();
            }
            if(hasAccess()){
                return current->data;
            }else{
                return head->data;
            }
        }

        void add(T data){
            Add(data);
        }

        void remove(T data){
            Remove(data);
        }

        bool isEmpty(){
            return head == NULL;
        }

        bool hasAccess(){
            return current != NULL;
        }

        T *getNext(){
            return next()->data;
        }

        int getLength(){
            return length;
        }
    private:
        struct ListNode{
            T *data;
            ListNode *next;
            ListNode *previous;
        };
        ListNode *head;
        ListNode *tail;
        ListNode *current;
        int length = 0;

        void toFirst(){
            if(!isEmpty()){
                current = head;
            }
        }

        ListNode *next(){
            if (current != NULL && current->next != NULL)
            {
                current = current->next;
                return current;
            }else{
                return head;
            }
        }

        void Add(T item){
            ListNode *node = new ListNode();
            node->data = &item;
            if(!isEmpty()){
                node->previous = tail;
                tail->next = node;
            }else{
                head = node;
                tail = node;
            }
            length++;
        }

        void Remove(T item){
            ListNode *n;
            toFirst();
            for(int i = 0; i < length; i++){
                if (current->data == item){
                    n = current;
                    break;
                }
                next();
            }
            if(n != NULL){
                n->next->previous = n->previous;
                n->previous->next = n->next;
                length--;
            }
        }
};
#endif
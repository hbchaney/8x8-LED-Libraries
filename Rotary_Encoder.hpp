#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H


#include <Arduino.h> 


class RotaryEncoder { 
    int in_pin1, in_pin2, button_pin; 
    int8_t l,r,lrmem = 3;
    int TRANS[16];
    int lrsum = 0;
    unsigned long press_start;  

    public: 
    RotaryEncoder(int,int,int); 
    ~RotaryEncoder(); 
    int check_increment();
    unsigned long check_button(); 

};
 




#endif
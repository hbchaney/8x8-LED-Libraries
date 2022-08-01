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

class Toggle { 
//set a toggle to activate after a certain time limit 
//then when deactivated pause inputs for a certain time limit 
//set timer to trigger_length for on/off inputs that dont use time 

unsigned long trigger_length; 
unsigned long pause_length; 

unsigned long pause_start = 0; 
unsigned long trigger_start = 0; 
bool toggle_status = 0; 

public: 
Toggle (unsigned long, unsigned long); 
~Toggle (); 
bool state(unsigned long); 

};



#endif
#ifndef TIMER_CLASS
#define TIMER_CLASS


#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <Arduino.h> 



class MatrixTimer : private Adafruit_BicolorMatrix { 

//graphics pointer
const uint8_t *num_array[10];

//general blinker 
bool blink_state = true; //turns on and off 
unsigned long last_switch = 0; // last time the blink_state changed

// time state variable 
unsigned long pause_start, pause_cache, time_end;  
int minute, second, hour; 

// mode variables 
bool timer_running, reset_mode, finished_mode;  


public: 
MatrixTimer (); 
~MatrixTimer (); 
void begin (uint8_t,TwoWire*); 

//time management
void start_time ();
void reset_time(); 
void pause_time(); 

//always called functions 
void display_time ();
void update_time (); 
void update_time(int);  
//updates all the functions in timer also checks for instructions 
void update_timer (int); //the int will be instructions  

//blink management 
void update_blink (unsigned long value=500); //defualt is set to 500 ms  

}; 

#endif
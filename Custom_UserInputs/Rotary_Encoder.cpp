#include "Rotary_Encoder.hpp"

RotaryEncoder::RotaryEncoder(int pin1, int pin2, int sw) : in_pin1{pin1}, in_pin2{pin2}, button_pin{sw}, TRANS{0,-1,1,14,1,0,14,-1,-1,14,0,1,14,1,-1,0} {
 
    pinMode(pin1,INPUT_PULLUP); 
    pinMode(pin2,INPUT_PULLUP); 
    pinMode(sw,INPUT_PULLUP); 

    l = digitalRead(in_pin1);
    r = digitalRead(in_pin2);  
    press_start = 0; 
} 

int RotaryEncoder::check_increment() { 
    l = digitalRead(in_pin1);
    r = digitalRead(in_pin2);
    lrmem = ((lrmem & 0x03) << 2) + 2*l + r;
    lrsum = lrsum + TRANS[lrmem];
    /* encoder not in the neutral state */
    if(lrsum % 4 != 0) return(0);
    /* encoder in the neutral state */
    if (lrsum == 4)
        {
        lrsum=0;
        return(1);
        }
    if (lrsum == -4)
        {
        lrsum=0;
        return(-1);
        }
    /* lrsum > 0 if the impossible transition */
    lrsum=0;
    return(0);
}

unsigned long RotaryEncoder::check_button() { 
    //returns how long the button has been held down for 
    //might add in a delay after button presses of a certain length 
    if (digitalRead(button_pin) == LOW) {
        if (press_start == 0) { 
            press_start = millis(); 
            return 0; 
        } 
        return millis() - press_start; 
    }
    press_start = 0; 
    return 0; 
}

//the time for an input to trigger 
//the pause after the toggle turns back off 
Toggle::Toggle(unsigned long trig, unsigned long pause) : trigger_length(trig), pause_length(pause) {} 

bool Toggle::state(unsigned long timer) { 

}


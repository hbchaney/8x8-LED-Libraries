#include "Timer.hpp"

static const uint8_t PROGMEM  small_0[] = 
  { 0b11100000,
    0b10100000, 
    0b10100000,
    0b10100000, 
    0b11100000,
    0b00000000,
    0b00000000,
    0b00000000 },
    small_1[] = 
  { 0b01000000,
    0b11000000,
    0b01000000,
    0b01000000, 
    0b11100000,
    0b00000000,
    0b00000000,
    0b00000000 },
    small_2[] = 
  { 0b11100000,
    0b00100000,
    0b01000000,
    0b10000000, 
    0b11100000,
    0b00000000,
    0b00000000,
    0b00000000 },
    small_3[] = 
  { 0b11100000,
    0b00100000, 
    0b11100000,
    0b00100000, 
    0b11100000,
    0b00000000,
    0b00000000,
    0b00000000 },
    small_4[] = 
  { 0b00100000,
    0b10100000, 
    0b11100000,
    0b00100000, 
    0b00100000,
    0b00000000,
    0b00000000,
    0b00000000 },
    small_5[] = 
  { 0b11100000,
    0b10000000, 
    0b11100000,
    0b00100000, 
    0b11100000,
    0b00000000,
    0b00000000,
    0b00000000 },
    small_6[] = 
  { 0b01100000,
    0b10000000, 
    0b11100000,
    0b10100000, 
    0b11100000,
    0b00000000,
    0b00000000,
    0b00000000 },
    small_7[] = 
  { 0b11100000,
    0b00100000, 
    0b01000000,
    0b10000000, 
    0b10000000,
    0b00000000,
    0b00000000,
    0b00000000 },
    small_8[] = 
  { 0b11100000,
    0b10100000, 
    0b11100000,
    0b10100000, 
    0b11100000,
    0b00000000,
    0b00000000,
    0b00000000 },
    small_9[] = 
  { 0b11100000,
    0b10100000, 
    0b11100000,
    0b00100000, 
    0b00100000,
    0b00000000,
    0b00000000,
    0b00000000 },
    reset_R[] = 
    {
       0b11000000,
       0b11000000,
       0b10100000,
       0b00000000,
       0b00000000,
       0b00000000,
       0b00000000,
       0b00000000 
    };

//timer starts at zero and in reset mode 
MatrixTimer::MatrixTimer () :  pause_start{0},
                            pause_cache{0},
                            time_end{0},
                            hour{0}, // max of two hours 
                            minute{0},
                            second{0}, 
                            timer_running{false},
                            reset_mode{true},
                            finished_mode{false},
                            blink_state{true},
                            last_switch{0},
                            num_array{small_0,small_1,small_2,small_3,small_4,small_5,small_6,small_7,small_8,small_9}  {}

MatrixTimer::~MatrixTimer() {} 

void MatrixTimer::begin(uint8_t reg, TwoWire* thewire= &Wire1) {
    Adafruit_BicolorMatrix::begin(reg,thewire);
}

void MatrixTimer::display_time() { 
    if (finished_mode) {
        clear();  
        fillRect(0,0,8,8,LED_GREEN); 
        writeDisplay(); 
        return; 
    }
    //if in reset mode 
    if (reset_mode) { 
        clear(); 
        drawBitmap(5,3,num_array[minute % 10],3,5,LED_RED); // right number
        drawBitmap(1,3,num_array[minute/10],3,5,LED_RED);  //left number 
        drawBitmap(0,0,reset_R,8,3,LED_GREEN); // rest symbol 
        if (hour > 0) { 
            drawLine(0,7,0,8-hour,LED_RED); 
        }
        writeDisplay(); 
        return; 

    }
    else { 
        clear(); 
        drawBitmap(5,3,num_array[minute % 10],3,5,LED_YELLOW); // right number
        drawBitmap(1,3,num_array[minute/10],3,5,LED_YELLOW); 
        drawLine(7,0,7-((second % 10)/10)*8,0,LED_RED ); //less than 10 second line 
        drawLine(7,1,7-(second/10),1,LED_GREEN); //more than 10 second line
        drawPixel(second % 3,2, LED_RED );  //running animation 
        if (hour > 0) { 
            drawLine(0,7,0,8-hour,LED_RED); 
        }
        writeDisplay(); 
        return; 
    }
    
}

void MatrixTimer::reset_time() { 
    //sets the time to zero 
    minute = 0; 
    hour = 0; 
    second = 0; 

    //turns to reset mode 
    timer_running = false; 
    reset_mode = true; 
    finished_mode = false; 

    //resets the pause cache and the timer end 
    pause_start = 0; 
    pause_cache = 0; 
    time_end = 0; 


}

//update time for reset mode 
void MatrixTimer::update_time(int value) { 
    //increase the value of hour and minute if necessary 
    minute += value;
    hour += minute / 60;

    // make sure minute is less than 60 
    if (minute < 0) { 
        if (hour > 0) {hour -= 1;}
        minute = 0; 
    }

    minute = minute % 60; 

}

void MatrixTimer::pause_time() { 
    pause_start = millis(); 
    timer_running = false; 
}

void MatrixTimer:: start_time() { 
    //if in pause mode 
    if (pause_start > 0) { 
        pause_cache += millis() - pause_start; 
        timer_running = true; 
        pause_start = 0; //rest the pause mode 
    }
    else { 
        //if in reset mode 
        time_end = (((hour *60 *60) + (minute * 60)) * 1000) + millis(); //time for the timer to end at 
        reset_mode = false; 
        timer_running = true; 

    }



}

void MatrixTimer::update_time() { 
    //ignores reset and finished modes 
    if (reset_mode || finished_mode) { 
        return; 
    }

    //checks for pause and yeets out 
    if (pause_start > 0) {
        return; 
    }

    unsigned long time_left = (time_end + pause_cache) - (millis()); 

    //check to see if time has run out
    if ((time_end + pause_cache) < millis()) { 
        finished_mode = true;
        return;  
    }
    //hour calcs 
    hour = time_left / (1000 * 60 * 60);
    // reducing the hour from the time 
    time_left = time_left - (hour * 1000 * 60 * 60);
    // minute calc 
    minute = time_left/(1000*60); 
    //reducing the minutes 
    time_left = time_left - (minute*1000 * 60); 
    //seconds 
    second = time_left/(1000); 
}

void MatrixTimer::update_blink(unsigned long value) { 
    if ((millis() - last_switch) >= 500 ) { 
        blink_state = blink_state^1; 
    }
}

void MatrixTimer::update_timer(int instructions) { 
    /* 
        instructions are as follows: 
        0: does not change anything 
        1: increase increment by one when in reset mode 
        -1: decrease time increment by one when in reset mode 
        87: turn to reset mode 
        5: 
            reset_mode: start timer 
            timer_running : pause mode 
            pause_mode : start timer 
            finished_mode : go to reset_mode 
    */ 

    // master updater called every loop 
    update_blink(); 

    //if paused and button pressed then start up
    

    if (reset_mode) { 
        switch (instructions) { 
            case 1: 
            update_time(1); 
            break; 
            case -1: 
            update_time(-1); 
            break; 
            case 5: 
            start_time(); 
            break; 
            case 87: 
            reset_time(); 
            break; 
        }
    }
    else if (pause_start > 0 && instructions == 5) { 
        start_time(); 
    }
    else if (pause_start == 0 && instructions == 5) { 
        pause_time(); 
    }
    if (instructions == 87) { 
    reset_time(); 
        
    }

    if (finished_mode && instructions == 5) { 
        reset_time(); 
    }

    update_time(); 
    display_time(); 


}

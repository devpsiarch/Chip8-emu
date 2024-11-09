#include <iostream>
#include "./chip8.h"

int main(int argc,char **argv){
    //setting up graphics and callbacks
    setup_graphics();
    setup_input();

    //updating the timers 
    auto lasttime = high_resolution_clock::now();

    //init the system
    init();
    loadprogram();

    for(;;){
        //timer updating 
    auto current_time = high_resolution_clock::now();
    auto rntime = duration_cast<milliseconds>(current_time - lasttime);
        if(rntime.count() >= 16){
            update_timers();
            lasttime = current_time;
        } 
        emulate_cycle();
        if(draw_flage()) draw_graphics();
        set_keys();
    }
    return 0;
}
